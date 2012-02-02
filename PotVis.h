/*
 * PotVis.h
 *
 *  Created on: Sep 16, 2010
 *      Author: petter
 */

#ifndef POTVIS_H_
#define POTVIS_H_

#include "Vector.h"

class PotVis//just the base class
{
public:
	PotVis();
	virtual ~PotVis();
	virtual bool Visible(Vector3D p)=0;
	virtual bool VisibleCube(Vector3D p,float halfSide)=0;
};

#include "PotVisAll.h"
#include "PotVisIntersection.h"
#include "PotVisPlane.h"

#endif /* POTVIS_H_ */
