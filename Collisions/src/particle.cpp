
#include "particle.h"


Particle::Particle()
{
	colour.x = colour.y = colour.z = position.x = position.y = radius = moved =
		lastPosition.x = lastPosition.y = rotation = mass = velocity.x = velocity.y = 0;
}
void Particle::setPosition(const float x, const float y)
{
	lastPosition = position;
	position.x = x;
	position.y = y;
}
void Particle::setPosition(Vector2 in)
{
	lastPosition = position;
	position = in;
}

void Particle::scale(const float in)
{

}

void Particle::setRadius(const float r)
{
	radius = r;
}

Vector2 Particle::getPosition() const
{
    return position;
}

float Particle::getRadius() const
{
    return radius;
}

void Particle::setRotation(const float in)
{
	rotation = in;
}
float Particle::getRotation()
{
	return rotation;
}

void Particle::integrate(float duration)
{
	lastPosition = position;
	position += velocity*duration;
}

Vector3 Particle::getCol() const
{
	return colour;
}

void Particle::setCol(const float x, const float y, const float z)
{
	colour.x = x;
	colour.y = y;
	colour.z = z;
}

void Particle::setVelocity(const float x, const float y)
{
	if (x > 0 && y > 0)
		atRest = false;
    velocity.x = x;
    velocity.y = y;
}


void Particle::setVelocity(Vector2 in)
{
	if (in.x > 0 && in.y > 0)
		atRest = false;
	velocity = in;
}
Vector2 Particle::getVelocity() const
{
    return velocity;
}
float Particle::getMass()
{
	return mass;
}
void Particle::setMass(float in)
{
	mass = in;
}
void Particle::draw()
{

}
void Particle::rest()
{
	atRest = true;
	//setVelocity(0, 0);
}
Vector2 Particle::getLastPosition() const
{
	return lastPosition;
}
bool Particle::hasMoved()
{
	return moved;
}
void Particle::setMoved(bool in)
{
	moved = in;
}

void Particle::removeHGrid(hGridSpace* remove)
{
	for (vector<hGridSpace*>::iterator i = hGrids.begin(); i != hGrids.end(); i++)
	{
		if (*i == remove)
		{
			hGrids.erase(i);
			return;
		}
	}
}