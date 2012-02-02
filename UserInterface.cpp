/*
 * UserInterface.cpp
 *
 *  Created on: Jul 14, 2009
 *      Author: petter
 */

#include <GL/glut.h>

#include "UserInterface.h"
#include "Game.h"

UserInterface::UserInterface(Game *g)
{
	game=g;
}

UserInterface::~UserInterface()
{
	// TODO Auto-generated destructor stub
}

void UserInterface::setOrthoView()
{
	float w=game->getResX();
	float h=game->getResY();
	/*float ratio =  w / h;
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
			  0.0f,1.0f,0.0f);*/

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluOrtho2D(0, 1.0f, 0, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
