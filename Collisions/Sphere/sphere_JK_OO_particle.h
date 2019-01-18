#pragma once
// sphereJK_OO.cpp
// particle class added to the application

#include "staticParticle.h"
#include "ConvexPolygon.h"
#include "../Sphere/CWCollision.h"
#include "../Sphere/CCCollision.h"
#include "../Sphere/PPCollision.h"
#include "../Sphere/PWCollision.h"
#include "../Sphere/CPolCollision.h"
#include "HierarchicalGrid.h"
#include "CPCollision.h"
#include <vector>
#include <time.h>
#include <windows.h>
#include <iostream>
#include <chrono>
#include <memory>
#include <list>
#include <set>
#include <ctime>

using namespace std;

//#define EDGE_PREDICTION	
#define THREAD_USAGE //currently useless
#define MOVE_RESIST 0.996;
//#define GRAVITY
#define DRAW_POLY_DEBUG
#define GRID
//#define HGRID		//Only circles




class SphereDemo : public Application
{
	HierarchicalGrid hGrid;
	int xstep;
	int ystep;
	Particle* target = nullptr;
	int threadCount;
	bool drawingLine = false;
	Vector2 drawingLineStart;
	float massStore;
	CPCollision CPC;
	int widthSeg = 2;
	int heightSeg = 2;
	void onResize();
	int countTick = 0;
	float testStore = 0;
	float colStore = 0;

public:
	//Particle Information
	vector<Particle*> all;
	vector<Circle*> particle;
	vector<ConvexPolygon*> polygon;
	vector<Wall*> wall;

	//Grid Information
	vector<vector<vector<Circle*>>> circleSegs;
	vector<vector<vector<ConvexPolygon*>>> polySegs;
	vector<vector<vector<Wall*>>> wallSegs;
	int pCount;

	SphereDemo();

	//Draws Everything
	virtual void display();

	//Updates everything. Checks for collisions, updates position etc.
	virtual void update();

	//Checks for user input and executes
	virtual void inputUpdate();

	//Creating a new random circle
	virtual Circle *newRandSphereParticle();

	//Creating a new random circle
	virtual ConvexPolygon *newRandConvexPolygonParticle();

	//Checks the bounds and updates for gravity (if applied)
	virtual void genericChecks(Particle *circle);

	//Checks collisions against all others
	virtual void sequentialCheckAll();

	//Checks collisions against all others in the same grid spaces
	virtual void sequentialCheckGrid();

	//Checks collisions against all others in the same hierarchical grid spaces
	virtual void sequentialCheckHierarchicalGrid();

	//Sets the optimal division of grid spaces
	virtual void setOptimalGridSize();

	//Creates the grid the correct size for future use
	virtual void populateGrid();

	//Specific Draws 
	virtual void drawGrid();
	virtual void drawHGrid();


	//Input information
	virtual void mouseFunk(int button, int state, float x, float y);
	virtual void moveFunk(float x, float y);
	virtual void keyFunk(unsigned char button, float x, float y);
};
