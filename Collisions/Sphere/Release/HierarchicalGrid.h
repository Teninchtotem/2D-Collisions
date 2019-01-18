#pragma once
#include "Circle.h"
struct circlePos
{
	Circle* circle;
	Vector2 percentPlace;
};
struct gridSpace
{
	gridSpace(int itrIn) { itr = itrIn; };
	float bot, top, left, right;
	Vector2 index;
	vector<circlePos> circles;
	vector<gridSpace*> topNeighbours, botNeighbours, leftNeighbours, rightNeighbours;
	int itr = 0;
};

class HierarchicalGrid
{
protected:
	vector<gridSpace*> open;
	void setVariables(gridSpace *TL, gridSpace *TR, gridSpace *BL, gridSpace *BR, gridSpace *Parent);
public:
	vector<gridSpace*> closed;
	void split(vector<Circle*> circles, float height, float width);
};