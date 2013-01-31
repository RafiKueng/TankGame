/**
 * defs.hpp
 *
 * Author   :   Fatih Erol
 * Date     :   16.02.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_DEFS_HPP
#define GRAPHICSLAB_DEFS_HPP

// Definition for NULL pointer
#ifndef NULL
#define NULL 0
#endif


// Namespace definitions

#define GRAPHICSLAB_NAMESPACE_BEGIN \
namespace graphics_lab {

#define GRAPHICSLAB_NAMESPACE_END \
}

#define GRAPHICSLAB_NAMESPACE_QUALIFIER graphics_lab::
#define GRAPHICSLAB_NAMESPACE_USE using namespace graphics_lab;

// Include for exact width types like uint32_t
#include <stdint.h>
typedef unsigned int uint;
typedef unsigned char uchar;

// Constants
#define PI 3.14159265
#define TO_RADIAN( degree ) ( degree*PI/180 )
#define TO_DEGREE( radian ) ( radian*180/PI )

#define GRAVITATIONAL_ACCELARATION (-9.81)

#endif  //  GRAPHICSLAB_DEFS_HPP


