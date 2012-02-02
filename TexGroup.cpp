#include "TexGroup.h"
#include "utilities.h"

#include <GL/glext.h>
#include <iostream>
#include <math.h>
#include <string.h> //for memcmp by some reason
using namespace std;

Texture::Texture()
{
	id=0;
	edge=GL_REPEAT;
	af=0;
	cubemap=false;
	volume=false;
}
void Texture::loadTGA(const char *filename)				// Loads A TGA File Into Memory
{
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};		// Uncompressed TGA Header
	GLubyte		TGAcompare[12];									// Used To Compare TGA Header
	GLubyte		header[6];										// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;									// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		temp;											// Temporary Variable


	FILE *file = fopen(filename, "rb");							// Open The TGA File

	if(	file==NULL ||											// Does File Even Exist?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||	// Does The Header Match What We Want?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)										// Did The File Even Exist? *Added Jim Strong*
			id=0;										// Return False
		else													// Otherwise
		{
			fclose(file);										// If Anything Failed, Close The File
			id=0;										// Return False
		}
		return;
	}

	width  = header[1] * 256 + header[0];				// Determine The TGA Width	(highbyte*256+lowbyte)
	height = header[3] * 256 + header[2];				// Determine The TGA Height	(highbyte*256+lowbyte)

	if(	width	<=0	||									// Is The Width Less Than Or Equal To Zero
		height	<=0	||									// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))						// Is The TGA 24 or 32 Bit?
	{
		fclose(file);											// If Anything Failed, Close The File
		id=0;
		return;
	}

	bpp	= header[4];								// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel	= bpp/8;							// Divide By 8 To Get The Bytes Per Pixel
	size		= width*height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	data=(GLubyte *)malloc(size);			// Reserve Memory To Hold The TGA Data

	if(	data==NULL ||								// Does The Storage Memory Exist?
		fread(data, 1, size, file)!=size)	// Does The Image Size Match The Memory Reserved?
	{
		if(data!=NULL)							// Was Image Data Loaded
			free(data);							// If So, Release The Image Data

		fclose(file);											// Close The File
		id=0;											// Return False
		return;
	}

	for(GLuint i=0; i<int(size); i+=bytesPerPixel)			// Loop Through The Image Data
	{															// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=data[i];								// Temporarily Store The Value At Image Data 'i'
		data[i] = data[i + 2];		// Set The 1st Byte To The Value Of The 3rd Byte
		data[i + 2] = temp;						// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (file);												// Close The File

	// Build A Texture From The Data
	glGenTextures(1, &id);						// Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, id);				// Bind Our Texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	if (bpp==24)										// Was The TGA 24 Bits
	{
		type=GL_RGB;											// If So Set The 'type' To GL_RGB
	}
	else
	{
		type=GL_RGBA;											// If So Set The 'type' To GL_RGB
	}
	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);											// Texture Building Went Ok, Return True
}
void Texture::loadPNG(const char *filename)
{
	pngInfo info;
	id=pngBind(filename, PNG_BUILDMIPMAPS, PNG_ALPHA, &info ,edge,GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR);


	width=info.Width;
	height=info.Height;
}
void Texture::loadPNGcubemap(const char *filename)
{
	glGenTextures(1, &id);						// Generate OpenGL texture IDs
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);				// Bind Our Texture

	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	std::string filenamenoext=filename;
	filenamenoext=filenamenoext.substr(0,filenamenoext.size()-4);
	for(int i=0;i<6;i++)
	{
		pngRawInfo info;
		string thisname=filenamenoext+int2string(i)+".png";
		pngLoadRaw(thisname.c_str(), &info);

		type=GL_RGB;

		width=info.Width;
		height=info.Height;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, info.Data);
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,type, width, height, type, GL_UNSIGNED_BYTE, info.Data);

		free(info.Data);
		cout << "Loaded "<<thisname<<" "<<width<<"x"<<height<<endl;

	}
	cubemap=true;
}
void Texture::load(string name,bool samename)
{
	string filename="data/textures/"+name;
	if(!samename)
		fn=filename;
	if(fn=="")
	{
		//group->hudprint("Can't load texture without filename");
		return;
	}
	if(fn.find(".tga")+4==fn.size())
	{
		loadTGA(filename.c_str());
	}
	else
	if(fn.find(".png")+4==fn.size())
	{
		loadPNG(filename.c_str());
	}
	else
	{
		cerr<<("Unknown texture format..."+fn);
	}
	if(id==0)
	{
		cerr<<("Couldn't load: "+fn);
	}
	else
	{
		//group->hudprint("Sucessfully loaded: "+fn);

	}
}
void Texture::loadcubemap(string name,bool samename)
{
	string filename="data/textures/"+name;
	if(!samename)
		fn=filename;
	if(fn=="")
	{
		//group->hudprint("Can't load texture without filename");
		return;
	}
	if(fn.find(".png")+4==fn.size())
	{
		loadPNGcubemap(filename.c_str());
	}
	else
	{
		cerr<<"Unknown texture format..."+fn<<endl;
	}
	if(id==0)
	{
		cerr<<"Couldn't load cubemap: "+fn<<endl;
	}
	else
	{
		cout<<"Sucessfully loaded: "+fn<<endl;
	}
}
void Texture::load(GLubyte *ndata,int nwidth,int nheight)
{
	fn="";
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,edge);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,edge);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR/*_MIPMAP_LINEAR*/);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, nwidth, nheight, GL_RGBA, GL_UNSIGNED_BYTE, ndata);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nwidth, nheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, ndata);
}
void Texture::load(GLenum internalformat,int width,int height)
{
	this->width=width;
	this->height=height;
	fn="";
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat,  width,height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
void Texture::makeNoise3D(int n,int startRes)
{
	fn="";
	volume=true;

	//glEnable(GL_TEXTURE_3D);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_3D, id);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	vector < NoiseHyperCube<3> > cubes;

	for(int s=startRes;s<=n;s*=2)
		cubes.push_back(NoiseHyperCube<3>(s));

	char* texels=new char[n*n*n];


	for(int i=0;i<n;i++)
	for(int j=0;j<n;j++)
	for(int k=0;k<n;k++)
	{
		Vector3D p=Vector3D((float(i)+0.5)/float(n),(float(j)+0.5)/float(n),(float(k)+0.5)/float(n));
		float val=0;

		for(int c=0;c<cubes.size();c++)
		{
			//val+=noiseC1<3,78>(p*pow(2.0f,float(pass)))/pow(2.0f,float(pass+1-initialpass));
			val+=cubes[c].getValC1(p) /pow(2.0f,float(c+1));
		}
		val=pow(1.0f-fabs(val),10.0f);

		//texels[i+n*j+n*n*k]=(char) rD(  (val+1.0f)/2.0f*256.0f  );
		texels[i+n*j+n*n*k]=(char) rD( val*256.0f  );
	}


	glTexImage3D(GL_TEXTURE_3D, 0, 1, n, n, n, 0, GL_RED,GL_UNSIGNED_BYTE, texels);
	delete [] texels;
}
void Texture::makeRNoise3D(int n,int startRes)
{
	fn="";
	volume=true;

	//glEnable(GL_TEXTURE_3D);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_3D, id);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	vector < NoiseHyperCube<3> > cubes;

	for(int s=startRes;s<=n;s*=2)
		cubes.push_back(NoiseHyperCube<3>(s));

	char* texels=new char[n*n*n];


	for(int i=0;i<n;i++)
	for(int j=0;j<n;j++)
	for(int k=0;k<n;k++)
	{
		Vector3D p=Vector3D((float(i)+0.5)/float(n),(float(j)+0.5)/float(n),(float(k)+0.5)/float(n));
		float val=0;

		for(int c=0;c<cubes.size();c++)
		{
			//val+=noiseC1<3,78>(p*pow(2.0f,float(pass)))/pow(2.0f,float(pass+1-initialpass));
			val+=cubes[c].getValC1(p) /pow(2.0f,float(c+1));
		}
		val=(1.0f+val)/2.0f;
		//val=pow(1.0f-fabs(val),10.0f);

		//texels[i+n*j+n*n*k]=(char) rD(  (val+1.0f)/2.0f*256.0f  );
		texels[i+n*j+n*n*k]=(char) rD( val*256.0f  );
	}


	glTexImage3D(GL_TEXTURE_3D, 0, 1, n, n, n, 0, GL_RED,GL_UNSIGNED_BYTE, texels);
	delete [] texels;
}
void Texture::makeRGBNoise3D(int n,int startRes)
{
	fn="";
	volume=true;

	//glEnable(GL_TEXTURE_3D);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_3D, id);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	vector < NoiseHyperCube<3> > Rcubes;
	vector < NoiseHyperCube<3> > Gcubes;
	vector < NoiseHyperCube<3> > Bcubes;

	for(int s=startRes;s<=n;s*=2)
	{
		Rcubes.push_back(NoiseHyperCube<3>(s));
		Gcubes.push_back(NoiseHyperCube<3>(s));
		Bcubes.push_back(NoiseHyperCube<3>(s));
	}
	float* texels=new float[n*n*n*3];


	for(int i=0;i<n;i++)
	for(int j=0;j<n;j++)
	for(int k=0;k<n;k++)
	{
		Vector3D p=Vector3D((float(i)+0.5)/float(n),(float(j)+0.5)/float(n),(float(k)+0.5)/float(n));
		float Rval=0;
		float Gval=0;
		float Bval=0;

		for(int c=0;c<Rcubes.size();c++)
		{
			//val+=noiseC1<3,78>(p*pow(2.0f,float(pass)))/pow(2.0f,float(pass+1-initialpass));
			Rval+=Rcubes[c].getValC1(p) /pow(2.0f,float(c+1));
			Gval+=Gcubes[c].getValC1(p) /pow(2.0f,float(c+1));
			Bval+=Bcubes[c].getValC1(p) /pow(2.0f,float(c+1));
		}
		/*Rval=(1.0f+Rval)/2.0f;
		Gval=(1.0f+Gval)/2.0f;
		Bval=(1.0f+Bval)/2.0f;*/
		//val=pow(1.0f-fabs(val),10.0f);
		const float scale=10.0;
		//texels[i+n*j+n*n*k]=(char) rD(  (val+1.0f)/2.0f*256.0f  );
		texels[(i+n*j+n*n*k)*3+0]=Rval*scale;//(char) rD( Rval*256.0f  );
		texels[(i+n*j+n*n*k)*3+1]=Gval*scale;//(char) rD( Gval*256.0f  );
		texels[(i+n*j+n*n*k)*3+2]=Bval*scale;//(char) rD( Bval*256.0f  );
	}


	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB32F_ARB, n, n, n, 0, GL_RGB,GL_FLOAT, texels);
	delete [] texels;

}
void Texture::bind()
{
	//glActiveTextureARB(GL_TEXTURE0_ARB);
	GLenum cap;
	if(cubemap)
		cap=GL_TEXTURE_CUBE_MAP_EXT;
	else if(volume)
		cap=GL_TEXTURE_3D;
	else
		cap=GL_TEXTURE_2D;
	glEnable(cap);
	glBindTexture(cap, id);
}
void Texture::bind(int num)
{
	glActiveTextureARB(GL_TEXTURE0_ARB+num);
	GLenum cap;
	if(cubemap)
		cap=GL_TEXTURE_CUBE_MAP_EXT;
	else if(volume)
		cap=GL_TEXTURE_3D;
	else
		cap=GL_TEXTURE_2D;
	glEnable(cap);
	glBindTexture(cap, id);
}
bool Texture::isloaded()
{
	return id>0;
}
void Texture::setAF(int b)
{
	af=b;
	#define GL_TEXTURE_MAX_ANISOTROPY_EXT          0x84FE
	if(cubemap)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, id);
		glTexParameterf(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAX_ANISOTROPY_EXT, float(af));
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, float(af));
	}
}
void Texture::reload()
{
	if(id!=0 && fn!="")
	{
		glDeleteTextures(1, &id);
		load(fn.c_str(),true);
		setAF(af);
	}
}
GLint Texture::getid()
{
	return id;
}
////////////////////////////texture group/////////////////////////////////
TexGroup* TexGroup::singleton=NULL;

