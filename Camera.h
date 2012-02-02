/*
 * Camera.h
 *
 *  Created on: May 8, 2010
 *      Author: petter
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Vector.h"
#include "PotVis.h"

class Camera: public PotVis
{
public:
	Camera();
	virtual ~Camera();
	void setProjection();
	virtual bool Visible(Vector3D p);
	virtual bool VisibleCube(Vector3D p,float halfSide);
	Vector3D from,to,up;
	float verFOV,near,far;
	int width,height;
private:
	float frustum[6][4];
	void extractFrustum();
	bool inView(Vector3D p,float r=0);
};

#endif /* CAMERA_H_ */
