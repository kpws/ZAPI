/*
 * GLUTInterface.cpp
 *
 *  Created on: Jul 16, 2009
 *      Author: petter
 */
#include <GL/glew.h>
#include <GL/glut.h>

#include "GLUTInterface.h"
#include "Game.h"

Game* GLUTInterface::game = NULL;

GLUTInterface::GLUTInterface()
{
	// TODO Auto-generated constructor stub

}

GLUTInterface::~GLUTInterface()
{
	// TODO Auto-generated destructor stub
}

void GLUTInterface::setReciever(Game* g)
{
	game=g;
	glutDisplayFunc(staticRenderScene);
	glutIdleFunc(staticIdle);
	glutKeyboardFunc(staticProcessNormalKeys);
	glutSpecialFunc(staticPressKey);
	glutSpecialUpFunc(staticReleaseKeySpec);
	glutKeyboardUpFunc(staticReleaseKey);
	glutIdleFunc(staticRenderScene);
	glutReshapeFunc(staticChangeSize);
	glutMouseFunc(staticProcessMouse);
	glutMotionFunc(staticProcessMouseActiveMotion);
	glutPassiveMotionFunc(staticProcessMousePassiveMotion);
}
void GLUTInterface::staticRenderScene()
{
	game->renderScene();
}
void GLUTInterface::staticIdle()
{
	glutPostRedisplay();
}
void GLUTInterface::staticProcessMouse(int button, int state, int x,
		int y)
{
	game->processMouse(button, state, x, y);
}
void GLUTInterface::staticProcessMouseActiveMotion(int x, int y)
{
	game->processMouseMotion(x, y);
}
void GLUTInterface::staticProcessMousePassiveMotion(int x, int y)
{
	game->processMouseMotion(x, y);
}
void GLUTInterface::staticProcessMouseEntry(int state)
{
	game->processMouseEntry(state);
}
void GLUTInterface::staticProcessNormalKeys(unsigned char key, int x,
		int y)
{
	game->processNormalKeys(key, x, y);
}
void GLUTInterface::staticProcessSpecialKeys(int key, int x, int y)
{
	game->processSpecialKeys(key, x, y);
}
void GLUTInterface::staticPressKey(int key, int x, int y)
{
	game->pressKey(key, x, y);
}
void GLUTInterface::staticReleaseKeySpec(int key, int x, int y)
{
	game->releaseKey(key, x, y);
}
void GLUTInterface::staticReleaseKey(unsigned char key, int x, int y)
{
	game->releaseKey(key, x, y);
}
void GLUTInterface::staticChangeSize(int w, int h)
{
	game->changeSize(w, h);
}
