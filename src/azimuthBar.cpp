/**
 * azimuthBar.cpp
 *
 * Author   :   Fatih Erol
 * Date     :   28.02.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "azimuthBar.hpp"

// Includes
#include "gl_includes.hpp"
#include "textBox.hpp"

#include <cmath>
#include <sstream>
#include <iomanip>

GRAPHICSLAB_NAMESPACE_BEGIN

AzimuthBar::AzimuthBar() :
    Drawable(),
    _width( 1.0 ),
    _height( 1.0 ),
    _azimuth( 0.0 )
{
}

AzimuthBar::~AzimuthBar()
{
}

void AzimuthBar::setWidth( float width )
{
    _width = width;
}

void AzimuthBar::setHeight( float height )
{
    _height = height;
}

void AzimuthBar::setAzimuth( float azimuth )
{
    _azimuth = azimuth;
}

void AzimuthBar::draw() const
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
    /* azimuth display. Again, we display the text    */
    /* with the azimuth value. Then we draw a white   */
    /* circular frame, sliced into four regions.      */
    /* Similar to ElevationBar, we use different radii*/
    /* in x and y directions to multiply with sine    */
    /* and cosine to get the x and y values.          */
    /* We draw a yellow thick line to show the        */
    /* azimuth direction. Note that before thickening */
    /* the yellow line, we remember the original      */
    /* value using glGetFloatv function with          */
    /* GL_LINE_WIDTH parameter, and reset the line    */
    /* width back to this after drawing the line.     */
    /**************************************************/

	// Draw text that shows azimuth
    glPushMatrix();
	glColor4f( 1.0, 0.0, 0.0, alpha );
	float textHeight = 12.0;
	std::stringstream txt; txt << "Azimuth: " << std::setfill(' ') << std::setw(3) << std::setprecision(2) << std::fixed << _azimuth;
	TextBox text( txt.str(), textHeight, 2 );
	float textWidth = text.getWidth();
    glTranslatef( _width/2.0 - textWidth/2.0, yMargin, 0.0 );
    text.draw();
    glPopMatrix();

	// Draw the circle with four regions
    float left = xMargin, right = _width - xMargin;
    float bottom = yMargin + textHeight + yMargin, top = _height - yMargin;
	float centerX = (left + right) / 2.0, centerY = (bottom + top) / 2.0;
	float radiusX = (right - left) / 2.0, radiusY = (top - bottom) / 2.0;
    float angleStep = 0.01;

	// outline of circle
	glBegin( GL_LINE_LOOP );
    glColor4f( 1.0, 1.0, 1.0, alpha );
	for (float a = 0.0; a <= 2.0 * PI; a += angleStep)
		glVertex3f(centerX + radiusX * cos(a), centerY + radiusY * sin(a), 0.0);
    glEnd();

	// 2 lines that slice up circle
	glBegin(GL_LINES);
	glColor4f(1.0, 1.0, 1.0, alpha);
	glVertex3f(centerX, bottom, 0.0);
	glVertex3f(centerX, top, 0.0);
	glVertex3f(left, centerY, 0.0);
	glVertex3f(right, centerY, 0.0);
	glEnd();

	// thick line to represent azimuth direction
	GLfloat originalWidth;
	glGetFloatv(GL_LINE_WIDTH, &originalWidth);
	glLineWidth(5.0f);
	glBegin( GL_LINES );
    glColor4f( 1.0, 1.0, 0.0, alpha );
	glVertex3f( centerX, centerY, 0.0 );
	glVertex3f(centerX + radiusX * cos((_azimuth + 90) / 180.0 * PI), centerY+ radiusY * sin((_azimuth + 90)  / 180.0 * PI), 0.0);
    glEnd();
	glLineWidth(originalWidth);

    /**************************************************/
    /**************************************************/


    glPopMatrix();
}


GRAPHICSLAB_NAMESPACE_END


