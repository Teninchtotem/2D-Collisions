#include "Wall.h"


Wall::Wall(Vector2 start, Vector2 end, float bounce)
{
	this->bounce = bounce;
	this->start = start;
	this->end = end;
	colour.x = colour.y = colour.z = 1;
}
Vector2 Wall::getStart()
{
	return start;
}
Vector2 Wall::getEnd()
{
	return end;
}
float Wall::getBounce()
{
	return bounce;
}
void Wall::setStart(Vector2 in)
{
	start = in;
}
void Wall::setEnd(Vector2 in)
{
	start = in;
}

void Wall::draw()
{
	glLoadIdentity();
	glColor3f(colour.x, colour.y, colour.z);
	glLineWidth(10);
	glBegin(GL_LINES);
	glVertex3f(start.x, start.y, 0.0);
	glVertex3f(end.x, end.y, 0);
	glEnd();
}