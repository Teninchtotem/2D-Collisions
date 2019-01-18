#pragma once

#include "../Sphere/Wall.h"
#include "Circle.h"
#include <iostream>

using namespace std;

class CCCollision
{
public:

	Circle* A;
	Circle* B;

	float restitution;
	Vector2 contactNormal;
	float penetration;
	float seperatingVelocity;

	/**
	* Resolves this contact, for both velocity and interpenetration.
	*/
	void resolve();

	/**
	* Calculates the separating velocity at this contact.
	*/
	float calculateSeparatingVelocity();

	/**
	* Handles the impulse calculations for this collision.
	*/
	void resolveVelocity();

	//Pushes objects away from eachother when they are not nessiarily colliding
	void passivePush();

	/**
	* Handles the impulse calculations for this collision using prediction.
	*/
	bool resolvePredictive(Circle *Ain, Circle *Bin);

	/**
	* Resolves the penetration for this collision by pushing apart the circles.
	*/
	void resolvePenetration();

	/**
	* Resolves the penetration for this collision by pushing apart the circles.
	*/
	bool checkOverlap();

	/**
	* Checks if the circle and wall are colliding
	*/
	bool testCircleCircle(Circle *A, Circle *B);

};