/**
 * powerBar.hpp
 * 
 * Author   :   Fatih Erol
 * Date     :   28.02.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_POWERBAR_HPP
#define GRAPHICSLAB_POWERBAR_HPP

// Base class include
#include "drawable.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

/** Class to draw user interface element that displays cannon power */
class PowerBar : public Drawable
{
public:
    /** Constructor */
    PowerBar();
    
    /** Destructor */
    ~PowerBar();
    
    /** Draw the object to the screen. */
    void draw() const;
    
    
    void setWidth( float width );
    void setHeight( float height );
    void setPower( float power );
    
private:
    float _width, _height;
    float _power;
    
}; // class PowerBar

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_POWERBAR_HPP


