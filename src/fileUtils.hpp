/**
 * fileUtils.hpp
 * 
 * Author   :   Fatih Erol
 * Date     :   18.04.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_FILEUTILS_HPP
#define GRAPHICSLAB_FILEUTILS_HPP

// Includes
#include "defs.hpp"
#include <stdio.h>

GRAPHICSLAB_NAMESPACE_BEGIN

/** Class for some file utility functions */
class FileUtils
{
public:
    /**
     * Read an 8-bit byte from the open file.
     *
     * @return      Next 8-bit byte read from the file
     */
    static uint8_t readByte( FILE *f );
    
    /**
     * Read a 16-bit short integer from the open file.
     *
     * @return      Next 16-bit short integer read from the file
     */
    static uint16_t readShort( FILE *f );
    
}; // class FileUtils

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_FILEUTILS_HPP


