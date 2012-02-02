/*
 * Game.cpp
 *
 *  Created on: Jul 6, 2009
 *      Author: petter
 */
#include <GL/glew.h>
#include <GL/glut.h>

#include "Game.h"
#include "utilities.h"

Game::Game()
{
	for(int i=0;i<256;i++)
		keys[i]=false;
	leftDown=false;
	middleDown=false;
	rightDown=false;
	debugMode=false;
	goalFps=60.0f;
	dt=1.0f/goalFps;
}
Game::~Game()
{
}
void Game::setUI(UserInterface* UI)
{
	currentUI=UI;
	currentUI->init();
}
UserInterface* Game::getUI()
{
	return currentUI;
}
void Game::play(int argc, char **argv)
{
	width=1333;//1680;
	height=768;//1050;

	centeringMouse=false;

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	string gameModeString=int2string(width)+"x"+int2string(height)+":32";
	glutGameModeString(gameModeString.c_str());

	/*glutCreateWindow("Flowit!");*/

	if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
		glutEnterGameMode();
	else
	{
		cout << "The select mode is not available" << endl;
		exit(1);
	}

	checkEnv();
	glutIgnoreKeyRepeat(1);

	GLUTInterface::setReciever(this);

	glutSetCursor(GLUT_CURSOR_NONE);
	//cout << glutGet(GLUT_WINDOW_STENCIL_SIZE);
	init();
	lastTime=clock();
	glutMainLoop();

}
void Game::checkEnv()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
		cout << "Error: " << glewGetErrorString(err) << endl;

	if (!glewIsSupported("GL_VERSION_2_0"))
	{
		cerr << "OpenGL 2.0 not supported\n";
		exit(1);
	}
	else
	{
		cout << "OpenGL 2.0 is supported!!! Great!!!" << endl;
	}
}
void Game::renderScene(void)
{
	currentUI->draw();

	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) {
	    errString = gluErrorString(errCode);
	   cerr << "OpenGL Error: "<< errString<<endl;
	}




	if(centeringMouse)
		glutWarpPointer(width/2,height/2);
	//int n=1;
	//dt=(dt*float(n-1)+float(newTime-lastTime)/float(CLOCKS_PER_SEC))/float(n);
	double elapsed=float(clock()-lastTime)/float(CLOCKS_PER_SEC);
	if(elapsed<1.0/goalFps)
	{
		usleep((1.0/goalFps-elapsed)*1000000.0);
		//cout << "asd: "<< (1.0f/120.0f-dt)*1000000.0f<<endl;
	}

	glutSwapBuffers();
	lastTime=clock();
}
void Game::processMouse(int button, int state, int x, int y)
{
	if(state==GLUT_DOWN)
	{
		if(button==GLUT_LEFT_BUTTON)
			leftDown=true;
		else if(button==GLUT_MIDDLE_BUTTON)
			middleDown=true;
		else if(button==GLUT_RIGHT_BUTTON)
			rightDown=true;
	}
	else if(state==GLUT_UP)
	{
		if(button==GLUT_LEFT_BUTTON)
			leftDown=false;
		else if(button==GLUT_MIDDLE_BUTTON)
			middleDown=false;
		else if(button==GLUT_RIGHT_BUTTON)
			rightDown=false;
	}
	currentUI->mouseClick(button,state);
}
void Game::processMouseMotion(int x, int y)
{
	mouseX=x;
	mouseY=y;
	if(centeringMouse)
		currentUI->mouseMove(x-width/2,y-height/2);
	else
		currentUI->mouseMove(x,y);
}
void Game::processMouseEntry(int state)
{}
void Game::processNormalKeys(unsigned char key, int x, int y)
{
	if(key>=0 && key<256)
		keys[key]=true;
	currentUI->keyPress(key);
	if(key=='n')
		debugMode=!debugMode;
}
void Game::processSpecialKeys(int key, int x, int y)
{
	if(key>=0 && key<256)
		keys[key]=true;
	currentUI->keyPress(key);
}
void Game::pressKey(int key, int x, int y)
{
	if(key>=0 && key<256)
		keys[key]=true;
	currentUI->keyPress(key);
	if(key=='n')
		debugMode=!debugMode;
}
void Game::releaseKey(int key, int x, int y)
{
	if(key>=0 && key<256)
		keys[key]=false;
}
void Game::changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,5.0,
		      0.0,0.0,-1.0,
			  0.0f,1.0f,0.0f);
}
bool Game::isKeyDown(char key)
{
	if(key>=0 && key<256)
		return keys[key];
	else
		return false;
}
bool Game::isLeftDown()
{
	return leftDown;
}
bool Game::isMiddleDown()
{
	return middleDown;
}
bool Game::isRightDown()
{
	return rightDown;
}
int Game::getResX()
{
	return width;
}
int Game::getResY()
{
	return height;
}
int Game::getMouseX()
{
	return mouseX;
}
int Game::getMouseY()
{
	return mouseY;
}
float Game::getDT()
{
	return dt;
}
void Game::centerMouse(bool center)
{
	centeringMouse=center;
	if(center)
		glutWarpPointer(width/2,height/2);
}
bool Game::isDebug()
{
	return debugMode;
}
void Game::setGoalFps(float fps)
{
	goalFps=fps;
	dt=1.0f/goalFps;
}
