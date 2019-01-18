#include "../Sphere/CWCollision.h"


// Contact implementation
void CWCollision::resolve()
{
	resolvePenetration();
	resolveVelocity();
}

float CWCollision::calculateSeparatingVelocity() const
{
	Vector2 relativeVelocity = circle->getVelocity();
	return relativeVelocity * contactNormal;
}

void CWCollision::resolveVelocity()
{
	// Find the velocity in the direction of the contact
	float separatingVelocity = calculateSeparatingVelocity();

	// Check if it needs to be resolved
	if (separatingVelocity > 0)
	{
		// The contact is either separating, or stationary - there's
		// no impulse required.
		return;
	}

	// Calculate the new separating velocity
	float newSepVelocity = -separatingVelocity * restitution;
	float deltaVelocity = newSepVelocity - separatingVelocity;

	// We apply the change in velocity to each object in proportion to
	// their inverse mass (i.e. those with lower inverse mass [higher
	// actual mass] get less change in velocity)..
	float totalInverseMass = 1 / circle->getMass();

	// If all particles have infinite mass, then impulses have no effect
	if (totalInverseMass <= 0) return;

	// Calculate the impulse to apply
	float impulse = deltaVelocity / totalInverseMass;

	// Find the amount of impulse per unit of inverse mass
	Vector2 impulsePerIMass = contactNormal * impulse;

	// Apply impulses: they are applied in the direction of the contact,
	// and are proportional to the inverse mass.
	circle->setVelocity(circle->getVelocity() +
		impulsePerIMass * (1 / circle->getMass())
		);
}

void CWCollision::resolvePenetration()
{
	while (testCircleWall(circle, wall))
	{
		circle->setPosition(circle->getPosition() + contactNormal * 0.2);
	}
}

bool CWCollision::testCircleWall(Circle *A, Wall *B)
{
	circle = A;
	wall = B;

	bool collision = false;
	float restitutionSet = B->getBounce() * A->bounce;
	unsigned used = 0;

	// Check for penetration
	Vector2 toParticle = A->getPosition() - B->getStart();
	Vector2 lineDirection = B->getEnd() - B->getStart();

	float projected = toParticle * lineDirection;
	float platformSqLength = lineDirection.squareMagnitude();
	float squareRadius = pow(A->getRadius() + 1.65,2); //+1.65 to give thickness to the wall to help the ball not go through

	if (projected <= 0)
	{

		// The blob is nearest to the start point
		if (toParticle.squareMagnitude() < squareRadius)
		{
			// We have a collision
			contactNormal = toParticle.unit();
			restitution = restitutionSet;
			penetration = A->getRadius() - toParticle.magnitude();
			collision = true;
		}

	}
	else if (projected >= platformSqLength)
	{
		// The blob is nearest to the end point
		toParticle = A->getPosition() - B->getEnd();
		if (toParticle.squareMagnitude() < squareRadius)
		{
			// We have a collision
			contactNormal = toParticle.unit();
			restitution = restitutionSet;
			penetration = A->getRadius() - toParticle.magnitude();
			collision = true;
		}
	}
	else
	{
		// the blob is nearest to the middle.
		float distanceToPlatform = toParticle.squareMagnitude() - projected*projected / platformSqLength;
		if (distanceToPlatform < squareRadius)
		{
			// We have a collision
			Vector2 closestPoint = B->getStart() + lineDirection*(projected / platformSqLength);

			contactNormal = (A->getPosition() - closestPoint).unit();
			restitution = restitutionSet;
			penetration = A->getRadius() - sqrt(distanceToPlatform);
			collision = true;
		}
	}
	return collision;
}