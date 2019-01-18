#include "Circle.h"


void Circle::scale(const float in)
{
	setRadius(radius * in);
}

void Circle::setRadius(const float r)
{
	radius = r;
	mass = pow(radius, 2) * M_PI;
}
void Circle::integrate(float duration)
{
	position += velocity*duration;
}
void Circle::draw()
{
	glLoadIdentity();
	glTranslatef(position.x, position.y, 0.0f);

	glColor3ub(colour.x, colour.y, colour.z);
	glutSolidSphere(radius, 32, 2);
}