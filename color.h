#pragma once

class color
{
public:
	color(void);
	color(float nr,float ng,float b,float na);
	color operator+(color a);
	color operator*(float a);
	void clamp();
	void set();
	float r,g,b,a;
};
