#pragma once

class Application
{
protected:
	int height;
	int width;
	int windowHeight;
	int windowWidth;
	//time interval for the timer
	float timeinterval;
	//Mouse variables
	int prevB = -1, prevS = -1;
	float prevX = 0, prevY = 0;
	int nowB = -1, nowS = -1;
	float nowX = 0, nowY = 0;
	virtual void onResize() {};
public:
    virtual void initGraphics();
    virtual void display();
	virtual void update();
	virtual void mouseFunk(int button, int state, float x, float y) {}
	virtual void moveFunk(float x, float y) {}
	virtual void keyFunk(unsigned char button, float x, float y) {}
    virtual void resize(int width, int height);
	virtual int getWidth();
	virtual int getHeight();
	float getTimeinterval();
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	void setTimeinterval(float timeint);
 };



    

