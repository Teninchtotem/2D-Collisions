#include "../Sphere/PPCollision.h"


// Contact implementation
void PPCollision::resolve()
{
	calculateSeparatingVelocity();
	//if (seperatingVelocity >= 0) PIEPIEPIEPIE THIS THSI THIS
	if (seperatingVelocity >= 0)
		return;
	
	if (intersect())
	{
		resolvePenetration();
		resolveVelocity();
	}
}
float PPCollision::calculateSeparatingVelocity()
{
	Vector2 relativeVelocity = A->getVelocity();
	relativeVelocity -= B->getVelocity();
	seperatingVelocity = relativeVelocity * contactNormal;
	return seperatingVelocity;
}

void PPCollision::resolveVelocity()
{
	float friction = A->friction * B->friction; //0-1
	float elasticity = A->bounce * B->bounce; //0-1

	Vector2 N = MTD.unit();
	Vector2 V = A->getVelocity() - B->getVelocity();

	Vector2 Vn = N * (V*N);
	Vector2 Vt = V - Vn;

	if (Vt.magnitude() < 0.01f)
		friction = 1.01f;

	// response
	Vector2 ResV = Vt * -(friction)+Vn  * -(1 + elasticity);

	float ARatio = B->getMass() / (A->getMass() + B->getMass());
	float BRatio = A->getMass() / (A->getMass() + B->getMass());

	A->setVelocity(A->getVelocity() + ResV * ARatio);
	B->setVelocity(B->getVelocity() - ResV * BRatio);
}

bool PPCollision::checkRadiusOverlap()
{
	Vector2 diff = A->getPosition() - B->getPosition();
	if (diff.magnitude() < A->getBSRadius() + B->getBSRadius())
		return true;
	return false;
}
bool PPCollision::checkBBOverlap()
{
	if (A->BBB >= B->BBT)
		return false;
	if (A->BBT <= B->BBB)
		return false;
	if (A->BBR <= B->BBL)
		return false;
	if (A->BBL >= B->BBR)
		return false;

	return true;
}

bool PPCollision::testPolyPoly(ConvexPolygon *A, ConvexPolygon *B)
{
	this->A = A;
	this->B = B;
	//Currently a basic radius test
	Vector2 diff = A->getPosition() - B->getPosition();
	contactNormal = diff.unit() * 0.5;

	//There can be a lot of redundancy in checking both of these but it's 
	//still better than skipping stright to the propper collision tests
	if (checkRadiusOverlap())	//Initial Radius test
	{
		A->updateBB();
		B->updateBB();
		if(checkBBOverlap())	//Secondary bounding box test
			return true;
	}
	return false;
}






bool PPCollision::intersect()
{
	// potential separation axes. they get converted into push 
	vector<Vector2> axis;
	for (int j = A->vert.size()-1, i = 0; i < A->vert.size(); j = i, i++)
	{
		Vector2 E = A->vert[i] - A->vert[j];
		axis.push_back(Vector2(-E.y, E.x));
		if (axisSeparatePolygons(axis.back()))
			return false;
	}
	for (int j = B->vert.size()-1, i = 0; i < B->vert.size(); j = i, i++)
	{
		Vector2 E = B->vert[i] - B->vert[j];
		axis.push_back(Vector2(-E.y, E.x));
		if (axisSeparatePolygons(axis.back()))
			return false;
	}

	//// find the MTD among all the separation vectors 
	MTD = findMTD(axis);

	//// makes sure the push vector is pushing A away from B 
	Vector2 D = A->getPosition() - B->getPosition();
	if (D * MTD < 0.0f)
		MTD *= -1;

	return true;
}

bool PPCollision::axisSeparatePolygons(Vector2& Axis)
{
	float mina, maxa;
	float minb, maxb;

	calculateInterval(Axis, A, mina, maxa);
	calculateInterval(Axis, B, minb, maxb);

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
void PPCollision::resolvePenetration()
{
	float ARatio = B->getMass() / (A->getMass() + B->getMass());
	float BRatio = A->getMass() / (A->getMass() + B->getMass());
	A->setPosition(A->getPosition() + MTD * ARatio);
	B->setPosition(B->getPosition() - MTD * BRatio);
}
void PPCollision::calculateInterval(Vector2 Axis, ConvexPolygon *P, float& min, float& max)
{
	float d = Axis * (P->vert[0] + P->getPosition());
	min = max = d;
	for (int i = 0; i < P->vert.size(); i++)
	{
		float d = (P->vert[i] + P->getPosition()) * Axis;
		if (d < min)
			min = d;
		else
			if (d > max)
				max = d;
	}
}
Vector2 PPCollision::findMTD(vector<Vector2> pushVector)
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

Vector2 vecMatTimes(Vector2 vec, Vector2* Mat)
{
	return Vector2(0, 0);
}