/*
 * UserInterface.h
 *
 *  Created on: Jul 14, 2009
 *      Author: petter
 */

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include "Game.h"

class Game;

class UserInterface
{
public:
	UserInterface(Game *g);
	virtual void load()=0;
	virtual void init()=0;
	virtual void draw()=0;
	virtual void keyPress(char key)=0;
	virtual void mouseClick(int button, int state)=0;
	virtual void mouseMove(int x,int y)=0;
	virtual ~UserInterface();
protected:
	Game* game;
	void setOrthoView();
};

#endif /* USERINTERFACE_H_ */
