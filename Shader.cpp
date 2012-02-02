/*
 * Shader.cpp
 *
 *  Created on: Jul 31, 2009
 *      Author: petter
 */

#include "Shader.h"

#include <iostream>
#include <fstream>

using namespace std;

Shader::Shader()
{
	vert=0;
	frag=0;
	prog=0;
}

Shader::~Shader()
{
	clean();
}
void Shader::clean()
{
	if(vert!=0)
		glDeleteShader(vert);
	if(frag!=0)
		glDeleteShader(frag);
	if(prog!=0)
		glDeleteProgram(prog);
}
void Shader::load(string fragfilename,string vertfilename)
{
	cout << "Loading shader "<< fragfilename<<", " <<vertfilename <<"... "<<flush;
	ifstream fragfile(string("data/shaders/fragment/"+fragfilename).c_str(), ifstream::in);
	if(!fragfile)
	{
		cout << endl;
		clean();
		cerr<<"File "<< fragfilename << " not found."<<endl;
		return;
	}
	string fragfileContents;
    while (! fragfile.eof() )
    {
    	string line;
    	getline (fragfile,line);
    	fragfileContents+=line;
    }
	fragfile.close();

	ifstream vertfile(string("data/shaders/vertex/"+vertfilename).c_str(), ifstream::in);
	if(!vertfile)
	{
		cout << endl;
		clean();
		cerr<<"File "<< vertfilename << " not found."<<endl;
		return;
	}
	string vertfileContents;
    while (! vertfile.eof() )
    {
    	string line;
    	getline (vertfile,line);
    	vertfileContents+=line;
    }
	vertfile.close();

	vert=glCreateShader(GL_VERTEX_SHADER);
	if(vert==0)
	{
		cout << endl;
		cerr<<"Error: Unable to create vertex shader"<<endl;
		clean();
		return;
	}
	frag=glCreateShader(GL_FRAGMENT_SHADER);
	if(frag==0)
	{
		cout << endl;
		cerr<<"Error: Unable to create fragment shader"<<endl;
		clean();
		return;
	}

	const char* charray;
	charray=vertfileContents.c_str();
	glShaderSource(vert,1,&charray,NULL);
	charray=fragfileContents.c_str();
	glShaderSource(frag,1,&charray,NULL);

	int res;
	glCompileShader(vert);
	glGetShaderiv(vert,GL_COMPILE_STATUS,&res);
	if(res==GL_FALSE)
	{
		cout << endl;
		cerr << "Vertex shader compilation error: "<< vertfilename<<endl;
		int len;
		glGetShaderiv(vert,GL_INFO_LOG_LENGTH,&len);
		if(len>0)
		{
			char *log= new char[len];
			int realLen;
			glGetShaderInfoLog(vert, len, &realLen,log);
			cerr << log;
			delete [] log;
		}
		else
		{
			cerr << "Shader error withoiut message.";
		}
		clean();
		return;
	}
	glCompileShader(frag);
	glGetShaderiv(frag,GL_COMPILE_STATUS,&res);
	if(res==GL_FALSE)
	{
		cout << endl;
		cerr << "Fragment shader compilation error: "<< fragfilename<<endl;
		int len;
		glGetShaderiv(frag,GL_INFO_LOG_LENGTH,&len);
		if(len>0)
		{
			char *log= new char[len];
			int realLen;
			glGetShaderInfoLog(frag, len, &realLen,log);
			cerr << log;
			delete [] log;
		}
		else
		{
			cerr << "Shader error withoiut message.";
		}

		clean();
		return;
	}

	prog=glCreateProgram();

	glAttachShader(prog,vert);
	glAttachShader(prog,frag);

	glLinkProgram(prog);
	glGetProgramiv(prog,GL_LINK_STATUS,&res);
	if(res==GL_FALSE)
	{
		bool error=false;
		int len;
		glGetProgramiv(prog,GL_INFO_LOG_LENGTH,&len);

		if(len>0)
		{

			char *log= new char[len];
			int realLen;
			glGetProgramInfoLog(prog, len, &realLen,log);
			if(string(log).compare("Fragment shader(s) linked, vertex shader(s) linked. ") && false)
			{
				cout << endl;
				cerr << endl << "Shader linkage error: "<< fragfilename << ", "<< vertfilename<<endl;
				error=true;
				cerr << log;
			}
			delete [] log;
		}
		else
		{
			cerr << "Shader error withoiut message, ignoring.";
		}

		if(error)
		{
			clean();
			return;
		}
	}
	cout << "done"<< endl;
}
void Shader::enable()
{
	if(prog!=0)
		glUseProgram(prog);
}

GLint Shader::getLocation(string name)
{
	return glGetUniformLocation(prog,name.c_str());
}
void Shader::sendTransformToWorldMatrix()
{
	static GLint location=-1;
	if(location==-1)
		location=glGetUniformLocation(prog,"transformToWorldMatrix");

	//glUniformMatrix4fv(location, GLsizei count, GLboolean transpose, GLfloat *v);
}
void Shader::sendFloat(GLint location,float a)
{
	glUniform1f(location,a);
}
void Shader::send2Floats(GLint location,float a,float b)
{
	glUniform2f(location,a,b);
}
void Shader::send3Floats(GLint location,float a,float b,float c)
{
	glUniform3f(location,a,b,c);
}
void Shader::sendInt(GLint location,int a)
{
	glUniform1i(location,a);
}
