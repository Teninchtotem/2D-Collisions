#include "../Sphere/sphere_JK_OO_particle.h"

SphereDemo::SphereDemo()
{
	int spheres = 0;
	int polygons = 0;
	srand(time(0));

	//Getting number fo cores to know optimum threading number
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	for (int i = 0; i < spheres; i++)
		particle.push_back(newRandSphereParticle());

	for (int i = 0; i < polygons; i++)
		polygon.push_back(newRandConvexPolygonParticle());

	widthSeg = 2;
	heightSeg = 2;
	populateGrid();
	xstep = 2;
	ystep = 2;
}
void SphereDemo::setOptimalGridSize()
{
	if (particle.size() <= 0)
		return;

	//Old method using the average radius
	/*float avRad = 0;
	for (int i = 0; i < particle.size(); i++)
		avRad += particle[i]->getRadius();
	avRad /= particle.size();

	float newDist = float(width) / (avRad * 2);
	widthSeg = newDist;
	newDist = float(height) / (avRad * 2);
	heightSeg = newDist;*/

	float pCsqrt = (int)sqrt(particle.size());

	widthSeg = pCsqrt;
	heightSeg = pCsqrt;

	populateGrid();
}
void SphereDemo::populateGrid()
{
	//Setting up the grid vectors in preperation
	wallSegs.clear();
	circleSegs.clear();
	polySegs.clear();
	for (int i = 0; i < widthSeg; i++)
	{
		vector <vector<Circle*>> pushW;
		vector <vector<Wall*>> pushWWall;
		vector <vector<ConvexPolygon*>> pushWPol;
		circleSegs.push_back(pushW);
		wallSegs.push_back(pushWWall);
		polySegs.push_back(pushWPol);

		for (int j = 0; j < heightSeg; j++)
		{
			vector<Circle*> pushH;
			vector<Wall*> pushHWall;
			vector<ConvexPolygon*> pushHWPol;
			circleSegs[i].push_back(pushH);
			wallSegs[i].push_back(pushHWall);
			polySegs[i].push_back(pushHWPol);
		}
	}
}
Circle *SphereDemo::newRandSphereParticle()
{
	Circle *newParticle = new Circle;
	newParticle->setPosition((rand() % 150) - 75, (rand() % 150) - 75);
	newParticle->setVelocity((rand() % 40) - 20, (rand() % 40) - 20);
	newParticle->setCol((rand() % 255), (rand() % 255), (rand() % 255));
	newParticle->setRotation(float((rand() % 100) - 50) / 10);
	newParticle->setRadius((rand() % 15) + 5);

	return newParticle;
}
ConvexPolygon *SphereDemo::newRandConvexPolygonParticle()
{
	ConvexPolygon *newParticle = new ConvexPolygon;
	newParticle->setPosition((rand() % 150) - 75, (rand() % 150) - 75);
	newParticle->setVelocity((rand() % 40) - 20, (rand() % 40) - 20);
	newParticle->setCol((rand() % 255), (rand() % 255), (rand() % 255));
	newParticle->setRotation(float((rand() % 100) - 50) / 10);
	newParticle->setRadius((rand() % 15) + 5);
	newParticle->generateRandomPoints((rand() % 4) + 3);

	return newParticle;
}
void SphereDemo::display(void)
{
	Application::display();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Define draws
	//#####################################################################################
#ifdef HGRID
	drawHGrid();
#endif // HGRID

#ifdef GRID
	drawGrid();
#endif // GRID

#ifdef DRAW_POLY_DEBUG
	for (int i = 0; i < polygon.size(); i++)
		polygon[i]->drawDebug(); //Drawing circles around the polygons
#endif // DRAW_POLY_DEBUG
	//#####################################################################################


	//Draw in reverse order so clicking has priority on front object
	for (int i = particle.size() - 1; i >= 0; i--)
		particle[i]->draw();

	for (int i = 0; i < wall.size(); i++)
		wall[i]->draw();

	for (int i = 0; i < polygon.size(); i++)
		polygon[i]->draw();

	//Draw line from object to cursor
	if (target != nullptr && nowB == GLUT_RIGHT_BUTTON && nowS == GLUT_DOWN)
	{
		glColor3ub(target->getCol().x * 1.5, target->getCol().y * 1.5, target->getCol().z * 1.5);
		glLoadIdentity();
		glLineWidth(4);
		glBegin(GL_LINES);
		glVertex2f(target->getPosition().x, target->getPosition().y);
		glVertex2f(nowX, nowY);
		glEnd();
	}
	//Draw line from first click to cursor
	if (drawingLine)
	{
		glColor3f(1, 1, 1);
		glLoadIdentity();
		glLineWidth(10);
		glBegin(GL_LINES);
		glVertex2f(drawingLineStart.x, drawingLineStart.y);
		glVertex2f(nowX, nowY);
		glEnd();
	}
	glutSwapBuffers();
}
void SphereDemo::sequentialCheckAll()
{
	int col = 0;
	int check = 0;
	for (int i = 0; i < particle.size(); i++)
	{
		//Checking for Circle/Circle Collisions
		CCCollision CCCol;
		for (int j = i + 1; j < particle.size(); j++)
		{
			check++;
#ifdef EDGE_PREDICTION
			CCCol.resolvePredictive(particle[i], particle[j]);
#else
			if (CCCol.testCircleCircle(particle[i], particle[j]))
			{
				col++;
				CCCol.resolve();
			}
#endif
		}

		//Checking for Circle/Wall collisions
		CWCollision CWCol;
		for (int j = 0; j < wall.size(); j++)
		{
			check++;
			if (CWCol.testCircleWall(particle[i], wall[j]))
			{
				col++;
				CWCol.resolve();
			}
		}
		//float duration = timeInterval / 1000;

#ifdef EDGE_PREDICTION
		if (!particle[i]->hasMoved())
			particle[i]->integrate(1);
#endif
		genericChecks(particle[i]);

	}
	colStore += col;
	testStore += check;
	if (countTick >= 100)
	{
		COORD co;
		co.X = 0;
		co.Y = 1;
		SetConsoleCursorPosition(console, co);
		cout << "							" << "\n" << "							";
		SetConsoleCursorPosition(console, co);
		cout << "Avg TPF: " << testStore / 100 << "\n" << "Avg CPF: " << colStore / 100;
		testStore = 0;
		colStore = 0;
	}
}
void SphereDemo::sequentialCheckGrid()
{
	int check = 0;
	int col = 0;
	for (int i = 0; i < particle.size(); i++)
	{
		Circle *thisCircle = particle[i];

		for (int j = 0; j < thisCircle->grids.size(); j++)
		{
			int xIndex = thisCircle->grids[j][0];
			int yIndex = thisCircle->grids[j][1];

			//Checking for other circles
			CCCollision CCCol;
			for (int k = 0; k < circleSegs[xIndex][yIndex].size(); k++)
			{
				Circle *target = circleSegs[xIndex][yIndex][k];
				if (target != thisCircle && target->id > thisCircle->id
					&& target->currentTarget != thisCircle)
				{
					check++;
					if (CCCol.testCircleCircle(thisCircle, target))
					{
						col++;
						target->currentTarget = thisCircle;
						CCCol.resolve();
					}
				}
			}

			//Checking for walls
			CWCollision CWCol;
			for (int k = 0; k < wallSegs[xIndex][yIndex].size(); k++)
			{
				check++;
				if (CWCol.testCircleWall(thisCircle, wallSegs[xIndex][yIndex][k]))
				{
					CWCol.resolve();
					col++;
				}
			}

			//Checking for Polygons
			CPolCollision CPCol;
			for (int k = 0; k < polySegs[xIndex][yIndex].size(); k++)
			{
				check++;
				if (CPCol.testCirclePol(thisCircle, polySegs[xIndex][yIndex][k]))
				{
					CPCol.resolve();
					col++;
				}
			}

		}

	}
	for (int i = 0; i < polygon.size(); i++)
	{
		ConvexPolygon *thisPoly = polygon[i];

		for (int j = 0; j < thisPoly->grids.size(); j++)
		{
			int xIndex = thisPoly->grids[j][0];
			int yIndex = thisPoly->grids[j][1];

			//Checking for other polygons
			PPCollision PPCol;
			for (int k = 0; k < polySegs[xIndex][yIndex].size(); k++)
			{
				ConvexPolygon *target = polySegs[xIndex][yIndex][k];
				if (target != thisPoly && target->id > thisPoly->id
					&& target->currentTarget != thisPoly)
				{
					check++;
					if (PPCol.testPolyPoly(thisPoly, target))
					{
						col++;
						target->currentTarget = thisPoly;
						PPCol.resolve();
					}
				}
			}

			//Checking for Walls
			PWCollision PWCol;
			for (int k = 0; k < wallSegs[xIndex][yIndex].size(); k++)
			{
				check++;
				if (PWCol.testCircleWall(thisPoly, wallSegs[xIndex][yIndex][k]))
				{
					PWCol.resolve();
					col++;
				}
			}
		}

	}

	//Output Statistics
	colStore += col;
	testStore += check;
	if (countTick >= 100)
	{
		COORD co;
		co.X = 0;
		co.Y = 1;
		SetConsoleCursorPosition(console, co);
		cout << "							" << "\n" << "							";
		SetConsoleCursorPosition(console, co);
		cout << "Avg TPF: " << testStore / 100 << "\n" << "Avg CPF: " << colStore / 100;
		testStore = 0;
		colStore = 0;
	}
}
void SphereDemo::sequentialCheckHierarchicalGrid()
{
	int check = 0;
	int col = 0;
	for (int i = 0; i < particle.size(); i++)
	{
		Circle *thisCircle = particle[i];

		for (int j = 0; j < thisCircle->hGrids.size(); j++)
		{
			hGridSpace *gridSpace = thisCircle->hGrids[j];

			//Checking for other circles
			CCCollision CCCol;
			for (int k = 0; k < gridSpace->sideCircles.size(); k++)
			{
				Circle *target = gridSpace->sideCircles[k].circle;
				if (target != thisCircle && target->id > thisCircle->id
					&& target->currentTarget != thisCircle)
				{
					check++;
					if (CCCol.testCircleCircle(thisCircle, target))
					{
						col++;
						target->currentTarget = thisCircle;
						CCCol.resolve();
					}
				}
			}

			//Checking for walls
			CWCollision CWCol;
			for (int k = 0; k < wall.size(); k++)
			{
				check++;
				if (CWCol.testCircleWall(thisCircle, wall[k]))
				{
					CWCol.resolve();
					col++;
				}
			}

		}

	}
	colStore += col;
	testStore += check;
	if (countTick >= 100)
	{
		COORD co;
		co.X = 0;
		co.Y = 1;
		SetConsoleCursorPosition(console, co);
		cout << "							" << "\n" << "							";
		SetConsoleCursorPosition(console, co);
		cout << "Avg TPF: " << testStore / 100 << "\n" << "Avg CPF: " << colStore / 100;
		testStore = 0;
		colStore = 0;
	}
}
void SphereDemo::update(void)
{
	inputUpdate();

	for (int i = 0; i < particle.size(); i++)
	{
		//Also use this loop to update positions
#ifdef EDGE_PREDICTION
		particle[i]->setMoved(false);
#else
		particle[i]->atRest = false; (/*duration*/ 1);
		particle[i]->integrate(/*duration*/ 1);
		particle[i]->grids.clear();
		particle[i]->hGrids.clear();
		particle[i]->currentTarget = nullptr;
#endif // EDGE_PREDICTION

#ifdef GRID
		float rad = particle[i]->getRadius();
		//Assigning left to segments
		int xLeftIndex = max(0, min(((((particle[i]->getPosition().x - rad) / 2) / width) + 0.5) * widthSeg, widthSeg - 1));
		//Assigning right to segments
		int xRightIndex = max(0, min(((((particle[i]->getPosition().x + rad) / 2) / width) + 0.5) * widthSeg, widthSeg - 1));
		//Assigning top to segments
		int yTopIndex = max(0, min(((((particle[i]->getPosition().y + rad) / 2) / height) + 0.5) * heightSeg, heightSeg - 1));
		//Assigning bottom to segments
		int yBotIndex = max(0, min(((((particle[i]->getPosition().y - rad) / 2) / height) + 0.5) * heightSeg, heightSeg - 1));

		for (int j = xLeftIndex; j <= xRightIndex; j++)
		{
			for (int k = yBotIndex; k <= yTopIndex; k++)
			{
				circleSegs[j][k].push_back(particle[i]);
				particle[i]->grids.push_back(Vector2(j, k));
			}
		}
#endif // GRID
	}
	for (int i = 0; i < polygon.size(); i++)
	{
		//Distributing particles fairly. This is because particle 0 is much slower than particle 10
		//Particle 0 cycles through ALL particles and the last particle cycles through none
		//This would not be efficient if distributed linearly 
		//tData[i % threadCount]->threadTargets.push_back(polygon[i]);

		//Also use this loop to update positions
#ifdef EDGE_PREDICTION
		polygon[i]->setMoved(false);
#else
		polygon[i]->atRest = false; (/*duration*/ 1);
		polygon[i]->integrate(/*duration*/ 1);
		polygon[i]->grids.clear();
		polygon[i]->hGrids.clear();
		polygon[i]->currentTarget = nullptr;
#endif // EDGE_PREDICTION

#ifdef GRID
		float rad = polygon[i]->getRadius();
		//Assigning left to segments
		int xLeftIndex = max(0, min(((((polygon[i]->getPosition().x - rad) / 2) / width) + 0.5) * widthSeg, widthSeg - 1));
		//Assigning right to segments
		int xRightIndex = max(0, min(((((polygon[i]->getPosition().x + rad) / 2) / width) + 0.5) * widthSeg, widthSeg - 1));
		//Assigning top to segments
		int yTopIndex = max(0, min(((((polygon[i]->getPosition().y + rad) / 2) / height) + 0.5) * heightSeg, heightSeg - 1));
		//Assigning bottom to segments
		int yBotIndex = max(0, min(((((polygon[i]->getPosition().y - rad) / 2) / height) + 0.5) * heightSeg, heightSeg - 1));

		for (int j = xLeftIndex; j <= xRightIndex; j++)
		{
			for (int k = yBotIndex; k <= yTopIndex; k++)
			{
				polySegs[j][k].push_back(polygon[i]);
				polygon[i]->grids.push_back(Vector2(j, k));
			}
		}
#endif // GRID
	}
#ifdef GRID
	for (int i = 0; i < wall.size(); i++)
	{
		//Assigning start and end to segments
		int startX = max(0, min(((((wall[i]->getStart().x) / 2) / width) + 0.5) * widthSeg, widthSeg - 1));
		int startY = max(0, min(((((wall[i]->getStart().y) / 2) / height) + 0.5) * heightSeg, heightSeg - 1));
		int endX = max(0, min(((((wall[i]->getEnd().x) / 2) / width) + 0.5) * widthSeg, widthSeg - 1));
		int endY = max(0, min(((((wall[i]->getEnd().y) / 2) / height) + 0.5) * heightSeg, heightSeg - 1));

		int tempStore = 0;
		if (endX < startX)
		{
			tempStore = startX;
			startX = endX;
			endX = tempStore;
		}
		if (endY < startY)
		{
			tempStore = startY;
			startY = endY;
			endY = tempStore;
		}

		for (int j = startX; j <= endX; j++)
		{
			for (int k = startY; k <= endY; k++)
			{
				wallSegs[j][k].push_back(wall[i]);
			}
		}
	}
#endif

#ifdef GRID
	sequentialCheckGrid();

#else 
#ifdef HGRID
	hGrid.split(particle, height, width);
	sequentialCheckHierarchicalGrid();
#else
	sequentialCheckAll();
#endif
#endif



#ifdef GRID
	for (int i = 0; i < widthSeg; i++)
	{
		for (int j = 0; j < heightSeg; j++)
		{
			polySegs[i][j].clear();
			circleSegs[i][j].clear();
			wallSegs[i][j].clear();
		}
	}
#endif

	for (int i = 0; i < particle.size(); i++)
		genericChecks(particle[i]);

	for (int i = 0; i < polygon.size(); i++)
		genericChecks(polygon[i]);

	if (countTick >= 100)
	{
		countTick = 0;
		COORD co;
		co.X = 0;
		co.Y = 5;
		SetConsoleCursorPosition(console, co);
		cout << "Particles: " << particle.size();
#ifdef HGRID
		co.Y = 7;
		SetConsoleCursorPosition(console, co);
		cout << "Circles to Split: " << hGrid.numberToSplit << endl;
		cout << "Max No of Splits: " << hGrid.maxItr << endl;
#endif // DEBUG
	}
	countTick++;
	glutPostRedisplay();
}
void SphereDemo::genericChecks(Particle *circle)
{
	float bounce = 0.8;
	float bounceFriction = 0.88;
	float rollFriction = 0.9;

	Vector2 position = circle->getPosition();
	Vector2 velocity = circle->getVelocity();
	float radius = circle->getRadius();

#ifdef GRAVITY
	if (!circle->atRest)
		velocity.y -= 0.086;//3000 * duration;
#endif // GRAVITY

	velocity.y *= MOVE_RESIST;
	velocity.x *= MOVE_RESIST;

	if (position.x > width - radius)
	{
		//right = true;
		velocity.x = -velocity.x * bounce;
		position.x = width - radius;
	}

	else if (position.x < -width + radius)
	{
		//left = true;
		velocity.x = -velocity.x * bounce;
		position.x = -width + radius;
	}

	// Reverse direction when you reach top or bottom edge
	if (position.y > height - radius)
	{
		//top = true;
		velocity.y = -velocity.y * bounce;
		velocity.x = velocity.x * bounceFriction;
		position.y = height - radius;
	}
	else if (position.y < -height + radius)
	{
#ifdef GRAVITY
		//bottom = true;
		if (velocity.y < -0.6)
		{
			//Bounce
			velocity.x = velocity.x * bounceFriction;
			velocity.y = -velocity.y * bounce;
		}
		else
		{
			//Roll
			velocity.x = velocity.x * rollFriction;
			if (abs(velocity.x) < 0.05)
				circle->rest();
			velocity.y = 0;
		}
#else
		//Bounce
		velocity.x = velocity.x * bounceFriction;
		velocity.y = -velocity.y * bounce;
#endif
		position.y = -height + radius;
	}
	circle->setPosition(position.x, position.y);
	circle->setVelocity(velocity.x, velocity.y);
}
Application* getApplication()
{
	return new SphereDemo();
}






























