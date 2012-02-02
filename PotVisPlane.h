/*
 * PotVisPlane.h
 *
 *  Created on: Sep 16, 2010
 *      Author: petter
 */

#ifndef POTVISPLANE_H_
#define POTVISPLANE_H_

#include "PotVis.h"

class PotVisPlane:public PotVis
{
public:
	PotVisPlane(Vector3D planeP,Vector3D normal);
	PotVisPlane(Vector3D normal,float dist);
	virtual ~PotVisPlane();
	bool Visible(Vector3D p);
	bool VisibleCube(Vector3D p,float halfSide);
private:
	Vector3D p,n;
};

#endif /* POTVISPLANE_H_ */
