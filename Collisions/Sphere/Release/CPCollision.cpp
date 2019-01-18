#include "../Sphere/CPCollision.h"


bool CPCollision::testCirclePoint(Circle *circle, float x, float y)
{
	Vector2 diff = circle->getPosition() - Vector2(x, y);
	if (diff.magnitude() < circle->getRadius())
		return true;
	return false;
}