#pragma once

#include "../Sphere/Wall.h"
#include "Circle.h"
#include "ConvexPolygon.h"
#include <iostream>

using namespace std;

class CPCollision
{
public:

	/**
	* Checks if the circle and point are colliding
	*/
	bool testCirclePoint(Circle *circle, float x, float y);
	/**
	* Checks if the polygon and point are colliding - currently just radius test
	*/
	bool testPolygonPoint(ConvexPolygon *polygon, float x, float y);

};