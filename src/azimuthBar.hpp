/**
 * azimuthBar.hpp
 * 
 * Author   :   Fatih Erol
 * Date     :   28.02.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_AZIMUTHBAR_HPP
#define GRAPHICSLAB_AZIMUTHBAR_HPP

// Base class include
#include "drawable.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

/** AzimuthBar */
class AzimuthBar : public Drawable
{
public:
    /** Constructor */
    AzimuthBar();
    
    /** Destructor */
    ~AzimuthBar();
    
    /** Draw the object to the screen. */
    void draw() const;
    
    
    void setWidth( float width );
    void setHeight( float height );
    void setAzimuth( float azimuth );
    
private:
    float _width, _height;
    float _azimuth;
    
}; // class AzimuthBar

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_AZIMUTHBAR_HPP