TexGroup::TexGroup(void)
{
	textures.clear();
	edge=GL_REPEAT;
}

void TexGroup::reload()
{
	for(size_t i=0;i<textures.size();i++)
		textures[i].reload();
}
void TexGroup::setAF(int n)
{
	for(size_t i=0;i<textures.size();i++)
		textures[i].setAF(n);
}

int TexGroup::add(string fn)
{
	cout << "loading "+fn+"... ";
	for(int i=0;i<textures.size();i++)
	{
		if(textures[i].fn==fn)
			return i;
	}
	Texture t;
	t.group=this;
	t.edge=edge;
	t.load(fn);
	cout  << int2string(t.getWidth()) << "x"<< int2string(t.getHeight()) ;
	textures.push_back(t);
	cout << " done"<<endl;
	return textures.size()-1;
}
int TexGroup::addcubemap(string fn)
{
	for(int i=0;i<textures.size();i++)
	{
		if(textures[i].fn==fn)
			return i;
	}
	Texture t;
	t.group=this;
	t.edge=edge;
	t.loadcubemap(fn);
	textures.push_back(t);
	return textures.size()-1;
}
int TexGroup::add(GLubyte *data,int width,int height)
{
	Texture t;
	t.group=this;
	t.edge=edge;
	t.load(data,width,height);
	textures.push_back(t);
	return textures.size()-1;
}
int TexGroup::add(GLenum internalformat,int width,int height)
{
	Texture t;
	t.group=this;
	t.edge=edge;
	t.load(internalformat,width,height);
	textures.push_back(t);
	return textures.size()-1;
}
int TexGroup::addNoise3D(int size,int startRes)
{
	Texture t;
	t.makeNoise3D(size,startRes);
	t.group=this;
	textures.push_back(t);
	return textures.size()-1;
}
int TexGroup::addRNoise3D(int size,int startRes)
{
	Texture t;
	t.makeRNoise3D(size,startRes);
	t.group=this;
	textures.push_back(t);
	return textures.size()-1;
}
int TexGroup::addRGBNoise3D(int size,int startRes)
{
	Texture t;
	t.makeRGBNoise3D(size,startRes);
	t.group=this;
	textures.push_back(t);
	return textures.size()-1;
}
void TexGroup::bind(int i,int num)
{
	if(num<0/* || num>=GL_MAX_TEXTURE_UNITS_ARB*/)
	{
	//	hudprint("Bad multitexture id");
		return;
	}
	if(i<0)
		return;
	if(i<textures.size())
	{
		textures[i].bind(num);
	}
	//else
		//hudprint("No such texture.(i to high)");
}
GLint TexGroup::getid(int i)
{
	if(i<0 || i>textures.size())
	{
		//hudprint("Bad argument for GLint texgroup::getid(int i)");
		return -1;
	}
	else
		return textures[i].getid();
}
int TexGroup::getWidth(int i)
{
	if(i<0 || i>textures.size())
	{
		//hudprint("Bad argument for GLint texgroup::getid(int i)");
		return -1;
	}
	else
		return textures[i].getWidth();
}
int TexGroup::getHeight(int i)
{
	if(i<0 || i>textures.size())
	{
		//hudprint("Bad argument for GLint texgroup::getid(int i)");
		return -1;
	}
	else
		return textures[i].getHeight();
}
void TexGroup::disableallmultitex(void)
{
	//glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_CUBE_MAP_EXT);
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_CUBE_MAP_EXT);
	glActiveTextureARB(GL_TEXTURE3_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_CUBE_MAP_EXT);
	glActiveTextureARB(GL_TEXTURE4_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_CUBE_MAP_EXT);
	glActiveTextureARB(GL_TEXTURE5_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_CUBE_MAP_EXT);
	glActiveTextureARB(GL_TEXTURE6_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_CUBE_MAP_EXT);
	glActiveTextureARB(GL_TEXTURE7_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_CUBE_MAP_EXT);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_CUBE_MAP_EXT);
}
TexGroup*  TexGroup::getSingleton()
{
	if(singleton==NULL)
		singleton=new TexGroup();

	return singleton;
}
