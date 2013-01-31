/**
 * textBox.hpp
 * 
 * Author   :   Fatih Erol
 * Date     :   18.02.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_TEXTBOX_HPP
#define GRAPHICSLAB_TEXTBOX_HPP

// Base class include
#include "drawable.hpp"

// Includes
#include <string>

GRAPHICSLAB_NAMESPACE_BEGIN

/** Class to draw text to the screen */
class TextBox : public Drawable
{
public:
    /** 
     * Constructor 
     *
     * @param[in]   text    Text
     * @param[in]   height	Height of text box, kind of font size
     */
    TextBox( const std::string &text, float height = 1.0, float lineWidth = 1.0 );
    
    /** Destructor */
    ~TextBox();

    /** Draw the object to the screen. */
    void draw() const;

	/** 
	 * Get width of the drawn text.
	 *
	 * @return		Width of the draw text
	 */
	float getWidth() const;
    
private:
    // Text
    std::string _text;

    // Height of the text box
    float _height;

    // Line width
    float _lineWidth;
    
}; // class TextBox

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_TEXTBOX_HPP


