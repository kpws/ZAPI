#include "color.h"
#include <GL/glut.h>

color::color(void)
{
}

color::color(float nr,float ng,float nb,float na)
{
	r=nr;
	g=ng;
	b=nb;
	a=na;
}
color color::operator+(color ac)
{
	color rc;
	rc.r=r+ac.r;
	rc.g=g+ac.g;
	rc.b=b+ac.b;
	rc.a=a;
	return rc;
}
color color::operator*(float na)
{
	color rc;
	rc.r=r*na;
	rc.g=g*na;
	rc.b=b*na;
	rc.a=a;
	return rc;
}
void color::clamp()
{
	r=r<0?0:r>1.0f?1.0f:r;
	g=g<0?0:g>1.0f?1.0f:g;
	b=b<0?0:b>1.0f?1.0f:b;
	a=a<0?0:a>1.0f?1.0f:a;
}
void color::set()
{
	glColor4f(r,g,b,a);
}