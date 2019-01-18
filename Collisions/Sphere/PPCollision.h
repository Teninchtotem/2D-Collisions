#pragma once

#include "../Sphere/Wall.h"
#include "ConvexPolygon.h"
#include <iostream>

using namespace std;

class PPCollision
{
public:

	Vector2 MTD; 
	ConvexPolygon* A;
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
	* Checks if the bounding circles are overlapping
	*/
	bool checkRadiusOverlap();

	/**
	* Checks if the bounding boxes are overlapping
	*/
	bool checkBBOverlap();

	/**
	* Checks if the circle and wall are colliding
	*/
	bool testPolyPoly(ConvexPolygon *A, ConvexPolygon *B);

	//Pushes the polygons away from eachother
	void resolvePenetration();

	//Calculates min and max coordinates along an axis for a polygon
	void calculateInterval(Vector2 Axis, ConvexPolygon *P, float& min, float& max);

	//Checks if the polygons can be seperated along an axis
	bool axisSeparatePolygons(Vector2& Axis);

	//Checks if the polygons overlap on any of the axis
	bool intersect();

	//Finds the MTD from the vector
	Vector2 findMTD(vector<Vector2> pushVector); 
};