#include "../Sphere/CCCollision.h"


// Contact implementation
void CCCollision::resolve()
{
	calculateSeparatingVelocity();
	///if (seperatingVelocity < 0)
	//{
		resolvePenetration();
		resolveVelocity();
	//}
	//else
		//passivePush();
}
float CCCollision::calculateSeparatingVelocity()
{
	Vector2 relativeVelocity = A->getVelocity();
	relativeVelocity -= B->getVelocity();
	seperatingVelocity = relativeVelocity * contactNormal;
	return seperatingVelocity;
}

void CCCollision::resolveVelocity()
{
	// Check if it needs to be resolved
	if (seperatingVelocity > 0)
		return;

	float restitutionSet = B->bounce * A->bounce;

	Vector2 AVel = A->getVelocity();
	Vector2 BVel = B->getVelocity();
	Vector2 APos = A->getPosition();
	Vector2 BPos = B->getPosition();
	Vector2 diff = APos - BPos;

	float ARad = A->getRadius();
	float BRad = B->getRadius();
	float AMass = A->getMass();
	float BMass = B->getMass();

	// First, find the normalized vector n from the center of 
	// circle1 to the center of circle2
	Vector2 n = APos - BPos;
	n.normalise();

	float a1 = AVel.scalarProduct(n);
	float a2 = BVel.scalarProduct(n);

	// Using the optimized version, 
	// optimizedP =  2(a1 - a2)
	//              -----------
	//                m1 + m2
	float optimizedP = (2.0 * (a1 - a2)) / (AMass + BMass);

	Vector2 v1N = AVel - n * optimizedP * BMass * restitutionSet;
	Vector2 v2N = BVel + n * optimizedP * AMass * restitutionSet;

	/*if ((A->atRest || B->atRest) && v1N.magnitude() < 0.7 && v2N.magnitude() < 0.7)
	{
		if(!A->atRest)
			A->setVelocity(v1N.x * 0.5, v1N.y * 0.5);
		if(!B->atRest)
			B->setVelocity(v2N.x * 0.5, v2N.y * 0.5);
		A->rest();
		B->rest();
	}
	else*/
	{
		A->setVelocity(v1N.x, v1N.y);
		B->setVelocity(v2N.x, v2N.y);
	}
}
void CCCollision::passivePush()
{
	Vector2 APos = A->getPosition();
	Vector2 BPos = B->getPosition();
	float AMass = A->getMass();
	float BMass = B->getMass();
	float ARad = A->getRadius();
	float BRad = B->getRadius();

	Vector2 n = APos - BPos;
	n *= 0.2;
	//n.normalise();

	float BRatio = 0; 
	float ARatio = 0; 

	if (BMass > AMass)
	{
		ARatio = 1;
		BRatio = AMass / BMass;
	}
	else
	{
		BRatio = 1;
		ARatio = BMass / AMass;
	}
	int count = 0;
	float pushVal = 0.4;
	float viscosity = 0.6;
	//if (contactNormal != Vector2(0, 0))
	//	while (checkOverlap() && count < 5)
	//	{
	//		count++;
	//		A->setPosition(A->getPosition() + (n * viscosity * ARatio));
	//		B->setPosition(B->getPosition() - (n * viscosity * BRatio));
	//	}
	A->setVelocity(A->getVelocity() + (n * pushVal * ARatio));
	B->setVelocity(B->getVelocity() - (n * pushVal * BRatio));
}

bool CCCollision::resolvePredictive(Circle *Ain, Circle *Bin)
{
	A = Ain;
	B = Bin;
	calculateSeparatingVelocity();
	Vector2 C = A->getPosition() - B->getPosition();
	Vector2 totalVel = B->getVelocity() - A->getVelocity();
	float lengthC = C.magnitude();
	float sumRadii = A->getRadius() + B->getRadius();
	//If these conditions are met they have a good chance of hitting each other
	if (lengthC < totalVel.magnitude() + sumRadii/* && testMovementCollision(A, B)*/)
	{
		Vector2 N = totalVel;
		N.normalise();
		double D = N.scalarProduct(C);
		if (D <= 0)
			return false;

		double F = (lengthC * lengthC) - (D * D);

		double sumRadiiSquared = sumRadii * sumRadii;
		if (F >= sumRadiiSquared)
			return false;

		double T = sumRadiiSquared - F;

		if (T < 0)
			return false;

		double distance = D - sqrt(T);

		double mag = totalVel.magnitude();

		if (mag < distance)
			return false;

		Vector2 tempAVel = A->getVelocity();
		Vector2 tempBVel = B->getVelocity();

		tempAVel.normalise();
		tempBVel.normalise();

		float ratio = A->getVelocity().magnitude() / B->getVelocity().magnitude();

		if (A->getVelocity().magnitude() > 0)
		{
			Vector2 movement = A->getVelocity();
			movement.normalise();
			movement *= distance / (1 + (1 / ratio));
			float percentageDistance = movement.magnitude() / A->getVelocity().magnitude();
			A->integrate(percentageDistance);
		}


		if (B->getVelocity().magnitude() > 0)
		{
			Vector2 movement = B->getVelocity();
			movement.normalise();
			movement *= distance / (1 + ratio);
			float percentageDistance = movement.magnitude() / B->getVelocity().magnitude();
			B->integrate(percentageDistance);
		}

		A->setMoved(true);
		B->setMoved(true);
		resolveVelocity();
		return true;
	}
	return false;
}

void CCCollision::resolvePenetration()
{
	float weightRatioB = A->getMass() / B->getMass();
	float weightRatioA = B->getMass() / A->getMass();
	if (weightRatioB > weightRatioA)
	{
		weightRatioA /= weightRatioB;
		weightRatioB = 1;
	}
	else
	{
		weightRatioB /= weightRatioA;
		weightRatioA = 1;
	}
	int i = 0;
	if (contactNormal != Vector2(0, 0))
		while (checkOverlap())
		{
			i++;
			A->setPosition(A->getPosition() + (contactNormal*0.5*weightRatioA));
			B->setPosition(B->getPosition() - (contactNormal*0.5*weightRatioB));
		}
	//cout << i << endl;
}

bool CCCollision::checkOverlap()
{
	Vector2 diff = A->getPosition() - B->getPosition();
	if (diff.magnitude() < A->getRadius() + B->getRadius())
		return true;
	return false;
}

bool CCCollision::testCircleCircle(Circle *A, Circle *B)
{
	this->A = A;
	this->B = B;
	//Currently a basic radius test
	Vector2 diff = A->getPosition() - B->getPosition();
	contactNormal = diff.unit() * 0.5;
	return(checkOverlap());
}