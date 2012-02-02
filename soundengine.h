#pragma once


#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>
#include <vector>
#include <map>
#include "zvect.h"
#include "zutils.h"
#include "hud.h"
#include "camera.h"

using namespace std;
using namespace FMOD;


class soundengine
{
private:
	FMOD::System   *fmsys;
	struct sound
	{
		string name;
		Sound* fmsound;
	};
	int channelindex;
	map<int,Channel*> channels;
	vector<sound> sounds2D;
	vector<sound> sounds3D;
	void hudprint(string s);
public:
	hud* m_hudp;
	soundengine(void);
	void listener(camera *cam);
	void add2D(string name,string extension=".wav",bool loop=false);
	void add3D(string name,string extension=".wav",float max=50,bool loop=false);
	int play2D(string name);
	int play3D(string name,float volume,zvector p,zvector v);
	void stop(int id);
	void set3d(int id,zvector p,zvector v);
	void errcheck(FMOD_RESULT result);
	void init(void);
	void update(float speed);
	void getspectrum(int size,float *spectrum);
	void setvol(int id,float v);
	void setspeed(int id,float v);
};
