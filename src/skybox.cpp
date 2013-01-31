/**
 * Skybox.cpp
 *
 * Author   :   Fabian Gehri, rafael küng
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "skybox.hpp"

// Includes
#include "config.hpp"
#include "parameters.hpp"

#include "gl_includes.hpp"
#include <iostream>
#include <cmath>

#include "tgaTexture.hpp"
#include "material.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

Skybox::Skybox() :
    Drawable()
{


	scale = FIELD_SIZE;
    reset();
}


Skybox::~Skybox()
{
}

void Skybox::reset()
{
	//_texNfn = "wall.tga";
	_texNfn = "n.tga";
	_texEfn = "e.tga";
	_texSfn = "s.tga";
	_texWfn = "w.tga";
	_texUfn = "u.tga";
	_texDfn = "d.tga";

	_texN = new TGATexture( _texNfn );
	_texN->init();

	_texE = new TGATexture( _texEfn );
	_texE->init();

	_texS = new TGATexture( _texSfn );
	_texS->init();

	_texW = new TGATexture( _texWfn );
	_texW->init();

	_texU = new TGATexture( _texUfn );
	_texU->init();

	_texD = new TGATexture( _texDfn );
	_texD->init();

    Material _skyMaterial;
	_skyMaterial.setAmbient( Color( 0.2, 0.2, 0.2 ) );
    _skyMaterial.setDiffuse( Color( 0.5, 0.5, 0.5 ) );
    _skyMaterial.setSpecular( Color( 0.6, 0.6, 0.6 ) );
    _skyMaterial.setEmission( Color( 0.0, 0.0, 0.0 ) );
    _skyMaterial.setShininess( 10 );

}


void Skybox::draw() const
{
	float hoffset = scale*0.75;
	glColor3f(0.0,0.0,0.1);
    glPushMatrix();

    glEnable( GL_LIGHTING );
	_skyMaterial.setActive();
	glEnable(GL_TEXTURE_2D);

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	//north
	_texN->setActive( true );
	glNormal3f( 0.0, 0.0, -1.0 );
	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(-scale,scale+hoffset,scale);  //lo
		glTexCoord2f(0,1024); glVertex3f(-scale,-scale+hoffset,scale); //lu
		glTexCoord2f(1024,1024); glVertex3f(scale,-scale+hoffset,scale);  //ru
		glTexCoord2f(1024,0); glVertex3f(scale,scale+hoffset,scale);   //ro
	glEnd();
	_texN->setActive( false );

	//south
	_texS->setActive( true );
	glNormal3f( 0.0, 0.0, 1.0 );
	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(-scale,scale+hoffset,-scale);  //lo
		glTexCoord2f(0,1024); glVertex3f(-scale,-scale+hoffset,-scale); //lu
		glTexCoord2f(1024,1024); glVertex3f(scale,-scale+hoffset,-scale);  //ru
		glTexCoord2f(1024,0); glVertex3f(scale,scale+hoffset,-scale);   //ro
	glEnd();
	_texS->setActive( false );

	//east
	_texE->setActive( true );
	glNormal3f( -1.0, 0.0, 0.0 );
	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(scale,scale+hoffset,-scale);  //lo
		glTexCoord2f(0,1024); glVertex3f(scale,-scale+hoffset,-scale); //lu
		glTexCoord2f(1024,1024); glVertex3f(scale,-scale+hoffset,scale);  //ru
		glTexCoord2f(1024,0); glVertex3f(scale,scale+hoffset,scale);   //ro
	glEnd();
	_texE->setActive( false );

	//West
	_texW->setActive( true );
	glNormal3f( 1.0, 0.0, 0.0 );
	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(-scale,scale+hoffset,-scale);  //lo
		glTexCoord2f(0,1024); glVertex3f(-scale,-scale+hoffset,-scale); //lu
		glTexCoord2f(1024,1024); glVertex3f(-scale,-scale+hoffset,scale);  //ru
		glTexCoord2f(1024,0); glVertex3f(-scale,scale+hoffset,scale);   //ro
	glEnd();
	_texW->setActive( false );

	//top
	_texU->setActive( true );
	glNormal3f( 0.0, -1.0, 0.0 );
	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(-scale,scale+hoffset,scale);  //lo
		glTexCoord2f(0,1024); glVertex3f(-scale,scale+hoffset,-scale); //lu
		glTexCoord2f(1024,1024); glVertex3f(scale,scale+hoffset,-scale);  //ru
		glTexCoord2f(1024,0); glVertex3f(scale,scale+hoffset,scale);   //ro
	glEnd();
	_texU->setActive( false );

    glPopMatrix();
}

GRAPHICSLAB_NAMESPACE_END