#include <iostream>

#include "font.h"

using namespace std;

Font::Font(void)
{
	tex=-1;
	dl=-1;
}
void Font::build(string fn,float height,float width,float sep)
{
	TexGroup *tg=TexGroup::getSingleton();
	tex=tg->add(fn);

	cout << "building font: "<<fn<<"... ";
	dl=glGenLists(256);	cout << dl<<endl;									// Creating 95 Display Lists
	tg->bind(tex);
	for (int loop=0; loop<256; loop++)							// Loop Through All 95 Lists
	{
		float cx=float(loop%16)/16.0f;							// X Position Of Current Character
		float cy=float(loop/16)/16.0f;							// Y Position Of Current Character

		glNewList(dl+loop,GL_COMPILE);						// Start Building A List
				glBegin(GL_QUADS);									// Use A Quad For Each Character
					glTexCoord2f(cx,         cy+1.0f/16.0f); glVertex2f(0,0);	// Texture / Vertex Coord (Bottom Left)
					glTexCoord2f(cx+1.0f/16.0f, cy+1.0f/16.0f); glVertex2f(width,0);	// Texutre / Vertex Coord (Bottom Right)
					glTexCoord2f(cx+1.0f/16.0f, cy);		  glVertex2f(width,height);// Texture / Vertex Coord (Top Right)
					glTexCoord2f(cx,         cy);		  glVertex2f(0,height);	// Texture / Vertex Coord (Top Left)
				glEnd();												// Done Building Our Quad (Character)
			glTranslatef(sep+width,0,0);							// Move To The Right Of The Character
		glEndList();											// Done Building The Display List
	}
	cout << "done"<<endl;
										// Loop Until All 256 Are Built
}
void Font::print(float x,float y,string text)
{
	TexGroup *tg=TexGroup::getSingleton();

	if (text == "")											// If There's No Text
		return;													// Do Nothing

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);


	tg->bind(tex);
	glPushMatrix();												// Store The Modelview Matrix
	glTranslated(x,y,0);										// Position The Text (0,0 - Bottom Left)
	glListBase(dl);											// Choose The Font Set

	glCallLists(text.size(), GL_UNSIGNED_BYTE, text.c_str());			// Draws The Display List Text
	glPopMatrix();

}

FontGroup* FontGroup::singleton=NULL;
FontGroup::FontGroup()
{}
Font* FontGroup::add(string name,float height,float width,float sep)
{
	Font *f=new Font();
	f->build(name,height,width,sep);

	fonts.insert(  pair<string,Font*>(name,f)  );
	return f;
}
FontGroup*  FontGroup::getSingleton()
{
	if(singleton==NULL)
		singleton=new FontGroup();

	return singleton;
}
