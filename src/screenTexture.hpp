/**
 * screenTexture.hpp
 *
 * Author   :   Fatih Erol
 * Date     :   28.03.2010
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_SCREENTEXTURE_HPP
#define GRAPHICSLAB_SCREENTEXTURE_HPP

// Base class include
#include "texture.hpp"

// Includes
#include "types.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN


/** Texture to load from current drawn screen */
class ScreenTexture : public Texture
{
public:
	/** Constructor */
	ScreenTexture();

    /** Initialize texture data */
    void init();

	/** Grab screen to use as texture */
	void grabScreen( const Rectangle &rect );

	/** 
	 * Get image data for the texture.
	 *
	 * @return	Image data array
	 */
	uchar *getImage() const;

}; // class ScreenTexture


GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_SCREENTEXTURE_HPP


