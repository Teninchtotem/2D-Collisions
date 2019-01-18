#pragma once

#include "coreMath.h"
#include <gl/glut.h>// OpenGL toolkit
#include <app.h>// OpenGL toolkit
#include <vector>
using namespace std;

struct hGridSpace;

class Particle
{
protected:
	Vector2 position;
	Vector2 lastPosition;
	Vector2 velocity;
	Vector3 colour;
	float rotation = 0;
	float angle = 0;
	float radius;
	float mass;
	bool moved;

public:
	vector<Vector2> grids;
	vector<hGridSpace*> hGrids;
	float bounce = 0.9;
	float friction = 0.1;
	unsigned int id = 0;
	Particle *currentTarget;
	bool atRest = false;

	Particle();
	virtual void integrate(float duration);
	virtual void scale(const float in);
	virtual void draw();
	virtual void rest();
	virtual void removeHGrid(hGridSpace* remove);

	virtual float getRadius() const;
	virtual float getRotation();
	void setPosition(Vector2 in);
	virtual void setRadius(const float r);
	virtual Vector2 getPosition() const;
	virtual Vector2 getLastPosition() const;
	virtual Vector3 getCol() const;
	virtual void setCol(const float x, const float y, const float z);
	virtual void setVelocity(const float x, const float y);
	virtual void setVelocity(Vector2 in);
	virtual void setRotation(const float in);
	virtual Vector2 getVelocity() const;
	virtual float getMass();
	virtual void setMass(float in);
	virtual bool hasMoved();
	virtual void setMoved(bool in);
	virtual void setPosition(const float x, const float y);
};