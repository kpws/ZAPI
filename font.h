#pragma once

class Font;

#include <string>
#include <map>

#include "TexGroup.h"

using namespace std;

class Font
{
public:
	int tex,dl;
	Font(void);
	void build(string fn,float height,float width,float sep);
	void print(float x,float y,string text);
};

class FontGroup
{
public:
	Font* add(string name,float height,float width,float sep);
	static FontGroup* getSingleton();
private:
	FontGroup();

	static FontGroup* singleton;
	map<string,Font*> fonts;
};
