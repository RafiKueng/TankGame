/**
 * House.cpp
 *
 * Author   :   Rkueng
 * Date     :   
 *
 * All rights reserved.
 */

// Class declaration include
#include "House.hpp"

// Includes
#include "gl_includes.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN

void House::draw() const
{
	float offset = (1 - _popDens / 100.0f)/5.0;
	glPushMatrix();
	glTranslatef((_coord1[0]+_coord2[0])/2.0f, 0.0f, (_coord1[1]+_coord2[1])/2.0f);
	glColor3f(0.7-offset,0.7-offset,0.7-offset);
	glCallList(_displayListIndex);
	glPopMatrix();
}


GRAPHICSLAB_NAMESPACE_END