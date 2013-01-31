/**
 * screenTexture.cpp
 *
 * Author   :   Fatih Erol
 * Date     :   28.03.2010
 *
 * All rights reserved.
 */

// Class declaration include
#include "screenTexture.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

ScreenTexture::ScreenTexture() :
	Texture()
{
}

void ScreenTexture::init()
{
}


void ScreenTexture::grabScreen( const Rectangle &rect )
{
    // Set the texture data by grabbing image as a rectangle from the screen frame buffer
    glBindTexture( _target, _id );
	glCopyTexImage2D( _target, 0, GL_RGBA, rect.x, rect.y, rect.width, rect.height, 0 );
}

uchar *ScreenTexture::getImage() const
{
    glBindTexture( _target, _id );
	GLint width, height;
	glGetTexLevelParameteriv( _target, 0, GL_TEXTURE_WIDTH, &width );
	glGetTexLevelParameteriv( _target, 0, GL_TEXTURE_HEIGHT, &height);
	uchar *data = new uchar[height*width*4];
	glGetTexImage( _target, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

	return data;
}

GRAPHICSLAB_NAMESPACE_END

