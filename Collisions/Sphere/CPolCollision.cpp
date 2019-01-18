#include "../Sphere/CPolCollision.h"


// Contact implementation
void CPolCollision::resolve()
{
	calculateSeparatingVelocity();
	if (seperatingVelocity >= 0)
		return;

	//resolvePenetration();
	resolveVelocity();
}
float CPolCollision::calculateSeparatingVelocity()
{
	Vector2 relativeVelocity = A->getVelocity();
	relativeVelocity -= B->getVelocity();
	seperatingVelocity = relativeVelocity * contactNormal;
	return seperatingVelocity;
}

void CPolCollision::resolveVelocity()
{
	// Find the velocity in the direction of the contact
	float separatingVelocity = calculateSeparatingVelocity();

	// Calculate the new separating velocity
	float newSepVelocity = -separatingVelocity * restitution;
	float deltaVelocity = newSepVelocity - separatingVelocity;

	// We apply the change in velocity to each object in proportion to
	// their inverse mass (i.e. those with lower inverse mass [higher
	// actual mass] get less change in velocity)..
	float totalInverseMass = 1 / A->getMass();
	totalInverseMass += 1 / B->getMass();

	// If all particles have infinite mass, then impulses have no effect
	if (totalInverseMass <= 0) return;

	// Calculate the impulse to apply
	float impulse = deltaVelocity / totalInverseMass;

	// Find the amount of impulse per unit of inverse mass
	Vector2 impulsePerIMass = contactNormal * impulse;

	// Apply impulses: they are applied in the direction of the contact,
	// and are proportional to the inverse mass.

	A->setVelocity(A->getVelocity() + impulsePerIMass * (1 / A->getMass()));
	B->setVelocity(B->getVelocity() - impulsePerIMass * (1 / B->getMass()));
}
void CPolCollision::resolvePenetration()
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
	{
		while (checkOverlap())
		{
			i++;
			A->setPosition(A->getPosition() + (contactNormal*0.5*weightRatioA));
			B->setPosition(B->getPosition() - (contactNormal*0.5*weightRatioB));
		}
	}
}

bool CPolCollision::checkOverlap()
{
	Vector2 diff = A->getPosition() - B->getPosition();
	if (diff.magnitude() < A->getRadius() + B->getBSRadius())
		return true;
	return false;
}
bool CPolCollision::checkTrueCollision()
{
	float restitutionSet = B->bounce * A->bounce;


	for (int j = B->vert.size() - 1, i = 0; i < B->vert.size(); j = i, i++)
	{
		Vector2 Start = B->vert[i] + B->getPosition();
		Vector2 End = B->vert[j] + B->getPosition();

		// Check for penetration
		Vector2 toParticle = A->getPosition() -Start;
		Vector2 lineDirection = End -Start;

		float projected = toParticle * lineDirection;
		float platformSqLength = lineDirection.squareMagnitude();
		float squareRadius = pow(A->getRadius(), 2); 

		bool collision = false;
		if (projected <= 0)
		{

			// The blob is nearest to the start point
			if (toParticle.squareMagnitude() < squareRadius)
			{
				// We have a collision
				contactNormal = toParticle.unit();
				restitution = restitutionSet;
				penetration = A->getRadius() - toParticle.magnitude();
				return true;
			}

		}
		else if (projected >= platformSqLength)
		{
			// The blob is nearest to the end point
			toParticle = A->getPosition() - End;
			if (toParticle.squareMagnitude() < squareRadius)
			{
				// We have a collision
				contactNormal = toParticle.unit();
				restitution = restitutionSet;
				penetration = A->getRadius() - toParticle.magnitude();
				return true;
			}
		}
		else
		{
			// the blob is nearest to the middle.
			float distanceToPlatform = toParticle.squareMagnitude() - projected*projected / platformSqLength;
			if (distanceToPlatform < squareRadius)
			{
				// We have a collision
				Vector2 closestPoint =Start + lineDirection*(projected / platformSqLength);

				contactNormal = (A->getPosition() - closestPoint).unit();
				restitution = restitutionSet;
				penetration = A->getRadius() - sqrt(distanceToPlatform);
				return true;
			}
		}
	}
	return false;
}

bool CPolCollision::testCirclePol(Circle *A, ConvexPolygon *B)
{
	this->A = A;
	this->B = B;

	Vector2 diff = A->getPosition() - B->getPosition();
	contactNormal = diff.unit() * 0.5;

	bool collide = false;
	if (checkOverlap())
	{
		B->updateRotationPoints();
		if (checkTrueCollision())
			collide = true;
	}
	return(collide);
}