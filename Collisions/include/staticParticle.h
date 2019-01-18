#pragma once

#include "particle.h"

class StaticParticle : public Particle
{
protected:
	vector<Vector2> vert;

public:
	virtual void integrate(float duration);
	virtual void scale(const float in);
	virtual void generateRandomPoints(int pointCount);
	virtual bool checkSeperation(vector<float> points);
	virtual void sort(vector<float> *points);
	virtual float calculateArea(vector<Vector2> in);
	virtual void draw();

	virtual vector<Vector2> getPoints();
	virtual float getMass();
	virtual void setVelocity(const float x, const float y);
	virtual void setRadius(const float r);
};
