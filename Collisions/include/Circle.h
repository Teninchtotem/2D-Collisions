#pragma once

/**
* A particle is the simplest object that can be simulated in the
* physics system.

*/
#include "particle.h"

class Circle: public Particle
{
protected:

public:
	//Updates the circle
	virtual void integrate(float duration);

	//Changes the size of the circle
	virtual void scale(const float in);
	
	//Draws the circle
	virtual void draw();

	virtual void setRadius(const float in);
};
