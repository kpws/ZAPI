/*
 * Vector.h
 *
 *  Created on: Jul 27, 2009
 *      Author: petter
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#define _USE_MATH_DEFINES
#include <math.h>

template<int D>
class Vector
{
public:
	Vector()
	{}
	Vector(float a)
	{
		x[0]=a;
	}
	Vector(float a,float b)
	{
		x[0]=a;
		x[1]=b;
	}
	Vector(float a,float b,float c)
	{
		x[0]=a;
		x[1]=b;
		x[2]=c;
	}
	Vector(float a,float b,float c,float d)
	{
		x[0]=a;
		x[1]=b;
		x[2]=c;
		x[3]=d;
	}
	Vector(float a,float b,float c,float d,float e)
	{
		x[0]=a;
		x[1]=b;
		x[2]=c;
		x[3]=d;
		x[4]=e;
	}
	Vector(const float *v)
	{
		for(int i=0;i<D;i++)
			x[i]=v[i];
	}
	template<int E>
	Vector(const Vector<E>& o)
	{
		if(D>E)
		{
			for(int i=0;i<E;i++)
				x[i]=o.x[i];
			for(int i=E;i<D;i++)
				x[i]=0.0f;
		}
		else
			for(int i=0;i<D;i++)
				x[i]=o.x[i];
	}
	template<int E>
	void operator=(const Vector<E>& o)
	{
		if(D>E)
		{
			for(int i=0;i<E;i++)
				x[i]=o.x[i];
			for(int i=E;i<D;i++)
				x[i]=0.0f;
		}
		else
			for(int i=0;i<D;i++)
				x[i]=o.x[i];
	}
	Vector<D> operator+(const Vector<D>& o)const
	{
		Vector<D> r;
		for(int i=0;i<D;i++)
			r.x[i]=x[i]+o.x[i];
		return r;
	}
	Vector<D> operator-(const Vector<D>& o)const
	{
		Vector<D> r;
		for(int i=0;i<D;i++)
			r.x[i]=x[i]-o.x[i];
		return r;
	}
	Vector<D> operator-()const
	{
		Vector<D> r;
		for(int i=0;i<D;i++)
			r.x[i]=-x[i];
		return r;
	}
	void operator+=(const Vector<D>& o)
	{
		*this=*this+o;
	}
	void operator-=(const Vector<D>& o)
	{
		*this=*this-o;
	}
	Vector<D> operator*(const float& f)const
	{
		Vector<D> r;
		for(int i=0;i<D;i++)
			r.x[i]=x[i]*f;
		return r;
	}
	Vector<D> operator/(const float& f)const
	{
		Vector<D> r;
		for(int i=0;i<D;i++)
			r.x[i]=x[i]/f;
		return r;
	}
	float sqrNorm()const
	{
		float r=0;
		for(int i=0;i<D;i++)
			r+=x[i]*x[i];
		return r;
	}
	float norm()const
	{
		return sqrt(sqrNorm());
	}
	void normalize()
	{
		*this=unit();
	}
	Vector<D> unit()const
	{
		return *this/norm();
	}
	bool operator==(const Vector<D>& o)const
	{
		for(int i=0;i<D;i++)
			if(x[i]!=o.x[i])//if(  fabs((x[i]-o.x[i])/x[i])>0.001f  )
				return false;
		return true;
	}
	float x[D];
};

typedef Vector<1> Vector1D;
typedef Vector<2> Vector2D;
typedef Vector<3> Vector3D;
typedef Vector<4> Vector4D;
typedef Vector<5> Vector5D;

template<int D>
float dot(const Vector<D> &a,const Vector<D> &b)
{
	float r=0;
	for(int i=0;i<D;i++)
	r+=a.x[i]*b.x[i];
	return r;
}

template<int D>
Vector<D> crossVec(const Vector<D>& a,const Vector<D>& b)
{
	Vector<D> r;
	r.x[0]=a.x[1]*b.x[2]-a.x[2]*b.x[1];
	r.x[1]=a.x[2]*b.x[0]-a.x[0]*b.x[2];
	r.x[2]=a.x[0]*b.x[1]-a.x[1]*b.x[0];
	return r;
}

#endif /* VECTOR_H_ */
