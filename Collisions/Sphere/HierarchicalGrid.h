#pragma once
#include "Circle.h"
struct circlePos
{
	Circle* circle;
	Vector2 percentPlace;
};
struct sidePos
{
	Circle* circle;
	float topThresh;
	float botThresh;
	float leftThresh;
	float rightThresh;
	float topPercent;
	float botPercent;
	float leftPercent;
	float rightPercent;
};
struct hGridSpace
{
	hGridSpace(int itrIn) { itr = itrIn; };
	float bot, top, left, right;
	Vector2 index;
	vector<circlePos> circles;
	vector<sidePos> sideCircles;
	vector<hGridSpace*> topNeighbours, botNeighbours, leftNeighbours, rightNeighbours;
	int itr = 0;
};

class HierarchicalGrid
{
protected:
	vector<hGridSpace*> open;
	void setVariables(hGridSpace *TL, hGridSpace *TR, hGridSpace *BL, hGridSpace *BR, hGridSpace *Parent);
public:
	int numberToSplit = 4;
	int maxItr = 5;
	vector<hGridSpace*> closed;
	void split(vector<Circle*> circles, float height, float width);
};