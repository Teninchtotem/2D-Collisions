#include "PWCollision.h"


// Contact implementation
void PWCollision::resolve()
{
	resolvePenetration();
	resolveVelocity();
}

void PWCollision::resolveVelocity()
{
	float friction = poly->friction; //0-1
	float elasticity = poly->bounce * wall->getBounce(); //0-1

	Vector2 N = MTD.unit();
	Vector2 V = poly->getVelocity();

	Vector2 Vn = N * (V*N);
	Vector2 Vt = V - Vn;

	if (Vt.magnitude() < 0.01f)
		friction = 1.01f;

	// response
	Vector2 ResV = Vt * -(friction)+Vn  * -(1 + elasticity);

	poly->setVelocity(poly->getVelocity() + ResV);
}

void PWCollision::resolvePenetration()
{
	poly->setPosition(poly->getPosition() + MTD);
}

bool PWCollision::testCircleWall(ConvexPolygon *A, Wall *B)
{
	poly = A;
	wall = B;

	if (testRadius())
		if (intersect())
			return true;
	return false;
}

bool PWCollision::testRadius()
{
	bool collision = false;
	float restitutionSet = wall->getBounce() * poly->bounce;
	unsigned used = 0;

	// Check for penetration
	Vector2 toParticle = poly->getPosition() - wall->getStart();
	Vector2 lineDirection = wall->getEnd() - wall->getStart();

	float projected = toParticle * lineDirection;
	float platformSqLength = lineDirection.squareMagnitude();
	float squareRadius = pow(poly->getBSRadius() + 1.65, 2); //+1.65 to give thickness to the wall to help the ball not go through

	if (projected <= 0)
	{

		// The blob is nearest to the start point
		if (toParticle.squareMagnitude() < squareRadius)
			collision = true;

	}
	else if (projected >= platformSqLength)
	{
		// The blob is nearest to the end point
		toParticle = poly->getPosition() - wall->getEnd();
		if (toParticle.squareMagnitude() < squareRadius)
			collision = true;
	}
	else
	{
		// the blob is nearest to the middle.
		float distanceToPlatform = toParticle.squareMagnitude() - projected*projected / platformSqLength;
		if (distanceToPlatform < squareRadius)
			collision = true;
	}
	return collision;
}
bool PWCollision::intersect()
{
	// potential separation axes. they get converted into push 
	vector<Vector2> axis;
	for (int j = poly->vert.size() - 1, i = 0; i < poly->vert.size(); j = i, i++)
	{
		Vector2 E = poly->vert[i] - poly->vert[j];
		axis.push_back(Vector2(-E.y, E.x));
		if (axisSeparatePolygons(axis.back()))
			return false;
	}
	Vector2 E = wall->getStart() - wall->getEnd();
	axis.push_back(Vector2(-E.y, E.x));
	if (axisSeparatePolygons(axis.back()))
		return false;

	//// find the MTD among all the separation vectors 
	MTD = findMTD(axis);

	//// makes sure the push vector is pushing A away from B 
	Vector2 D = poly->getVelocity() * -1;
	if (D * MTD < 0.0f)
		MTD *= -1;

	return true;
}

bool PWCollision::axisSeparatePolygons(Vector2& Axis)
{
	float mina, maxa;
	float minb, maxb;

	calculateInterval(Axis, poly, mina, maxa);
	calculateInterval(Axis, wall, minb, maxb);

	if (mina > maxb || minb > maxa)
		return true;

	// find the interval overlap 
	float d0 = maxa - minb;
	float d1 = maxb - mina;
	float depth = (d0 < d1) ? d0 : d1;

	// convert the separation axis into a push vector (re-normalise 
	// the axis and multiply by interval overlap) 
	float axis_length_squared = Axis*Axis;

	Axis *= depth / axis_length_squared;
	return false;
}
void PWCollision::calculateInterval(Vector2 Axis, ConvexPolygon *P, float& min, float& max)
{
	float d = Axis * (P->vert[0] + P->getPosition());
	min = max = d;
	for (int i = 0; i < P->vert.size(); i++)
	{
		d = (P->vert[i] + P->getPosition()) * Axis;
		if (d < min)
			min = d;
		else
			if (d > max)
				max = d;
	}
}
void PWCollision::calculateInterval(Vector2 Axis, Wall *W, float& min, float& max)
{
	float d = Axis * (W->getStart());
	min = max = d;
	d = W->getEnd() * Axis;
	if (d < min)
		min = d;
	else
		if (d > max)
			max = d;
}
Vector2 PWCollision::findMTD(vector<Vector2> pushVector)
{
	MTD = pushVector[0];
	float mind2 = pushVector[0] * pushVector[0];
	for (int i = 1; i < pushVector.size(); i++)
	{
		float d2 = pushVector[i] * pushVector[i];
		if (d2 < mind2)
		{
			mind2 = d2;
			MTD = pushVector[i];
		}
	}
	return MTD;
}