/*
 * Menu.h
 *
 *  Created on: Jul 14, 2009
 *      Author: petter
 */

#ifndef MENU_H_
#define MENU_H_

#include <vector>
#include <string>

#include "UserInterface.h"
#include "font.h"

using namespace std;

class MenuEventReciever
{
public:
	virtual void menuEvent(string event)=0;
};

class Menu: public UserInterface
{
public:
	enum itemType{IT_SUB,IT_BUTTON};
	Menu(Game* g,MenuEventReciever *r, string name);
	~Menu();
	void load();
	void init();
	void draw();
	void keyPress(char key);
	void mouseClick(int button, int state);
	void mouseMove(int x,int y);
	void addItem(string s,itemType it);
private:
	class MenuPage;
	class MenuItem
	{
	public:
		MenuItem(string name);
		virtual ~MenuItem();
		string getName();
		string getPath();
		void setParent(MenuPage *parent)
	    {
	        this->parent = parent;
	    }
		MenuPage *getParent() const
	    {
	        return parent;
	    }
	private:
		string name;
	protected:
		MenuPage* parent;
	};
	class StringChooser: public MenuItem
	{
	public:
		StringChooser(string name);
	};
	class Button: public MenuItem
	{
	public:
		Button(string name);
	};
	class MenuPage: public MenuItem
	{
	public:
		MenuPage(string name);
		void addItem(MenuItem* mi,string path);
		void up();
		void down();
		void draw(Font* f);
	    MenuItem *getSelection();
	private:
		vector<MenuItem*> items;
		int selection;
	};

	MenuEventReciever* rec;
	MenuPage root;
	MenuPage* currentPage;
	Font *font;
};
#endif /* MENU_H_ */
