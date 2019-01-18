#pragma once

#include "../Sphere/Wall.h"
#include "Circle.h"
#include <iostream>
using namespace std;

class CWCollision
{
public:
	
	Circle* circle;
	Wall* wall;

	float restitution;
	Vector2 contactNormal;
	float penetration;

	/**
	* Resolves this contact, for both velocity and interpenetration.
	*/
	void resolve();

	/**
	* Calculates the separating velocity at this contact.
	*/
	float calculateSeparatingVelocity() const;

	/**
	* Handles the impulse calculations for this collision.
	*/
	void resolveVelocity();

	/**
	* Resolves the penetration for this collision by pushing apart the circle and the wall.
	*/
	void resolvePenetration();

	/**
	* Checks if the circle and wall are colliding
	*/
	bool testCircleWall(Circle *A, Wall *B);

};