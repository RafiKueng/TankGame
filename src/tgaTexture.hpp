/**
 * tgaTexture.hpp
 * 
 * Author   :   Fatih Erol
 * Date     :   18.04.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_TGATEXTURE_HPP
#define GRAPHICSLAB_TGATEXTURE_HPP

// Base class include
#include "texture.hpp"

// Includes
#include "types.hpp"
#include <string>

GRAPHICSLAB_NAMESPACE_BEGIN


/** Texture to load from a TGA image file */
class TGATexture : public Texture
{
public:
	/** 
     * Constructor 
     *
     * @param[in]   fileName    TGA image file name
     */
	TGATexture( const std::string &fileName );
    
    /** Initialize texture data */
    void init();
    
    
private:
    /** Structure to hold data for a TGA-Targa image */
    struct TGAData
    {
        // Width and Height of the image
        uint width, height;
        // Image data in RGBA format, 1 byte per component
        uchar *data;
        
        TGAData() :
            width( 0 ), height( 0 ),
            data( NULL )
        {}
        
        ~TGAData()
        {
            delete [] data;
        }
    };
    /**
     * Load a TGA image from a file.
     * 
     * @param[in]   fileName    Image file name
     *
     * @return      Structure that holds image data
     */
    TGAData *loadTGAFile( const std::string &fileName );

private:
    /** Image file name */
    std::string _fileName;
    
}; // class TGATexture


GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_TGATEXTURE_HPP


