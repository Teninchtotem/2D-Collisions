#pragma once
// sphereJK_OO.cpp
// particle class added to the application

#include "staticParticle.h"
#include "ConvexPolygon.h"
#include "../Sphere/CWCollision.h"
#include "../Sphere/CCCollision.h"
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
	SphereDemo();
	vector<vector<vector<Circle*>>> circleSegs;
	vector<vector<vector<Wall*>>> wallSegs;
	virtual void display();
	virtual void update();
	virtual void inputUpdate();
	virtual Circle *newRandSphereParticle();
	virtual ConvexPolygon *newRandConvexPolygonParticle();
	virtual void mouseFunk(int button, int state, float x, float y);
	virtual void moveFunk(float x, float y);
	virtual void keyFunk(unsigned char button, float x, float y);
	virtual void genericChecks(Circle *circle);
	virtual void sequentialCheckAll();
	virtual void sequentialCheckGrid();
	virtual void setOptimalGridSize();
	virtual void populateGrid();
	vector<Circle*> particle;
	vector<Wall*> wall;
	int pCount;
};
