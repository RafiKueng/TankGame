/**
 * drawable.hpp
 *
 * Author   :   Fatih Erol
 * Date     :   18.02.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_DRAWABLE_HPP
#define GRAPHICSLAB_DRAWABLE_HPP

// Includes
#include "defs.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

/** Abstract base class to provide interface for a drawable object in the scene */
class Drawable
{
public:
    /** Constructor. */
    Drawable() {}
    
    /** Destructor */
    virtual ~Drawable() {}
    
    /** Draw the object to the screen. */
    virtual void draw() const = 0;
    
}; // class Drawable


GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_DRAWABLE_HPP


