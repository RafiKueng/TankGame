/**
 * powerBar.cpp
 * 
 * Author   :   Fatih Erol
 * Date     :   28.02.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "powerBar.hpp"

// Includes
#include "gl_includes.hpp"
#include "textBox.hpp"

#include <sstream>
#include <iomanip>

GRAPHICSLAB_NAMESPACE_BEGIN

PowerBar::PowerBar() :
    Drawable(),
    _width( 1.0 ),
    _height( 1.0 ),
    _power( 0.5 )
{
}

PowerBar::~PowerBar()
{
}

void PowerBar::setWidth( float width )
{
    _width = width;
}

void PowerBar::setHeight( float height )
{
    _height = height;
}

void PowerBar::setPower( float power )
{
    _power = power;
}

void PowerBar::draw() const
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

    // Draw text that shows power
    glPushMatrix();
	glColor4f( 1.0, 0.0, 0.0, alpha );
	float textHeight = 12.0;
	std::stringstream txt; txt << "Power: " << std::setfill(' ') << std::setw(3) << std::setprecision(2) << std::fixed << _power;
	TextBox text( txt.str(), textHeight, 2 );
	float textWidth = text.getWidth();
    glTranslatef( _width/2.0 - textWidth/2.0, yMargin, 0.0 );
    text.draw();
    glPopMatrix();
    
    
    // Draw the bar, the height of which represents power value
    float left = xMargin, right = _width - xMargin;
    float bottom = yMargin + textHeight + yMargin, top = _height - yMargin;
    float barHeight = ( top - bottom )*_power;
    
    glBegin( GL_QUADS );
    glColor4f( 1.0, 0.0, 0.0, alpha );
    glVertex3f( left, bottom, 0.0 );
    glVertex3f( right, bottom, 0.0 );
    glColor4f( 1.0, _power, _power, alpha );
    glVertex3f( right, bottom + barHeight, 0.0 );
    glVertex3f( left, bottom + barHeight, 0.0 );
    glEnd();
    
    glPopMatrix();
}


GRAPHICSLAB_NAMESPACE_END


