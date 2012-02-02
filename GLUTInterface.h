/*
 * GLUTInterface.h
 *
 *  Created on: Jul 16, 2009
 *      Author: petter
 */

#ifndef GLUTINTERFACE_H_
#define GLUTINTERFACE_H_



class Game;

class GLUTInterface
{
public:
	static void setReciever(Game* g);

private:
	static Game* game;

	static void staticRenderScene();
	static void staticIdle();
	static void staticProcessMouse(int button, int state, int x, int y);
	static void staticProcessMouseActiveMotion(int x, int y);
	static void staticProcessMousePassiveMotion(int x, int y);
	static void staticProcessMouseEntry(int state);
	static void staticProcessNormalKeys(unsigned char key, int x, int y);
	static void staticProcessSpecialKeys(int key, int x, int y);
	static void staticPressKey(int key, int x, int y);
	static void staticReleaseKeySpec(int key, int x, int y);
	static void staticReleaseKey(unsigned char key, int x, int y);
	static void staticChangeSize(int w, int h);



	GLUTInterface();
	virtual ~GLUTInterface();
};

#endif /* GLUTINTERFACE_H_ */
