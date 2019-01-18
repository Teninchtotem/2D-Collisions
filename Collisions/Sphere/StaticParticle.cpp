
#include "StaticParticle.h"

void StaticParticle::sort(vector<float> *points)
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
bool StaticParticle::checkSeperation(vector<float> in)
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
float StaticParticle::calculateArea(vector<Vector2> in)
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
void StaticParticle::generateRandomPoints(int pointCount)
{
	//Currently horribly inefficient with many redundencies (but works :D)
	//Generates random points around a circle and then throws them away and starts again if it looks dumb
	vector<float> points;
	do
	{
		points.clear();
		vert.clear();
		for (int i = 0; i < pointCount; i++)
		{
			points.push_back(float(rand() % int(2.0*M_PI * 1000)) / 1000);
		}

		sort(&points);

		//PIEPIEPIE  Maybe add a method to ensure a minimum distance?
		//Min = a0=0.75*(2*M_PI/N) perhapse?
		//http://stackoverflow.com/questions/21690008/how-to-generate-random-vertices-to-form-a-convex-polygon-in-c

		for (int i = 0; i < pointCount; i++)
		{
			vert.push_back(Vector2(radius * cos(points[i]), radius * sin(points[i])));
		}
	} while (!checkSeperation(points));

	mass = calculateArea(vert);
}
void StaticParticle::scale(const float in)
{
	setRadius(radius * in);
	for (int i = 0; i < vert.size(); i++)
	{
		vert[i] *= in;
	}
	mass = calculateArea(vert);
}

void StaticParticle::setRadius(const float r)
{
	radius = r;
}

vector<Vector2> StaticParticle::getPoints()
{
	return vert;
}

void StaticParticle::integrate(float duration)
{

}

void StaticParticle::setVelocity(const float x, const float y)
{

}

float StaticParticle::getMass()
{
	return 100000;
}
void StaticParticle::draw()
{
	glLoadIdentity();
	glTranslatef(position.x, position.y, 0.0f);
	glColor3ub(colour.x, colour.y, colour.z);

	glutSolidSphere(radius, 32, 2);

	/*glBegin(GL_TRIANGLE_FAN);
	for (int j = 0; j < vert.size(); j++)
		glVertex2f(vert[j].x, vert[j].y);
	glEnd();*/
}