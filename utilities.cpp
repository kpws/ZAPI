/*
 * utilities.cpp
 *
 *  Created on: Jul 15, 2009
 *      Author: petter
 */

#include "utilities.h"

float sqr(float a)
{
        return a*a;
}
float randf()
{
        return(1.0f-float(rand())/float(RAND_MAX)*2.0f);
}
float randf(float a)
{
        return(float(rand())/float(RAND_MAX)*a);
}
float randi(int a)
{
        return rand()%(a+1);
}


string int2string(int i)
{
        stringstream ss;
        string str;
        ss << i;
        ss >> str;
        return str;
}

int rD(float f)
{
	int i=f;
	if(f<0)i--;
	return i;
}

float interpC0(float a,float b,float x)
{
	return a*(1.0f-x)+b*x;
}

float interpC1(float a,float b,float x)
{
	float cosx=(cos(x*M_PI)+1.0f)/2.0f;
	return a*cosx+b*(1.0f-cosx);
}