void SphereDemo::onResize()
{
	setOptimalGridSize();
}

//UI stuff
void SphereDemo::moveFunk(float x, float y)
{
	//Convert mouse XY to world XY
	x = ((x / windowWidth) - 0.5) * width * 2;
	y = -((y / windowHeight) - 0.5) * height * 2;
	prevX = nowX;
	prevY = nowY;
	nowX = x;
	nowY = y;
}
void SphereDemo::mouseFunk(int button, int state, float x, float y)
{
	prevB = nowB;
	prevS = nowS;
	nowB = button;
	nowS = state;
	moveFunk(x, y);

	if (button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON)
	{
		if (nowS == GLUT_DOWN)
		{
			for (int i = 0; i < particle.size(); i++)
			{
				if (CPC.testCirclePoint(particle[i], nowX, nowY))
				{
					target = particle[i];
					if (button == GLUT_LEFT_BUTTON)
					{
						massStore = target->getMass();
						target->setMass(100000);
					}
					break;
				}
			}
			for (int i = 0; i < polygon.size(); i++)
			{
				if (CPC.testPolygonPoint(polygon[i], nowX, nowY))
				{
					target = polygon[i];
					if (button == GLUT_LEFT_BUTTON)
					{
						massStore = target->getMass();
						target->setMass(100000);
					}
					break;
				}
			}
		}
		else if (nowS == GLUT_UP)
		{
			if (button == GLUT_RIGHT_BUTTON && target != nullptr)
			{
				Vector2 diff = Vector2(nowX, nowY) - target->getPosition();
				target->setVelocity(diff.x * 0.1, diff.y * 0.1);
			}
			else if (button == GLUT_LEFT_BUTTON && target != nullptr)
			{
				target->setMass(massStore);
			}
			else if (drawingLine)
			{
				drawingLine = false;
				wall.push_back(new Wall(drawingLineStart, Vector2(nowX, nowY), 0.8));
			}
			target = nullptr;
		}
	}
}
void SphereDemo::keyFunk(unsigned char button, float x, float y)
{
	static float scaleFactor = 1.2;
	if (button == '=')
	{
		if (target == nullptr)
		{
			for (int i = 0; i < particle.size(); i++)
			{
				particle[i]->scale(scaleFactor);
			}
		}
		else
		{
			target->scale(scaleFactor);
			massStore = target->getMass();
			target->setMass(100000);
		}
		setOptimalGridSize();
	}
	else if (button == '-')
	{
		if (target == nullptr)
		{
			for (int i = 0; i < particle.size(); i++)
			{
				particle[i]->scale(1 / scaleFactor);
			}
		}
		else
		{
			target->scale(1 / scaleFactor);
			massStore = target->getMass();
			target->setMass(100000);
		}
		setOptimalGridSize();
	}
	else if (button == '.')
	{
		if (target != nullptr)
		{
			target->setRotation(target->getRotation() - 0.1);
		}
	}
	else if (button == ',')
	{
		if (target != nullptr)
		{
			target->setRotation(target->getRotation() + 0.1);
		}
	}
	else if (button == ']')
	{
		widthSeg += 1;
		heightSeg += 1;
		populateGrid();
	}
	else if (button == '[')
	{
		widthSeg -= 1;
		heightSeg -= 1;
		populateGrid();
	}
	else if (button == 't')
		hGrid.maxItr--;
	else if (button == 'y')
		hGrid.maxItr++;
	else if (button == 'g')
		hGrid.numberToSplit--;
	else if (button == 'h')
		hGrid.numberToSplit++;

	else if (button == 'l')
	{
		if (nowB == GLUT_LEFT_BUTTON)
		{
			if (nowS == GLUT_DOWN)
			{
				if (!drawingLine)
				{
					drawingLine = true;
					drawingLineStart.x = nowX;
					drawingLineStart.y = nowY;
				}
			}
		}
	}
	else if (button == 'n')
	{
		if (target == nullptr)
		{
			x = ((x / windowWidth) - 0.5) * width * 2;
			y = -((y / windowHeight) - 0.5) * height * 2;
			prevX = nowX;
			prevY = nowY;
			nowX = x;
			nowY = y;

			Circle *newParticle = new Circle;
			newParticle->setPosition(nowX, nowY);
			newParticle->setVelocity(0, 0);
			newParticle->setCol((rand() % 200) + 55, (rand() % 200) + 55, (rand() % 200) + 55);
			newParticle->setRotation(float((rand() % 100) - 50) / 10);
			newParticle->setRadius((rand() % 15) + 5);

			bool acceptable = true;
			for (int i = 0; i < particle.size(); i++)
			{
				CCCollision test;
				if (test.testCircleCircle(particle[i], newParticle))
				{
					acceptable = false;
				}
			}
			if (acceptable)
			{
				particle.push_back(newParticle);
				newParticle->id = particle.size();
				setOptimalGridSize();

				if (nowB == GLUT_LEFT_BUTTON && nowS == GLUT_DOWN)
					target = newParticle;
			}
			else
				delete newParticle;
		}
	}
	else if (button == 'b')
	{
		if (target == nullptr)
		{
			x = ((x / windowWidth) - 0.5) * width * 2;
			y = -((y / windowHeight) - 0.5) * height * 2;
			prevX = nowX;
			prevY = nowY;
			nowX = x;
			nowY = y;
			
			ConvexPolygon *newParticle = new ConvexPolygon;
			newParticle->setPosition(nowX, nowY);
			newParticle->setVelocity(0, 0);
			newParticle->setCol((rand() % 200) + 55, (rand() % 200) + 55, (rand() % 200) + 55);
			newParticle->setRadius((rand() % 15) + 5);
			newParticle->generateRandomPoints((rand() % 4) + 3);

			polygon.push_back(newParticle);
			newParticle->id = polygon.size();
			setOptimalGridSize();

			if (nowB == GLUT_LEFT_BUTTON && nowS == GLUT_DOWN)
				target = newParticle;
		}
	}
	else if (button == 'i')
	{
		if (target == nullptr)
		{
			float radiusMin = 2;
			float radiusMax = 4;

			radiusMin *= 10;
			radiusMax *= 10;

			float radius = (rand() % int(radiusMax - (1 + radiusMin))) + radiusMin;
			radius /= 10;
			float widthTrack = -width + radius;
			while (widthTrack < width)
			{
				float radius = (rand() % int(radiusMax - (1 + radiusMin))) + radiusMin;
				radius /= 10;
				Circle *newParticle = new Circle;
				newParticle->setPosition(widthTrack, height - (radius * 3) + (((float((rand() % 200) - 100) / 100) * radius)));
				newParticle->setVelocity(0, 0);
				newParticle->setCol((rand() % 200) + 55, (rand() % 200) + 55, (rand() % 200) + 55);
				newParticle->setRotation(float((rand() % 100) - 50) / 10);
				newParticle->setRadius(radius);
				particle.push_back(newParticle);
				newParticle->id = particle.size();
				widthTrack += radius * 2.5f;
			}
			setOptimalGridSize();
		}
	}
}
void SphereDemo::inputUpdate()
{
	static float dragLag = 1;
	if (target != nullptr)
	{
		if (nowB == GLUT_LEFT_BUTTON && nowS == GLUT_DOWN)
		{
			Vector2 diff = Vector2(nowX, nowY) - target->getPosition();
			if (massStore < 100000)
				target->setVelocity(diff.x * dragLag, diff.y * dragLag);
			else
				target->setPosition(nowX, nowY);
		}
	}

}

