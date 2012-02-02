/*
 * PotVisIntersection.h
 *
 *  Created on: Sep 16, 2010
 *      Author: petter
 */

#ifndef POTVISINTERSECTION_H_
#define POTVISINTERSECTION_H_

#include "PotVis.h"

class PotVisIntersection: public PotVis
{
public:
	PotVisIntersection(PotVis *pva,PotVis *pvb);
	virtual ~PotVisIntersection();
	bool Visible(Vector3D p);
	bool VisibleCube(Vector3D p,float halfSide);
private:
	PotVis *a,*b;
};

#endif /* POTVISINTERSECTION_H_ */
