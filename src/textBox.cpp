/**
 * textBox.cpp
 * 
 * Author   :   Fatih Erol
 * Date     :   18.02.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "textBox.hpp"

// Includes
#include "gl_includes.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

TextBox::TextBox( const std::string &text, float height, float lineWidth ) : 
    Drawable(),
    _text( text ),
    _height( height ),
    _lineWidth( lineWidth )
{
}

TextBox::~TextBox()
{
}


float TextBox::getWidth() const
{	
    float textHeight = 120.0;
	float textWidth = 0;
    for ( uint32_t i = 0; i < _text.length(); i++ )
    {
        textWidth += glutStrokeWidth( GLUT_STROKE_ROMAN, _text[i] );
    }
	textWidth *= _height/textHeight;
	return textWidth;
}

void TextBox::draw() const
{
    // Remember original line width
    float lineWidth;
    glGetFloatv( GL_LINE_WIDTH, &lineWidth );
    
    glLineWidth( _lineWidth );
    
    float textHeight = 120.0;
    float scaleFactor = _height/textHeight;
    
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glScalef( scaleFactor, scaleFactor, scaleFactor);
    
    for ( uint32_t i = 0; i < _text.length(); i++ )
    {
        glutStrokeCharacter( GLUT_STROKE_ROMAN, _text[i] );
    }
    
    glPopMatrix();
    
    // Restore line width
    glLineWidth( lineWidth );
}


GRAPHICSLAB_NAMESPACE_END


