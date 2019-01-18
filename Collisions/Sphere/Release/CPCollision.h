#pragma once

#include "../Sphere/Wall.h"
#include "Circle.h"
#include <iostream>

using namespace std;

class CPCollision
{
public:

	/**
	* Checks if the circle and point are colliding
	*/
	bool testCirclePoint(Circle *circle, float x, float y);

};