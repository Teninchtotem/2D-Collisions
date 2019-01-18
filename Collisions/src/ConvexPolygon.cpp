
#include "ConvexPolygon.h"

void ConvexPolygon::sort(vector<float> *points)
{
	for (int i = 0; i < (points->size() - 1); i++)
	{
		for (int j = 0; j < points->size() - i - 1; j++)
		{
			if ((*points)[j] >(*points)[j + 1])
			{
				float swap = (*points)[j];
				(*points)[j] = (*points)[j + 1];
				(*points)[j + 1] = swap;
			}
		}
	}
}
bool ConvexPolygon::checkSeperation(vector<float> in)
{
	vector<float> points = in;
	points.push_back(in[0]);

	float temp = 2 * M_PI / in.size();
	float minDiff = 0.75 * (2 * M_PI / in.size());

	for (int i = 0; i < in.size(); i++)
	{
		if (abs(points[i] - points[i + 1]) < minDiff)
			return false;
	}
	return true;
}
float ConvexPolygon::calculateArea(vector<Vector2> in)
{
	vector<Vector2> points = in;
	points.push_back(in[0]);


	float Val = 0;

	for (int i = 0; i < in.size(); i++)
	{
		Val += (points[i].x * points[i + 1].y) - (points[i].y * points[i + 1].x);
	}
	return Val / 2;
}
void ConvexPolygon::generateRandomPoints(int pointCount)
{
	angle = 1;
	//Currently horribly inefficient with many redundencies (but works :D)
	//Generates random points around a circle and then throws them away and starts again if it looks dumb
	vector<float> points;
	do
	{
		vertAngles.clear();
		points.clear();
		vert.clear();
		for (int i = 0; i < pointCount; i++)
		{
			points.push_back(float(rand() % int(2.0*M_PI * 1000)) / 1000);
		}

		sort(&points);

		for (int i = 0; i < pointCount; i++)
		{
			vert.push_back(Vector2(radius * cos(points[i]), radius * sin(points[i])));
			vertAngles.push_back(points[i]);
		}
	} while (!checkSeperation(points));

	edgeDir.clear();
	for (int i = 0; i < vert.size() - 1; i++)
	{
		edgeDir.push_back(Vector2(vert[i] - vert[i + 1]));
	}

	mass = calculateArea(vert);	
	ajustForCentroid(); 
	calculateBS();
}
void ConvexPolygon::scale(const float in)
{
	setRadius(radius * in);
	for (int i = 0; i < vert.size(); i++)
	{
		vert[i] *= in;
	}
	mass = calculateArea(vert);
	calculateBS();
}

void ConvexPolygon::setRadius(const float r)
{
	radius = r;
}

vector<Vector2> ConvexPolygon::getPoints()
{
	return vert;
}

bool ConvexPolygon::updateRotationPoints()
{
	//No need to do this if the angle wont change
	if (angle == 0)
		return false;

	float tempV = angle * 0.0174533;

	vert.clear();
	for (int i = 0; i < vertAngles.size(); i++)
	{
		vertAngles[i] += tempV;
		vert.push_back(Vector2(radius * cos(vertAngles[i]), radius * sin(vertAngles[i])));
	}
	ajustForCentroid();
	angle = 0;

	return true;
}

void ConvexPolygon::ajustForCentroid()
{
	//Code adapted from http://stackoverflow.com/questions/2792443/finding-the-centroid-of-a-polygon by Emile Cormier
	centroid = Vector2(0, 0);
	double signedArea = 0.0;
	double x0 = 0.0; // Current vertex X
	double y0 = 0.0; // Current vertex Y
	double x1 = 0.0; // Next vertex X
	double y1 = 0.0; // Next vertex Y
	double a = 0.0;  // Partial signed area

	 // For all vertices except last
	int i = 0;
	for (i = 0; i < vert.size() - 1; ++i)
	{
		x0 = vert[i].x;
		y0 = vert[i].y;
		x1 = vert[i + 1].x;
		y1 = vert[i + 1].y;
		a = x0*y1 - x1*y0;
		signedArea += a;
		centroid.x += (x0 + x1)*a;
		centroid.y += (y0 + y1)*a;
	}
	x0 = vert[i].x;
	y0 = vert[i].y;
	x1 = vert[0].x;
	y1 = vert[0].y;
	a = x0*y1 - x1*y0;
	signedArea += a;
	centroid.x += (x0 + x1)*a;
	centroid.y += (y0 + y1)*a;

	signedArea *= 0.5;
	centroid.x /= (6.0*signedArea);
	centroid.y /= (6.0*signedArea);

	for (int i = 0; i < vert.size(); i++)
		vert[i] -= centroid;

	centroidMag = centroid.magnitude();
}
void ConvexPolygon::calculateBS()
{
	//Simple worst case sphere that won't have to change as the polygon rotates
	float maxDist = 0;
	for (int i = 0; i < vert.size(); i++)
	{
		float testMag = vert[i].magnitude();
		if (testMag > maxDist)
			maxDist = testMag;
	}
	BSRadius = maxDist;
}
Vector2 ConvexPolygon::getBSPosition()
{
	return position;
}
void ConvexPolygon::updateBB()
{
	//Only need to update BB if the rotation has changed, otherwise only update position
	if (updateRotationPoints())
	{
		localBBT = vert[0].y;
		localBBB = vert[0].y;
		localBBL = vert[0].x;
		localBBR = vert[0].x;

		//Set the bounding box based on the most extreme of the x and y axis
		for (int i = 1; i < vert.size(); i++)
		{
			if (vert[i].y > localBBT)
				localBBT = vert[i].y;
			else if (vert[i].y < localBBB)
				localBBB = vert[i].y;

			if (vert[i].x > localBBR)
				localBBR = vert[i].x;
			else if (vert[i].x < localBBL)
				localBBL = vert[i].x;
		}
	}
	BBT = localBBT + position.y;
	BBB = localBBB + position.y;
	BBR = localBBR + position.x;
	BBL = localBBL + position.x;

	drawingBB = true;
}
void ConvexPolygon::integrate(float duration)
{

	position += velocity*duration;
	angle += rotation*duration;

}
float ConvexPolygon::getMass()
{
	return mass;
}
float ConvexPolygon::getBSRadius()
{
	return BSRadius;
}
void ConvexPolygon::setMass(float in)
{
	mass = in;
}
void ConvexPolygon::draw()
{
	glLoadIdentity();
	glTranslatef(position.x, position.y, 0.0f);
	glRotatef(angle, 0,0, 1);
	glColor3ub(colour.x, colour.y, colour.z);	

	glBegin(GL_TRIANGLE_FAN);
	for (int j = 0; j < vert.size(); j++)
		glVertex2f(vert[j].x, vert[j].y);
	glEnd();
}
void ConvexPolygon::drawDebug()
{
	//Bounding Sphere
	glLoadIdentity();
	glTranslatef(position.x, position.y, 0.0f);
	glColor4ub(255, 0, 0, 50);
	glutSolidSphere(BSRadius, 32, 2);

	//Dynamic Bounding Box
	if (drawingBB)
	{
		glLineWidth(2);
		glLoadIdentity();
		glColor4ub(colour.x + 10, colour.y + 10, colour.z + 10, 70);
		glBegin(GL_LINE_LOOP);
		glVertex2f(BBR, BBB);
		glVertex2f(BBR, BBT);
		glVertex2f(BBL, BBT);
		glVertex2f(BBL, BBB);
		glEnd();
		drawingBB = false;
	}
}