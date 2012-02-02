#include "soundengine.h"

soundengine::soundengine(void)
{
	m_hudp=NULL;
	channels.clear();
	sounds2D.clear();
	sounds3D.clear();
	channelindex=0;
}
void soundengine::hudprint(string s)
{
	if(m_hudp!=NULL)
	{
		m_hudp->print(s);
	}
}
void soundengine::init(void)
{
	channels.clear();
	sounds2D.clear();
	sounds3D.clear();
	channelindex=0;

	errcheck(FMOD::System_Create(&fmsys));
	unsigned int  fmodver;
	errcheck(fmsys->getVersion(&fmodver));
    if (fmodver < FMOD_VERSION)
    {
			hudprint("Error!  You are using an old version of FMOD "+int2string(fmodver)+".  This program requires "+int2string(FMOD_VERSION));
    }
	FMOD_SPEAKERMODE  speakermode;
	errcheck(fmsys->getDriverCaps(0,NULL,NULL,NULL,&speakermode));
    errcheck(fmsys->setSpeakerMode(speakermode));
    errcheck(fmsys->init(1000,  FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, NULL));
}
void soundengine::errcheck(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
			hudprint(string("FMOD error: ")+FMOD_ErrorString(result));
    }
}
void soundengine::add2D(string name,string extension,bool loop)
{
	sound s;
	string fn="data/sounds/"+name+extension;
	errcheck(fmsys->createSound(fn.c_str(), FMOD_HARDWARE, NULL, &s.fmsound));
	if(loop)
		errcheck(s.fmsound->setMode(FMOD_LOOP_NORMAL));
	else
		errcheck(s.fmsound->setMode(FMOD_LOOP_OFF));
	s.name=name;
	sounds2D.push_back(s);
}
void soundengine::add3D(string name,string extension,float max,bool loop)
{
	sound s;
	string fn="data/sounds/"+name+extension;
	errcheck(fmsys->createSound(fn.c_str(), FMOD_SOFTWARE | FMOD_3D, NULL, &s.fmsound));
	if(loop)
		errcheck(s.fmsound->setMode(FMOD_LOOP_NORMAL));
	else
		errcheck(s.fmsound->setMode(FMOD_LOOP_OFF));
	errcheck(s.fmsound->set3DMinMaxDistance(14.0f,100000.0f));
	s.name=name;
	sounds3D.push_back(s);
}
int soundengine::play2D(string name)
{
	for(size_t i=0;i<sounds2D.size();i++)
	{
		if(name==sounds2D[i].name)
		{
			Channel *c;
			errcheck(fmsys->playSound(FMOD_CHANNEL_FREE, sounds2D[i].fmsound, false, &c));
			channels[channelindex]=c;
			channelindex++;
			return channelindex-1;
		}
	}
	hudprint("Bad sound name: "+name);
	return -1;
}
int soundengine::play3D(string name,float volume,zvector p,zvector v)
{
	for(size_t i=0;i<sounds3D.size();i++)
	{
		if(name==sounds3D[i].name)
		{
			FMOD_VECTOR pos = { p.x, p.y, p.z};
			FMOD_VECTOR vel = { v.x, v.y, v.z};
			Channel *c;
			errcheck(fmsys->playSound(FMOD_CHANNEL_FREE, sounds3D[i].fmsound, true, &c));
			errcheck(c->set3DAttributes(&pos, &vel));
			errcheck(c->setVolume(volume));
			errcheck(c->setPaused(false));
			channels[channelindex]=c;
			channelindex++;
			return channelindex-1;
		}
	}
	hudprint("Bad sound name: "+name);
	return 0;
}
void soundengine::listener(camera *cam)
{
	FMOD_VECTOR from=cam->from.getfmvec();
	zvector v=zvector();
	FMOD_VECTOR vel=v.getfmvec();
	FMOD_VECTOR dir=cam->to.getfmvec();
	FMOD_VECTOR up=cam->up.getfmvec();
	errcheck(fmsys->set3DListenerAttributes(0, &from,&vel,&dir,&up));
}
void soundengine::update(float speed)
{
	FMOD::ChannelGroup *  channelgroup;
	errcheck(fmsys->getMasterChannelGroup(&channelgroup));
	errcheck(fmsys->update());
	errcheck(channelgroup->setPitch(speed));
	errcheck(fmsys->update());
}
void soundengine::stop(int c)
{
	if(c==-1)
	{
		hudprint("Bad sound");
		return;
	}
	if(channels.find(c)==channels.end())
	{
		hudprint("Non existing sound");
		return;
	}
	channels[c]->stop();
	channels.erase(c);
}
void soundengine::set3d(int id,zvector p,zvector v)
{
	if(id==-1)
		hudprint("Bad channel id");
	if(channels.find(id)==channels.end())
	{
		hudprint("Non existing sound");
		return;
	}
	FMOD_VECTOR pos = { p.x, p.y, p.z};
	FMOD_VECTOR vel = { v.x, v.y, v.z};
	channels[id]->set3DAttributes(&pos,&vel);
}
void soundengine::setvol(int id,float v)
{
	if(id==-1)
		hudprint("Bad channel id");
	if(channels.find(id)==channels.end())
	{
		hudprint("Non existing sound");
		return;
	}
	errcheck(channels[id]->setVolume(v));
}
void soundengine::setspeed(int id,float f)
{
	if(id==-1)
		hudprint("Bad channel id");
	if(channels.find(id)==channels.end())
	{
		hudprint("Non existing sound");
		return;
	}
	Sound *s;
	errcheck(channels[id]->getCurrentSound(&s));
	float df;
	errcheck(s->getDefaults(&df,NULL,NULL,NULL));
	errcheck(channels[id]->setFrequency(f*df));
}
void soundengine::getspectrum(int size,float *spectrum)
{
	errcheck(fmsys->getSpectrum(spectrum,size, 0, FMOD_DSP_FFT_WINDOW_HAMMING));
}