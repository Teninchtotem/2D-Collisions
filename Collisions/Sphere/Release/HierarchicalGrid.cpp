#include "HierarchicalGrid.h"

void HierarchicalGrid::setVariables(gridSpace *TL, gridSpace *TR, gridSpace *BL, gridSpace *BR, gridSpace *parent)
{
	//Setting boundaries and neighbours
	TL->top = parent->top;
	TL->bot = ((parent->top - parent->bot) / 2) + parent->bot;
	TL->left = parent->left;
	TL->right = ((parent->right - parent->left) / 2) + parent->left;
	//TL->botNeighbours.push_back(BL);
	//TL->rightNeighbours.push_back(TR);

	TR->top = parent->top;
	TR->bot = ((parent->top - parent->bot) / 2) + parent->bot;
	TR->left = ((parent->right - parent->left) / 2) + parent->left;
	TR->right = parent->right;
	//TR->leftNeighbours.push_back(TL);
	//TR->botNeighbours.push_back(BR);

	BL->top = ((parent->top - parent->bot) / 2) + parent->bot;
	BL->bot = parent->bot;
	BL->left = parent->left;
	BL->right = ((parent->right - parent->left) / 2) + parent->left;
	//BL->rightNeighbours.push_back(BR);
	//BL->topNeighbours.push_back(TL);

	BR->top = ((parent->top - parent->bot) / 2) + parent->bot;
	BR->bot = parent->bot;
	BR->left = ((parent->right - parent->left) / 2) + parent->left;
	BR->right = parent->right;
	//BR->leftNeighbours.push_back(BL);
	//BR->topNeighbours.push_back(TR);

	////Outer neighbours
	////top
	//for (int i = 0; i < parent->topNeighbours.size(); i++)
	//{
	//	if (parent->topNeighbours[i]->left < TL->right)
	//	{
	//		TL->topNeighbours.push_back(parent->topNeighbours[i]);

	//	}
	//	else
	//	{
	//		TR->topNeighbours.push_back(parent->topNeighbours[i]);

	//	}
	//}
	////bot
	//for (int i = 0; i < parent->botNeighbours.size(); i++)
	//{
	//	if (parent->botNeighbours[i]->left < BL->right)
	//	{
	//		BL->botNeighbours.push_back(parent->botNeighbours[i]);

	//	}
	//	else
	//	{
	//		BR->botNeighbours.push_back(parent->botNeighbours[i]);

	//	}
	//}
	////left
	//for (int i = 0; i < parent->leftNeighbours.size(); i++)
	//{
	//	if (parent->leftNeighbours[i]->top < TL->bot)
	//	{
	//		TL->leftNeighbours.push_back(parent->leftNeighbours[i]);

	//	}
	//	else
	//	{
	//		BL->leftNeighbours.push_back(parent->leftNeighbours[i]);

	//	}
	//}
	////right
	//for (int i = 0; i < parent->rightNeighbours.size(); i++)
	//{
	//	if (parent->rightNeighbours[i]->top < TR->bot)
	//	{
	//		TR->rightNeighbours.push_back(parent->rightNeighbours[i]);

	//	}
	//	else
	//	{
	//		BR->rightNeighbours.push_back(parent->rightNeighbours[i]);

	//	}
	//}

}
void HierarchicalGrid::split(vector<Circle*> circles, float height, float width)
{
	open.clear();
	closed.clear();
	open.push_back(new gridSpace(0));
	open[0]->bot = 0;
	open[0]->top = height * 2;
	open[0]->left = 0;
	open[0]->right = width * 2;
	open[0]->index = Vector2(0, 0);
	int numberToSplit = 4;
	int maxItr = 8;
	float trueWidth = width * 2;
	float trueHeight = height * 2;

	for (int i = 0; i < circles.size(); i++)
	{
		//circles[i]->grids.push_back(open[0]->index);
		circlePos pos;
		pos.circle = circles[i];
		pos.percentPlace = Vector2((circles[i]->getPosition().x + width) / trueWidth, (circles[i]->getPosition().y + height) / trueHeight);
		open[0]->circles.push_back(pos);
		open[0]->itr = 0;
	}

	if (circles.size() < numberToSplit)
	{
		closed.push_back(open[0]);
		return;
	}
		

	int itr = 0;
	while (open.size() > 0)
	{
		int itr = open[0]->itr + 1;
		//Split up the current grid into 4

		//top left
		gridSpace *topLeft = new gridSpace(itr);
		gridSpace *topRight = new gridSpace(itr);
		gridSpace *botLeft = new gridSpace(itr);
		gridSpace *botRight = new gridSpace(itr);
		
		setVariables(topLeft, topRight, botLeft, botRight, open[0]);

		for (int i = 0; i < open[0]->circles.size(); i++)
		{

			circlePos pos;
			pos.circle = circles[i];
			pos.circle->hGrids.clear();

			//Left
			if (open[0]->circles[i].percentPlace.x < 0.5)
			{
					pos.percentPlace.x = open[0]->circles[i].percentPlace.x * 2;

					//Top
					if (open[0]->circles[i].percentPlace.y >= 0.5)
					{
						pos.percentPlace.y = (open[0]->circles[i].percentPlace.y - 0.5) * 2;
						topLeft->circles.push_back(pos);
						pos.circle->hGrids.push_back(topLeft);
					}
					//Bottom
					else
					{
						pos.percentPlace.y = open[0]->circles[i].percentPlace.y * 2;
						botLeft->circles.push_back(pos);
						pos.circle->hGrids.push_back(botLeft);
					}

			}
			//Right
			else
			{
				pos.percentPlace.x = (open[0]->circles[i].percentPlace.x - 0.5) * 2;

				//Top
				if (open[0]->circles[i].percentPlace.y >= 0.5)
				{
					pos.percentPlace.y = (open[0]->circles[i].percentPlace.y - 0.5) * 2;
					topRight->circles.push_back(pos);
					pos.circle->hGrids.push_back(topRight);
				}
				//Bottom
				else
				{
					pos.percentPlace.y = open[0]->circles[i].percentPlace.y * 2;
					botRight->circles.push_back(pos);
					pos.circle->hGrids.push_back(botRight);
				}

			}

		}

		delete open[0];
		open.erase(open.begin());

		if (botRight->circles.size() >= numberToSplit && botRight->itr < maxItr)
		{
			open.push_back(botRight);
		}
		else
		{
			closed.push_back(botRight);
		}

		if (botLeft->circles.size() >= numberToSplit && botLeft->itr < maxItr)
		{
			open.push_back(botLeft);
		}
		else
		{
			closed.push_back(botLeft);
		}

		if (topRight->circles.size() >= numberToSplit && topRight->itr < maxItr)
		{
			open.push_back(topRight);
		}
		else
		{
			closed.push_back(topRight);
		}

		if (topLeft->circles.size() >= numberToSplit && topLeft->itr < maxItr)
		{
			open.push_back(topLeft);
		}
		else
		{
			closed.push_back(topLeft);
		}

		//PIEPIEPIE leak somewhere go fix it
	}

}