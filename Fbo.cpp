/*
 * Fbo.cpp
 *
 *  Created on: Oct 25, 2009
 *      Author: petter
 */
#include <iostream>

#include "Fbo.h"

Fbo::Fbo()
{
	// TODO Auto-generated constructor stub

}

Fbo::~Fbo()
{
	// TODO Auto-generated destructor stub
}

void Fbo::create(int width,int height)
{
	w=width;
	h=height;
	glGenFramebuffersEXT(1, &fbo);
}

void Fbo::attachTex(int tex,int num)
{

	if(w!=TexGroup::getSingleton()->getWidth(tex) || h!=TexGroup::getSingleton()->getHeight(tex))
		cerr << "FBO error, trying to attach texture of mismatching dimensions." << endl;
	else
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+num, GL_TEXTURE_2D,TexGroup::getSingleton()->getid(tex), 0);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
}

void Fbo::attachDepthBuffer()
{
	GLuint depthbuffer;
	glGenRenderbuffersEXT(1, &depthbuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthbuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void Fbo::bind()const
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,w, h);
}
void Fbo::unbind()
{
	glPopAttrib();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
