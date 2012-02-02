/*
 * utilities.h
 *
 *  Created on: Jul 15, 2009
 *      Author: petter
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include "Vector.h"
#include <climits>

using namespace std;

float sqr(float a);
float randf();
float randf(float a);
float randi(int a);

int rD(float f);

float interpC0(float a,float b,float x);

float interpC1(float a,float b,float x);

template<int D,int SEED,int C>
class _Noise
{
public:
	static float noise(Vector<D> p)
	{
		int seed=0;
		for(int i=0;i<D;i++)
		{
			int n=i+10;
			seed+=(n*n+n+41)*rD(p.x[i]);
		}

		if(seed<0)
			srand(((unsigned int)(-seed))+UINT_MAX/2+SEED);
		else
			srand((unsigned int)seed+SEED);
		return randf();
	}

	static float noiseC0(Vector<D> p)
	{
		float f=p.x[C-1];
		int prev=rD(f);
		Vector<D> p1=p,p2=p;
		p1.x[C-1]=float(prev);
		p2.x[C-1]=float(prev+1);
		return interpC0(_Noise<D,SEED,C-1>::noiseC0(p1),_Noise<D,SEED,C-1>::noiseC0(p2),f-float(prev));
	}

	static float noiseC1(Vector<D> p)
	{
		float f=p.x[C-1];
		int prev=rD(f);
		Vector<D> p1=p,p2=p;
		p1.x[C-1]=float(prev);
		p2.x[C-1]=float(prev+1);
		return interpC1(_Noise<D,SEED,C-1>::noiseC1(p1),_Noise<D,SEED,C-1>::noiseC1(p2),f-float(prev));
	}
};

template<int D,int SEED>
class _Noise<D,SEED,0>
{
public:
	static float noise(Vector<D> p)
	{
		int seed=0;
		for(int i=0;i<D;i++)
		{
			int n=i+10;
			seed+=(n*n+n+41)*rD(p.x[i]);
		}

		if(seed<0)
			srand(((unsigned int)(-seed))+UINT_MAX/2+SEED);
		else
			srand((unsigned int)seed+SEED);
		return randf();
	}

	static float noiseC0(Vector<D> p)
	{
		return noise(p);
	}

	static float noiseC1(Vector<D> p)
	{
		return noise(p);
	}
};

template<int D,int SEED>
float noise(Vector<D> p)
{
	return _Noise<D,SEED,D>::noise(p);
}

template<int D,int SEED>
float noiseC0(Vector<D> p)
{
	return _Noise<D,SEED,D>::noiseC0(p);
}

template<int D,int SEED>
float noiseC1(Vector<D> p)
{
	return _Noise<D,SEED,D>::noiseC1(p);
}

template<int D>
class NoiseHyperCube
{
public:
	NoiseHyperCube()
	{
		hyperPlanes=NULL;
	};
	NoiseHyperCube(int size)
	{
		n=size;
		hyperPlanes=new NoiseHyperCube<D-1>[n];
		for(int i=0;i<n;i++)
			hyperPlanes[i]=NoiseHyperCube<D-1>(n);
	}
	float getVal(const Vector<D> &p)const
	{
		int Dp=rD(p.x[D-1]*float(n))%n;
		return hyperPlanes[Dp].getVal(p.x);
	}
	float getValC0(const Vector<D> &p)const
	{
		int Dp=rD(p.x[D-1]*float(n))%n;

		return interpC0(hyperPlanes[Dp].getValC0(p.x),hyperPlanes[(Dp+1)%n].getValC0(p.x),     p.x[D-1]*float(n)-float(rD(p.x[D-1]*float(n)))      );
	}
	float getValC1(const Vector<D> &p)const
	{
		int Dp=rD(p.x[D-1]*float(n));
		if(Dp>=0)
			Dp=Dp%n;
		else
			Dp=(n-(-Dp)%n)%n;
		if(Dp<0 || Dp>=n || (Dp+1)%n<0 || (Dp+1)%n>=n)
		{
			cout << "Shit: "<<Dp<<","<<n<<","<<rD(p.x[D-1]*float(n))<<endl;
		}
		return interpC1(hyperPlanes[Dp].getValC1(p.x),hyperPlanes[(Dp+1)%n].getValC1(p.x),     p.x[D-1]*float(n)-float(rD(p.x[D-1]*float(n)))      );
	}
private:
	int n;
	NoiseHyperCube<D-1>* hyperPlanes;
};

template<>
class NoiseHyperCube<1>
{
public:
	NoiseHyperCube()
	{
		values=NULL;
	};
	NoiseHyperCube(int size)
	{
		n=size;
		values=new float[n];
		for(int i=0;i<n;i++)
		{
			values[i]=randf();
		}
	}
	float getVal(const Vector<1> &p)const
	{
		int Dp=rD(p.x[0]*float(n))%n;
		return values[Dp];
	}
	float getValC0(const Vector<1> &p)const
	{
		int Dp=rD(p.x[0]*float(n))%n;

		return interpC0(values[Dp],values[(Dp+1)%n],   p.x[0]*float(n)  -  float(rD(p.x[0]*float(n)))   );
	}
	float getValC1(const Vector<1> &p)const
	{
		int Dp=rD(p.x[0]*float(n))%n;

		return interpC1(values[Dp],values[(Dp+1)%n],   p.x[0]*float(n)  -  float(rD(p.x[0]*float(n)))   );
	}
private:
	int n;
	float *values;
};

string int2string(int i);

#endif /* UTILITIES_H_ */
