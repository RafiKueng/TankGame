/**
 * speedBar.cpp
 * 
 * Author   :   Fatih Erol
 * Date     :   28.02.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "speedBar.hpp"

// Includes
#include "gl_includes.hpp"
#include "textBox.hpp"

#include <cmath>
#include <sstream>
#include <iomanip>

GRAPHICSLAB_NAMESPACE_BEGIN

SpeedBar::SpeedBar() :
    Drawable(),
    _width( 1.0 ),
    _height( 1.0 ),
    _speed( 0.0 )
{
}

SpeedBar::~SpeedBar()
{
}

void SpeedBar::setWidth( float width )
{
    _width = width;
}

void SpeedBar::setHeight( float height )
{
    _height = height;
}

void SpeedBar::setSpeed( float speed )
{
    _speed = speed;
}

void SpeedBar::draw() const
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
    /* display for the Y component of speed that      */
    /* changes over time during motion of the bullet. */
    /* The middle point represents 0 value, and a bar */
    /* up and down this line should represent positive*/
    /* and negative speed values, respectively, drawn */
    /* as a bar similar to PowerBar. We also display  */
    /* text at the bottom of the frame. The bar height*/
    /* can be scaled so that maximum of +/-10.0 is    */
    /* displayed at full bar height. Note that the    */
    /* end of the bar has a color closer to white,    */
    /* similar to the PowerBar bar. The text in the   */
    /* bottom should show the speed, although the bar */
    /* cannot go over the maximum speed of +/-10.0, to*/
    /* stay within the frame boundaries.              */
    /**************************************************/
        
	// Draw text that shows Y component of velocity
    glPushMatrix();
	glColor4f( 1.0, 0.0, 0.0, alpha );
	float textHeight = 12.0;
	std::stringstream txt; txt << "Speed: " << std::setfill(' ') << std::setw(3) << std::setprecision(2) << std::fixed << _speed;
	TextBox text( txt.str(), textHeight, 2 );
	float textWidth = text.getWidth();
    glTranslatef( _width/2.0 - textWidth/2.0, yMargin, 0.0 );
    text.draw();
    glPopMatrix();

	// Draw the bar, the height of which represents the value of the Y component of velocity
    float left = xMargin, right = _width - xMargin;
    float bottom = yMargin + textHeight + yMargin, top = _height - yMargin;
	float center = (bottom + top) / 2.0;
	float maxSpeed = 10.0;
	float normSpeed;
    if (_speed < -maxSpeed) normSpeed = -1.0;
	else if (_speed > maxSpeed) normSpeed = 1.0;
	else normSpeed = _speed / maxSpeed;
	float barHeight = (top - bottom) / 2.0 * normSpeed;

    glBegin( GL_QUADS );
    glColor4f( 1.0, 0.0, 0.0, alpha );
    glVertex3f( left, center, 0.0 );
    glVertex3f( right, center, 0.0 );
    glColor4f( 1.0, abs(normSpeed), abs(normSpeed), alpha );
    glVertex3f( right, center + barHeight, 0.0 );
    glVertex3f( left, center + barHeight, 0.0 );
    glEnd();

    /**************************************************/
    /**************************************************/
    
    
    glPopMatrix();
}


GRAPHICSLAB_NAMESPACE_END


