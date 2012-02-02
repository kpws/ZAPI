/*
 * Game.h
 *
 *  Created on: Jul 6, 2009
 *      Author: petter
 */

#ifndef GAME_H_
#define GAME_H_


#include <iostream>
#include <time.h>

#include "UserInterface.h"
#include "GLUTInterface.h"

using namespace std;

class UserInterface;

class Game
{
public:
	Game();
	virtual ~Game();
	void play(int argc, char **argv);
	virtual void UIEvent(string s)=0;
	bool isKeyDown(char key);
	bool isLeftDown();
	bool isMiddleDown();
	bool isRightDown();
	int getResX();
	int getResY();
	int getMouseX();
	int getMouseY();
	void centerMouse(bool center);
	bool isDebug();
	float getDT();
protected:
	void setUI(UserInterface* UI);
	UserInterface* getUI();
	virtual void init()=0;
	void setGoalFps(float fps);
private:
	UserInterface* currentUI;
	bool keys[256],centeringMouse,leftDown,middleDown,rightDown,debugMode;
	friend class GLUTInterface;
	int width,height,mouseX,mouseY;
	clock_t lastTime;
	float dt,goalFps;
	void checkEnv();
	void renderScene();
	void processMouse(int button, int state, int x, int y);
	void processMouseMotion(int x, int y);
	void processMouseEntry(int state);
	void processNormalKeys(unsigned char key, int x, int y);
	void processSpecialKeys(int key, int x, int y) ;
	void pressKey(int key, int x, int y);
	void releaseKey(int key, int x, int y);
	void changeSize(int w, int h);
};

#endif /* GAME_H_ */
