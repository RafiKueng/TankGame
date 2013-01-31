/**
 * texture.cpp
 *
 * Author   :   Fatih Erol
 * Date     :   18.04.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "texture.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN

Texture::Texture( GLenum target ) :
	_target( target ),
    _width( 1.0 ), _height( 1.0 )
{    
    glGenTextures( 1, &_id );
}

Texture::~Texture()
{
    glDeleteTextures( 1, &_id );
}

void Texture::reset()
{
}

uchar *Texture::getImage() const
{    
	glBindTexture( _target, _id );
	GLint width, height;
	glGetTexLevelParameteriv( _target, 0, GL_TEXTURE_WIDTH, &width );
	glGetTexLevelParameteriv( _target, 0, GL_TEXTURE_HEIGHT, &height);
	uchar *data = new uchar[height*width*4];
	glGetTexImage( _target, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    
	return data;
}

void Texture::setActive( bool on ) const
{
    if ( on )
    {
        glEnable( _target );
        glBindTexture( _target, _id );
        glTexParameteri( _target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( _target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( _target, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri( _target, GL_TEXTURE_WRAP_T, GL_CLAMP );
    }
    else
    {
        glDisable( _target );
    }
    
}

/////////////////////////////////////////////////////////////////////////
// ImageTexture
/////////////////////////////////////////////////////////////////////////
ImageTexture::ImageTexture() :
    Texture()
{
}

void ImageTexture::init()
{
}

void ImageTexture::setImage( const uchar *image, uint width, uint height )
{
    _width = width; _height = height;
    
    // Set the texture data
	glBindTexture( _target, _id );
    glTexImage2D( _target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
}
/////////////////////////////////////////////////////////////////////////
// CheckerTexture
/////////////////////////////////////////////////////////////////////////
void CheckerTexture::init()
{    
    reset();
    

    GLint width = 512;
    GLint height = 512;
    _width = width; _height = height;
    uint bytesPerPixel = 4;
    
    uchar *data = new uchar[height*width*bytesPerPixel];
    
    // Prepare the data for the checker pattern image
    for ( int i = 0; i < height; i++ )
    {
        for ( int j = 0; j < width; j++ )
        {
            uint index = i*width*bytesPerPixel + j*bytesPerPixel;
            uint size = 32;
            bool white = ( ( i % (2*size) ) > size ) && ( ( j % (2*size) ) > size );
            for ( uint k = 0; k < bytesPerPixel; k++ )
            {
                data[index + k] = ( white ? 255 : 0 );
            }
        }
    }
    
    // Set the texture data
    glBindTexture( _target, _id );
    glTexImage2D( _target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    
    delete [] data;
}


GRAPHICSLAB_NAMESPACE_END

