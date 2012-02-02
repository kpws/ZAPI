/*
 * Fbo.h
 *
 *  Created on: Oct 25, 2009
 *      Author: petter
 */

#ifndef FBO_H_
#define FBO_H_


//#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>


#include <GL/glext.h>

#include "TexGroup.h"

class Fbo
{
private:
	GLuint fbo;
	int w,h;
public:
	void create(int width,int height);
	void attachTex(int tex,int num=0);
	void attachDepthBuffer();
	void bind()const;
	static void unbind();

	Fbo();
	virtual ~Fbo();
};

#endif /* FBO_H_ */
