/**
 * radar.cpp
 * 
 * Author   :   Fatih Erol
 * Date     :   28.02.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "radar.hpp"

// Includes
#include "gl_includes.hpp"
#include "textBox.hpp"

#include <cmath>
#include <sstream>
#include <iomanip>

GRAPHICSLAB_NAMESPACE_BEGIN

Radar::Radar() :
    Drawable(),
    _width( 1.0 ),
    _height( 1.0 )
{
}

Radar::~Radar()
{
}

void Radar::setWidth( float width )
{
    _width = width;
}

void Radar::setHeight( float height )
{
    _height = height;
}

void Radar::addPosition( const Point &position )
{
	_positions.push_back( position );
}

void Radar::clear()
{
	_positions.clear();
}

float Radar::dist(float x, float z) const
{
	return sqrt(x * x + z * z);
}

void Radar::draw() const
{
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    
    // Transparency value for the color
    float alpha = 0.6;
    
    // Draw the outer frame box
	glColor4f( 1.0, 0.0, 0.0, alpha );    
    glBegin( GL_LINE_LOOP );
    glVertex3f( 0, 0, 0.0 );
    glVertex3f( _width, 0, 0.0 );
    glVertex3f( _width, _height, 0.0 );
    glVertex3f( 0, _height, 0.0 );
    glEnd();
		
	float xMargin = 3.0;
    float yMargin = 3.0;
    
    /**************************************************/
    /**************** GRAPHICS LAB 02 *****************/
    /**************************************************/
    /* Insert code here to draw the content of the    */
    /* radar user interface. It will display the      */
    /* projection of the movement of the bullet in    */
    /* two dimensions within the white inner frame.   */
    /* Radar has a vector of position                 */
    /* points sampled during motion of the bullet, so */
    /* the left side will correspond to initial       */
    /* position, and a line path will show the rest.  */
    /* The y value of the position will determine the */
    /* y value of the path vertex, while x value of   */
    /* the path vertex will represent distance from   */
    /* the first position in the vector of positions. */
    /* Therefore x value is actually a distance       */
    /* calculated in xz-plane. You can set a maximum  */
    /* value of 10.0 in both directions to scale the  */
    /* displayed values within the frame.
    /**************************************************/
    
	if (_positions.size() > 0)
	{
		// Draw text that shows distance (in xz-plane)
		glPushMatrix();
		glColor4f( 1.0, 0.0, 0.0, alpha );
		float textHeight = 12.0;
		std::stringstream txt; txt << "Distance: " << std::setfill(' ') << std::setw(3) << std::setprecision(2) << std::fixed << dist(_positions.back().x, _positions.back().z);
		TextBox text( txt.str(), textHeight, 2 );
		float textWidth = text.getWidth();
		glTranslatef( _width/2.0 - textWidth/2.0, yMargin, 0.0 );
		text.draw();
		glPopMatrix();

		// Draw the radar
	    float left = xMargin, right = _width - xMargin;
	    float bottom = yMargin + textHeight + yMargin, top = _height - yMargin;
		float center = (bottom + top) / 2.0;
		float width = right - left, height = top - bottom;
		float maxDist = 10.0;

		// Draw the inner frame box
		glColor4f( 1.0, 1.0, 1.0, alpha );    
		glBegin( GL_LINE_LOOP );
		glVertex3f( left, bottom, 0.0 );
		glVertex3f( right, bottom, 0.0 );
		glVertex3f( right, top, 0.0 );
		glVertex3f( left, top, 0.0 );
		glEnd();

		// draw positions
		glColor4f( 1.0, 1.0, 1.0, alpha );    
		glBegin( GL_LINE_STRIP );
		for (PositionVector::const_iterator it = _positions.begin(); it < _positions.end(); it++)
			glVertex3f(left + dist((*it).x, (*it).z) / maxDist * width, bottom + (*it).y / maxDist * height, 0.0);
	    glEnd();
	}

    /**************************************************/
    /**************************************************/

    glPopMatrix();
}


GRAPHICSLAB_NAMESPACE_END


