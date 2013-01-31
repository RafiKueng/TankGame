/**
 * tgaTexture.cpp
 *
 * Author   :   Fatih Erol
 * Date     :   18.04.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "tgaTexture.hpp"

// Includes
#include "exception.hpp"
#include "fileUtils.hpp"

#include <sstream>

GRAPHICSLAB_NAMESPACE_BEGIN

TGATexture::TGATexture(  const std::string &fileName ) :
    Texture(),
    _fileName( fileName )
{
}

void TGATexture::init()
{
    reset();
 
    TGAData *tgaData = loadTGAFile( _fileName );
    
    _width = tgaData->width; 
    _height = tgaData->height;
    
    // Set texture data
    glBindTexture( _target, _id );
    glTexImage2D( _target, 0, GL_RGBA, tgaData->width, tgaData->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tgaData->data );
    
    delete tgaData;
}

TGATexture::TGAData *TGATexture::loadTGAFile( const std::string &fileName )
{
    TGAData *data = NULL;
    
    FILE* fp = fopen( fileName.c_str(), "rb");
    
    if (!fp) 
    {
        std::stringstream error;
        error << "Failed to load TGA file \"" << _fileName << "\"";
        throw Exception( error.str() );
    }
    
    uint8_t idLength    =   FileUtils::readByte( fp );
    uint8_t mapType     =   FileUtils::readByte( fp );
    uint8_t imageType   =   FileUtils::readByte( fp );
    uint16_t mapOrigin  =   FileUtils::readShort( fp );
    uint16_t mapLength  =   FileUtils::readShort( fp );
    uint8_t mapWidth    =   FileUtils::readByte( fp );
    uint16_t xOrigin    =   FileUtils::readShort( fp );
    uint16_t yOrigin    =   FileUtils::readShort( fp );
    uint16_t imageWidth =   FileUtils::readShort( fp );
    uint16_t imageHeight=   FileUtils::readShort( fp );
    uint8_t pixelDepth  =   FileUtils::readByte( fp );
    uint8_t imageDesc   =   FileUtils::readByte( fp );
    
    int ordering = ( ( imageDesc&0xff0000 ) >> 4 );
    
    if ( ordering == 1 || ordering == 3 )
    {
        std::stringstream error;
        error << "Ordering of image is not supported";
        throw Exception( error.str() );
    }
    
    // Pull out all of the id data    
    char  szJunk[65535];
    fread( szJunk, 1, idLength, fp );
    
    
    int size = imageWidth * imageHeight * 4;
    int depth  = pixelDepth >> 3;
    
    if ( depth == 2 )
    {
        std::stringstream error;
        error << "Only 8, 24 or 32bit images supported";
        throw Exception( error.str() );
    }
    
    
    data = new TGAData;
    data->width = imageWidth;
    data->height = imageHeight;

    data->data = new uchar[size];
    
    uint8_t buffer[4];
    for ( int i = 0; i < imageHeight; i++ )
    {
        for ( int j = 0; j < imageWidth; j++ )
        {
            fread( buffer, 1, depth, fp );
            switch ( depth )
            {
                case 1:
                    buffer[1] = buffer[2] = buffer[0];
                    buffer[3] = 0;
                    break;
                case 3:
                    buffer[3] = 0;
                    break;
            }
                     
            uint index;
            if ( ordering == 0 )
            {
                index = ( imageHeight - 1 - i ) * imageWidth * 4 + j * 4;
            }
            else
            {
                index = i * imageWidth * 4 + j * 4;
            }
            data->data[index + 0] = buffer[2];
            data->data[index + 1] = buffer[1];
            data->data[index + 2] = buffer[0];
            data->data[index + 3] = buffer[3];            
        }
    }
    
    fclose(fp);

    
    return data;
}

GRAPHICSLAB_NAMESPACE_END

