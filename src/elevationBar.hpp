/**
 * elevationBar.hpp
 * 
 * Author   :   Fatih Erol
 * Date     :   28.02.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_ELEVATIONBAR_HPP
#define GRAPHICSLAB_ELEVATIONBAR_HPP

// Base class include
#include "drawable.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

/** ElevationBar */
class ElevationBar : public Drawable
{
public:
    /** Constructor */
    ElevationBar();
    
    /** Destructor */
    ~ElevationBar();
    
    /** Draw the object to the screen. */
    void draw() const;
    
    
    void setWidth( float width );
    void setHeight( float height );
    void setElevation( float elevation );
    
private:
    float _width, _height;
    float _elevation;
    
}; // class ElevationBar

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_ELEVATIONBAR_HPP


