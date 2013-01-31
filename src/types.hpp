/**
 * types.hpp
 * 
 * Author   :   Fatih Erol
 * Date     :   21.02.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_TYPES_HPP
#define GRAPHICSLAB_TYPES_HPP

#include "defs.hpp"
#include <vector>


GRAPHICSLAB_NAMESPACE_BEGIN

typedef std::vector<int> IntVector;

/** Structure for a 3D point */
struct Point
{
    float x, y, z, w;
    
    Point( float x_ = 0.0, float y_ = 0.0, float z_ = 0.0, float w_ = 1.0 ) :
        x( x_ ), y( y_ ), z( z_ ), w( w_ )
    {
    }
};

/** Structure for a 3D vector */
struct Vector3D
{
    float x, y, z;

    Vector3D( float x_ = 0.0, float y_ = 0.0, float z_ = 0.0 ) :
        x( x_ ), y( y_ ), z( z_ )
    {
    }
};

/** Structure for color */
struct Color
{
    float r, g, b, a;
    
    Color( float r_ = 0.0, float g_ = 0.0, float b_ = 0.0, float a_ = 1.0 ) :
        r( r_ ), g( g_ ), b( b_ ), a( a_ )
    {
    }
};

struct Rectangle
{
    float x, y, width, height;
    
    Rectangle( float x_ = 0.0, float y_ = 0.0, float width_ = 1.0, float height_ = 1.0 ) :
        x( x_ ), y( y_ ), width( width_ ), height( height_ )
    {
    }
};

/** Rendering parameters */
struct RenderingParameters
{
    enum DrawMode 
    {
        WIREFRAME,
        POLYGON
    } drawMode;
    
    enum ShadeMode
    {
        FLAT,
        SMOOTH
    } shadeMode;
    
    RenderingParameters() :
        drawMode( POLYGON ),
        shadeMode( SMOOTH )
    {
    }
};

enum TerrainType
{
	INVALID, STREET_FAST, STREET_MED, STREET_SLOW, BUILDING_BORDER, BUILDING, PLACE, OUTSIDETOWN
};

enum PowerupType
{
	NONE, POWERUP_GRENADE, POWERUP_HP
};


enum PopulationDensity
{
	POP_HIGH, POP_MID, POP_LOW
};

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_TYPES_HPP


