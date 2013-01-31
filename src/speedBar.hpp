/**
 * speedBar.hpp
 * 
 * Author   :   Fatih Erol
 * Date     :   28.02.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_SPEEDBAR_HPP
#define GRAPHICSLAB_SPEEDBAR_HPP

// Base class include
#include "drawable.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

/** SpeedBar */
class SpeedBar : public Drawable
{
public:
    /** Constructor */
    SpeedBar();
    
    /** Destructor */
    ~SpeedBar();
    
    /** Draw the object to the screen. */
    void draw() const;
    
    
    void setWidth( float width );
    void setHeight( float height );
    void setSpeed( float speed );
    
private:
    float _width, _height;
    float _speed;
    
}; // class SpeedBar

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_SPEEDBAR_HPP


