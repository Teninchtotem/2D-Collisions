#include "HierarchicalGrid.h"

void HierarchicalGrid::setVariables(hGridSpace *TL, hGridSpace *TR, hGridSpace *BL, hGridSpace *BR, hGridSpace *parent)
{
	//Setting boundaries and neighbours
	TL->top = parent->top;
	TL->bot = ((parent->top - parent->bot) / 2) + parent->bot;
	TL->left = parent->left;
	TL->right = ((parent->right - parent->left) / 2) + parent->left;

	TR->top = parent->top;
	TR->bot = ((parent->top - parent->bot) / 2) + parent->bot;
	TR->left = ((parent->right - parent->left) / 2) + parent->left;
	TR->right = parent->right;

	BL->top = ((parent->top - parent->bot) / 2) + parent->bot;
	BL->bot = parent->bot;
	BL->left = parent->left;
	BL->right = ((parent->right - parent->left) / 2) + parent->left;

	BR->top = ((parent->top - parent->bot) / 2) + parent->bot;
	BR->bot = parent->bot;
	BR->left = ((parent->right - parent->left) / 2) + parent->left;
	BR->right = parent->right;
}
void HierarchicalGrid::split(vector<Circle*> circles, float height, float width)
{
	//Initiating variables
	open.clear();
	for (vector<hGridSpace*>::iterator i = closed.begin(); i != closed.end(); i++)
		delete *i;
	closed.clear();
	open.push_back(new hGridSpace(0));
	open[0]->bot = 0;
	open[0]->top = height * 2;
	open[0]->left = 0;
	open[0]->right = width * 2;
	open[0]->index = Vector2(0, 0);
	float trueWidth = width * 2;
	float trueHeight = height * 2;

	//Setting up the circles assuming only one grid space
	for (int i = 0; i < circles.size(); i++)
	{
		sidePos sPos;
		sPos.topThresh = (circles[i]->getPosition().y + circles[i]->getRadius() + height);
		sPos.botThresh = (circles[i]->getPosition().y - circles[i]->getRadius() + height);
		sPos.leftThresh = (circles[i]->getPosition().x - circles[i]->getRadius() + width);
		sPos.rightThresh = (circles[i]->getPosition().x + circles[i]->getRadius() + width);
		sPos.circle = circles[i];
		open[0]->sideCircles.push_back(sPos);
		open[0]->itr = 0;
	}

	//Checking if any further splits need to be made
	if (circles.size() < numberToSplit)
	{
		closed.push_back(open[0]); 
		for (int i = 0; i < circles.size(); i++)
			circles[i]->hGrids.push_back(open[0]);
		return;
	}


	int itr = 0;
	while (open.size() > 0)
	{
		int itr = open[0]->itr + 1;

		//Split up the current grid into 4
		//top left
		hGridSpace *topLeft = new hGridSpace(itr);
		hGridSpace *topRight = new hGridSpace(itr);
		hGridSpace *botLeft = new hGridSpace(itr);
		hGridSpace *botRight = new hGridSpace(itr);

		setVariables(topLeft, topRight, botLeft, botRight, open[0]);

		for (int i = 0; i < open[0]->sideCircles.size(); i++)
		{
			//Check to see if it circle is touching the Left side of the sub grid	
			if (open[0]->sideCircles[i].leftThresh < topLeft->right)
			{				
				if (open[0]->sideCircles[i].topThresh >= topLeft->bot)	//Check to see if it circle is touching the Top side of the sub grid	
					topLeft->sideCircles.push_back(open[0]->sideCircles[i]);
				
				if (open[0]->sideCircles[i].botThresh < botLeft->top)	//Check to see if it circle is touching the Bottom side of the sub grid	
					botLeft->sideCircles.push_back(open[0]->sideCircles[i]);
			}
			//Check to see if it circle is touching the right side of the sub grid	
			if (open[0]->sideCircles[i].rightThresh >= topRight->left)
			{				
				if (open[0]->sideCircles[i].topThresh >= topLeft->bot)	//Check to see if it circle is touching the Top side of the sub grid	
					topRight->sideCircles.push_back(open[0]->sideCircles[i]);
				
				if (open[0]->sideCircles[i].botThresh < botLeft->top)	//Check to see if it circle is touching the Bottom side of the sub grid	
					botRight->sideCircles.push_back(open[0]->sideCircles[i]);
			}
		}

		//Removing current open grid space
		delete open[0];
		open.erase(open.begin());


		//Sorting the 4 new grid spaces into either the closed or open list
		//If the grid space is complete, push the relivant data to the circles it contains
		if (botRight->sideCircles.size() >= numberToSplit && botRight->itr < maxItr)
		{
			open.push_back(botRight);
		}
		else
		{
			closed.push_back(botRight);
			for (int i = 0; i < botRight->sideCircles.size(); i++)
				botRight->sideCircles[i].circle->hGrids.push_back(botRight);
		}

		if (botLeft->sideCircles.size() >= numberToSplit && botLeft->itr < maxItr)
		{
			open.push_back(botLeft);
		}
		else
		{
			closed.push_back(botLeft);
			for (int i = 0; i < botLeft->sideCircles.size(); i++)
				botLeft->sideCircles[i].circle->hGrids.push_back(botLeft);
		}

		if (topRight->sideCircles.size() >= numberToSplit && topRight->itr < maxItr)
		{
			open.push_back(topRight);
		}
		else
		{
			closed.push_back(topRight);
			for (int i = 0; i < topRight->sideCircles.size(); i++)
				topRight->sideCircles[i].circle->hGrids.push_back(topRight);
		}

		if (topLeft->sideCircles.size() >= numberToSplit && topLeft->itr < maxItr)
		{
			open.push_back(topLeft);
		}
		else
		{
			closed.push_back(topLeft);
			for (int i = 0; i < topLeft->sideCircles.size(); i++)
				topLeft->sideCircles[i].circle->hGrids.push_back(topLeft);
		}
	}

}