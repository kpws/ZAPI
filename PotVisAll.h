/*
 * PotVisAll.h
 *
 *  Created on: Sep 16, 2010
 *      Author: petter
 */

#ifndef POTVISALL_H_
#define POTVISALL_H_

#include "PotVis.h"

class PotVisAll: public PotVis
{
public:
	PotVisAll();
	virtual ~PotVisAll();
	virtual bool Visible(Vector3D p)
	{
		return true;
	}
	bool VisibleCube(Vector3D p,float halfSide)
	{
		return true;
	}
};

#endif /* POTVISALL_H_ */
