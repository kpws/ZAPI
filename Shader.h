/*
 * Shader.h
 *
 *  Created on: Jul 31, 2009
 *      Author: petter
 */

#ifndef SHADER_H_
#define SHADER_H_


#include <string>
#include <GL/gl.h>
#include <GL/glext.h>

using namespace std;

class Shader
{
public:
	Shader();
	void load(string frag,string vert);
	virtual ~Shader();
	void enable();
	static void disable()
	{
		glUseProgram(0);
	};
	GLint getLocation(string name);
	void sendTransformToWorldMatrix();
	void sendFloat(GLint location,float a);
	void send2Floats(GLint location,float a,float b);
	void send3Floats(GLint location,float a,float b,float c);
	void sendInt(GLint location,int a);
private:
	GLuint vert,frag,prog;
	void clean();
};

#endif /* SHADER_H_ */
