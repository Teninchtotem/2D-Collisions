#pragma once

#include "particle.h"

class ConvexPolygon: public Particle
{
protected:
	Vector2 centroid = Vector2(0, 0);
	float centroidMag = 0;
	float BSRadius = 0;
	bool drawingBB = false;
	vector<float> vertAngles;
	float localBBT = 0, localBBB = 0, localBBR = 0, localBBL = 0;

	//Ajusts the center point to be the center of mass
	virtual void ajustForCentroid();

public:
	vector<Vector2> edgeDir;
	vector<Vector2> vert;
	float BBT = 0, BBB = 0, BBR = 0, BBL = 0;

	//Updates the polygons position and rotation
	virtual void integrate(float duration);

	//Scales the vertecies
	virtual void scale(const float in);

	//calculates the bounding sphere size
	virtual void calculateBS();

	//Generates the vertecies of a random polygon
	virtual void generateRandomPoints(int pointCount);

	//Checks if the randomly generated vertecies are valid
	virtual bool checkSeperation(vector<float> points);

	//Sorts the points in order
	virtual void sort(vector<float> *points);

	//Calculates the area of the polygon
	virtual float calculateArea(vector<Vector2> in);

	//Updates the bounding box size and position
	virtual void updateBB();

	//Resets the rotation and updates the vertecies ready for a collision
	virtual bool updateRotationPoints();

	//Draws the polygon
	virtual void draw();

	//Draw debug info
	virtual void drawDebug();


	virtual vector<Vector2> getPoints();
	virtual float getBSRadius();
	virtual Vector2 getBSPosition();
	virtual void setMass(float in);
	virtual float getMass();
	virtual void setRadius(const float r);

};
