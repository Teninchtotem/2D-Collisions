#pragma once

#include "coreMath.h"
#include <gl/glut.h>// OpenGL toolkit
#include <app.h>// OpenGL toolkit
#include <vector>
using namespace std;

class Wall 
{
protected:
	Vector2 start, end;
	Vector3 colour;
	float bounce;
public:
	Wall(Vector2 start, Vector2 end, float bounce);
	virtual void draw();
	virtual Vector2 getStart();
	virtual Vector2 getEnd();
	virtual float getBounce();
	virtual void setStart(Vector2 in);
	virtual void setEnd(Vector2 in);
};
