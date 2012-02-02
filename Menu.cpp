/*
 * Menu.cpp
 *
 *  Created on: Jul 14, 2009
 *      Author: petter
 */

#include <iostream>
#include <GL/glut.h>

#include "Menu.h"

using namespace std;

Menu::Menu(Game* g,MenuEventReciever *r,string name):UserInterface(g),root(name)
{
	rec=r;
	currentPage=&root;
}
Menu::~Menu()
{
	// TODO Auto-generated destructor stub
}
void Menu::load()
{
	font=FontGroup::getSingleton()->add("font.png",0.03,0.02,-0.01);
}
void Menu::init()
{
	game->centerMouse(false);

	setOrthoView();


	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_DEPTH_TEST);
}
void Menu::draw()
{
	setOrthoView();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);
	TexGroup::getSingleton()->disableallmultitex();
	currentPage->draw(font);
}
void Menu::keyPress(char key)
{
	if(key==GLUT_KEY_UP)
		currentPage->up();

	if(key==GLUT_KEY_DOWN)
		currentPage->down();

	if(key==13)
	{
		MenuPage* cp=dynamic_cast<MenuPage*>(currentPage->getSelection());
		if(cp!=NULL)
			currentPage=cp;
		else
		{
			if(currentPage->getSelection()!=NULL)
				rec->menuEvent(currentPage->getSelection()->getPath());
		}
	}
	if(key==27)
	{
		if(currentPage!=&root)
			currentPage=currentPage->getParent();
	}
}
void Menu::mouseClick(int button, int state)
{

}
void Menu::mouseMove(int x,int y)
{

}
void Menu::addItem(string s,itemType it)
{
	string name,path;
	size_t lp=s.find_last_of('/');
	if(lp==string::npos)
	{
		name=s;
		path="";
	}
	else
	{
		name=s.substr(lp+1,s.size()-lp-1);
		path=s.substr(0,lp);
	}
	//cout <<path <<endl;
	//cout <<name <<endl;

	MenuItem* mi;
	switch(it)
	{
	case IT_SUB:
		mi=new MenuPage(name);
		break;
	case IT_BUTTON:
		mi=new Button(name);
		break;
	}
	root.addItem(mi,path);
}


Menu::MenuItem::MenuItem(string name)
{
	this->name=name;
}
Menu::MenuItem::~MenuItem()
{}
string Menu::MenuItem::getName()
{
	return name;
}
string Menu::MenuItem::getPath()
{
	if(parent==NULL)
		return name;
	else
		return parent->getPath()+"/"+name;
}

Menu::MenuPage::MenuPage(string name):MenuItem(name)
{
	selection=0;
	parent=NULL;
}
void Menu::MenuPage::addItem(MenuItem* mi,string path)
{//cout <<"::::"<<path<< endl;
	string name,rempath;
	size_t lp=path.find_last_of('/');
	if(lp==string::npos)
	{
		items.push_back(mi);
		//MenuPage *mp=dynamic_cast<MenuPage *>(mi);

		mi->setParent(this);
	}
	else
	{
		name=path.substr(lp+1,path.size()-lp-1);
		rempath=path.substr(0,lp);
		for(size_t i=0;i<items.size();i++)
		{
			if(items[i]->getName()==name)
			{
				MenuPage *mp=dynamic_cast<MenuPage *>(items[i]);
				if(mp==NULL)
					cerr <<"Invalid path name, doesnt point to MenuPage: "<< path << endl;
				else
					mp->addItem(mi,rempath);
				return;
			}
		}
		cerr <<"Invalid menu path, couldn't find path: "<< path << endl;
	}
}
void Menu::MenuPage::up()
{
	if(!items.empty())
	{
		selection--;
		selection=(selection+items.size())%items.size();
	}
}
void Menu::MenuPage::down()
{
	if(!items.empty())
	{
		selection++;
		selection=selection%items.size();
	}
}
void Menu::MenuPage::draw(Font* f)
{
	for(size_t i=0;i<items.size();i++)
	{
		if(selection==i)
			glColor4f(0.0f,1.0f,0.5f,1.0f);
		else
			glColor4f(0.0f,0.5f,1.0f,1.0f);
		f->print(0.5,0.5-float(i)*0.03,items[i]->getName());
	}
}
Menu::MenuItem* Menu::MenuPage::getSelection()
{
	if(items.empty())
		return NULL;
	else
		return items[selection];
}

Menu::Button::Button(string name):MenuItem(name)
{
}
