/*
 * PotVisPlane.cpp
 *
 *  Created on: Sep 16, 2010
 *      Author: petter
 */

#include "PotVisPlane.h"

PotVisPlane::PotVisPlane(Vector3D planeP,Vector3D normal) {
	p=planeP;
	n=normal;
}
PotVisPlane::PotVisPlane(Vector3D normal,float dist) {
	n=normal;
	p=normal*dist;
}
PotVisPlane::~PotVisPlane() {
	// TODO Auto-generated destructor stub
}

bool PotVisPlane::Visible(Vector3D point)
{
	return dot(point-p,n)>=0.0f;
}

bool PotVisPlane::VisibleCube(Vector3D p,float s)
{
	if(Visible(p+Vector3D(-s,-s,-s)))
		return true;
	if(Visible(p+Vector3D(-s,-s,s)))
		return true;
	if(Visible(p+Vector3D(-s,s,-s)))
		return true;
	if(Visible(p+Vector3D(-s,s,s)))
		return true;
	if(Visible(p+Vector3D(s,-s,-s)))
		return true;
	if(Visible(p+Vector3D(s,-s,s)))
		return true;
	if(Visible(p+Vector3D(s,s,-s)))
		return true;
	if(Visible(p+Vector3D(s,s,s)))
		return true;
	return false;
}
