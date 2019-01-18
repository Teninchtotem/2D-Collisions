#pragma once

#include "../Sphere/Wall.h"
#include "Circle.h"
#include "ConvexPolygon.h"
#include <iostream>

using namespace std;

class CPolCollision
{
public:

	Circle* A;
	ConvexPolygon* B;

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

	/**
	* Resolves the penetration for this collision by pushing apart the circles.
	*/
	void resolvePenetration();

	//Checks if the objects are overlapping
	bool checkTrueCollision();

	/**
	* Resolves the penetration for this collision by pushing apart the circles.
	*/
	bool checkOverlap();

	/**
	* Checks if the circle and wall are colliding
	*/
	bool testCirclePol(Circle *A, ConvexPolygon *B);

};