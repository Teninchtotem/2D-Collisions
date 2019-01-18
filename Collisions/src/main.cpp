#include <gl/glut.h>

#include "app.h"
extern Application* getApplication();
Application* app;
#include <chrono> 
#include <ctime>
#include <iostream>

using namespace std;
int spfTick = 0;
float spfStore = 0;

void display(void)
{
	app->display();
}


void createWindow(const char* title)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow(title);

}
void MouseFunc(int button, int state, int x, int y)
{
	app->mouseFunk(button, state, x, y);
}
void MoveFunc(int x, int y)
{
	app->moveFunk(x, y);
}
void KeyFunc(unsigned char key, int x, int y)
{
	app->keyFunk(key, x, y);
}
void TimerFunc(int value)
{
	app->update();
	float  timeinterval = app->getTimeinterval();
	glutTimerFunc(timeinterval, TimerFunc, 1);
	//TimerFunc(0);
}
void IdleFunc()
{
	static std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	static std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	if (elapsed_seconds.count() * 1000 > app->getTimeinterval())
	{
		spfTick++;
		if (spfTick > 100)
		{
			COORD co;
			co.X = 0;
			co.Y = 0;
			SetConsoleCursorPosition(app->console, co);
			std::cout << "Avg SPF: " << spfStore/100 << std::endl;

			spfTick = 0;
			spfStore = 0;
		}
		spfStore += elapsed_seconds.count();
		start = std::chrono::system_clock::now();
		app->update();
	}


	/*float  timeinterval = app->getTimeinterval();
	glutTimerFunc(timeinterval, TimerFunc, 1); */

	//TimerFunc(0);
}


void resize(int width, int height)
{
	app->resize(width, height);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	app = getApplication();
	float  timeinterval = 11;	//Aprox 30fps
	app->setTimeinterval(timeinterval);
	createWindow("Sphere");
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(IdleFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MoveFunc);
	glutKeyboardFunc(KeyFunc);
	//glutTimerFunc(timeinterval, TimerFunc, 1);
	app->initGraphics();
	glutMainLoop();
	delete app;
	return 0;
}