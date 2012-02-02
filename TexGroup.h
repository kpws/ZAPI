#pragma once

class Texture;
class TexGroup;

#include <string>
#include <vector>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/glpng.h>
#include <GL/glext.h>
#include "utilities.h"

using namespace std;

class Texture
{
private:
	GLuint id,width,height,bpp,size;
	GLubyte	*data;
	GLuint	type;
	int af;
	bool cubemap,volume;
	void loadTGA(const char *filename);
	void loadPNG(const char *filename);
	void loadPNGcubemap(const char *filename);
public:
	string fn;
	GLuint	edge;
	TexGroup* group;
	Texture();
	void load(string filename,bool samename=false);
	void loadcubemap(string filename,bool samename=false);
	void load(unsigned char *data,int width,int height);
	void load(GLenum internalformat,int width,int height);
	void makeNoise3D(int size,int startRes);//Deprecated
	void makeRNoise3D(int size,int startRes);
	void makeRGBNoise3D(int size,int startRes);
	void bind();
	void bind(int num);
	bool isloaded();
	void setAF(int b);
	void reload();
	GLint getid();
	int getWidth(){return width;}
	int getHeight(){return height;}
};


class TexGroup
{
private:
	vector<Texture> textures;
	TexGroup(void);
	static TexGroup* singleton;
public:
	static TexGroup* getSingleton();
	GLuint edge;
	void reload();
	void setAF(int n);
	int add(string fn);
	int addcubemap(string fn);
	int add(unsigned char *data,int width,int height);
	int add(GLenum internalformat,int width,int height);
	int addNoise3D(int size,int startRes);//Deprecated
	int addRNoise3D(int size,int startRes);
	int addRGBNoise3D(int size,int startRes);
	void bind(int i,int num=0);
	void disableallmultitex(void);
	GLint getid(int i);
	int getWidth(int i);
	int getHeight(int i);
};