void SphereDemo::drawGrid()
{
	//Draw static grid
	glLoadIdentity();
	glColor4f(1, 1, 1, 0.1);
	for (int i = 1; i < widthSeg; i++)
	{
		glLineWidth(0.01);
		glBegin(GL_LINES);
		float xLine = (i * (float(width * 2) / widthSeg)) - width;
		glVertex2f(xLine, -height);
		glVertex2f(xLine, height);
		glEnd();
	}
	for (int i = 1; i < heightSeg; i++)
	{
		glLineWidth(0.01);
		glBegin(GL_LINES);
		float yLine = (i * (float(height * 2) / heightSeg)) - height;
		glVertex2f(-width, yLine);
		glVertex2f(width, yLine);
		glEnd();
	}
}
void SphereDemo::drawHGrid()
{
	//Draw hierarchial grid
	glColor4f(1, 1, 1, 0.1);
	for (int i = 0; i < hGrid.closed.size(); i++)
	{
		glLineWidth(0.01);
		glBegin(GL_LINE_LOOP);
		glVertex2f(hGrid.closed[i]->left - width, hGrid.closed[i]->bot - height);
		glVertex2f(hGrid.closed[i]->left - width, hGrid.closed[i]->top - height);
		glVertex2f(hGrid.closed[i]->right - width, hGrid.closed[i]->top - height);
		glVertex2f(hGrid.closed[i]->right - width, hGrid.closed[i]->bot - height);
		glEnd();
	}
}