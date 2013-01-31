/**
 * radar.hpp
 * 
 * Author   :   Fatih Erol
 * Date     :   28.02.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_RADAR_HPP
#define GRAPHICSLAB_RADAR_HPP

// Base class include
#include "drawable.hpp"

// Includes
#include "types.hpp"
#include <vector>

GRAPHICSLAB_NAMESPACE_BEGIN

/** Radar */
class Radar : public Drawable
{
public:
    /** Constructor */
    Radar();
    
    /** Destructor */
    ~Radar();
    
    /** Draw the object to the screen. */
    void draw() const;
    float dist(float, float) const;
    
    void setWidth( float width );
    void setHeight( float height );
	void addPosition( const Point &position );
	void clear();
    
private:
    float _width, _height;
    
	typedef std::vector<Point> PositionVector;
	PositionVector _positions;

}; // class Radar

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_RADAR_HPP


