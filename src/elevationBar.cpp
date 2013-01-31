/**
 * elevationBar.cpp
 * 
 * Author   :   Fatih Erol
 * Date     :   28.02.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "elevationBar.hpp"

// Includes
#include "gl_includes.hpp"
#include "textBox.hpp"

#include <cmath>
#include <sstream>
#include <iomanip>

GRAPHICSLAB_NAMESPACE_BEGIN

ElevationBar::ElevationBar() :
    Drawable(),
    _width( 1.0 ),
    _height( 1.0 ),
    _elevation( 0.0 )
{
}

ElevationBar::~ElevationBar()
{
}

void ElevationBar::setWidth( float width )
{
    _width = width;
}

void ElevationBar::setHeight( float height )
{
    _height = height;
}

void ElevationBar::setElevation( float elevation )
{
    _elevation = elevation;
}

void ElevationBar::draw() const
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
    /* elevation bar. The outer frame box is already  */
    /* drawn above. Check the lab slides for this     */
    /* week to see what needs to be drawn.            */
    /* Basically, we write the text in the bottom     */
    /* to display "Elevation: " and the value of      */
    /* elevation, as the PowerBar class does for      */
    /* power value. Then we draw a white frame that   */
    /* is shaped like a quarter slice of a circle,    */
    /* only the radius is taken differently in x and  */
    /* y directions so that the shape fits well in    */
    /* the box. The circular part can be considered   */
    /* as lines with vertices matching to x and y     */
    /* values radiusX*cos(A) and radiusY*sin(A)       */
    /* respectively. Inside the circle slice, we draw */
    /* representation of the actual elevation value   */
    /* as a filled polygon with similar vertices,     */
    /* with the radii shortened a bit to produce the  */
    /* empty space with the frame in the figure.      */
    /**************************************************/
    
	// Draw text that shows elevation
    glPushMatrix();
	glColor4f( 1.0, 0.0, 0.0, alpha );
	float textHeight = 12.0;
	std::stringstream txt; txt << "Elevation: " << std::setfill(' ') << std::setw(3) << std::setprecision(2) << std::fixed << _elevation;
	TextBox text( txt.str(), textHeight, 2 );
	float textWidth = text.getWidth();
    glTranslatef( _width/2.0 - textWidth/2.0, yMargin, 0.0 );
    text.draw();
    glPopMatrix();

	// Draw the slice of circle, the angle of which represents elevation value
    float left = xMargin, right = _width - xMargin;
    float bottom = yMargin + textHeight + yMargin, top = _height - yMargin;
	float radiusX = right - left, radiusY = top - bottom;
    float angleStep = 0.01;
	float radiusDiff = 2.0;

	// outline
	glBegin( GL_LINE_LOOP );
    glColor4f( 1.0, 1.0, 1.0, alpha );
    glVertex3f( left, bottom, 0.0 );
	for (float a = 0.0; a <= PI / 2.0; a += angleStep)
		glVertex3f(left + radiusX * cos(a), bottom + radiusY * sin(a), 0.0);
    glVertex3f( left, top, 0.0 );
    glEnd();

	// filling
	glBegin( GL_POLYGON );
    glColor4f( 0.0, 1.0, 0.0, alpha );
    glVertex3f( left, bottom, 0.0 );
	for (float a = 0.0; a <= _elevation / 180.0 * PI; a += angleStep)
		glVertex3f(left + (radiusX - radiusDiff) * cos(a), bottom + (radiusY - radiusDiff) * sin(a), 0.0);
    glEnd();

    /**************************************************/
    /**************************************************/
    

    glPopMatrix();
}


GRAPHICSLAB_NAMESPACE_END


