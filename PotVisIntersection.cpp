/*
 * PotVisIntersection.cpp
 *
 *  Created on: Sep 16, 2010
 *      Author: petter
 */

#include "PotVisIntersection.h"

PotVisIntersection::PotVisIntersection(PotVis *pva,PotVis *pvb)
{
	a=pva;
	b=pvb;
}

PotVisIntersection::~PotVisIntersection()
{
	// TODO Auto-generated destructor stub
}

bool PotVisIntersection::Visible(Vector3D p)
{
	return a->Visible(p)&&b->Visible(p);
}

bool PotVisIntersection::VisibleCube(Vector3D p,float halfSide)
{
	return a->VisibleCube(p,halfSide)&&b->VisibleCube(p,halfSide);
}
