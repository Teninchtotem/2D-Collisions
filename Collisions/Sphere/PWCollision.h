#pragma once
#pragma once

#include "../Sphere/Wall.h"
#include "ConvexPolygon.h"
#include <iostream>
using namespace std;

class PWCollision
{
public:

	ConvexPolygon* poly;
	Wall* wall;

	Vector2 MTD;
	float restitution;
	Vector2 contactNormal;
	float penetration;
	float seperatingVelocity;

	/**
	* Resolves this contact, for both velocity and interpenetration.
	*/
	void resolve();

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
	bool testCircleWall(ConvexPolygon *A, Wall *B);

	// Tests if the bounding sphere touches the wall
	bool testRadius();

	//Calculates min and max coordinates along an axis for a polygon
	void calculateInterval(Vector2 Axis, ConvexPolygon *P, float& min, float& max);
	//Calculates min and max coordinates along an axis for a wall
	void calculateInterval(Vector2 Axis, Wall *W, float& min, float& max);

	//Checks if the polygons can be seperated along an axis
	bool axisSeparatePolygons(Vector2& Axis);

	//Checks if the polygons overlap on any of the axis
	bool intersect();

	//Finds the MTD from the vector
	Vector2 findMTD(vector<Vector2> pushVector);

};