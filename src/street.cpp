/**
 * Street.cpp
 *
 * Author   :   Rkueng
 * Date     :   
 *
 * All rights reserved.
 */

// Class declaration include
#include "street.hpp"

// Includes
#include "gl_includes.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN

void Street::draw() const
{
	int xDim = _coord2[0]-_coord1[0];
	int zDim = _coord2[1]-_coord1[1];

	glPushMatrix();
	glTranslatef((_coord1[0]+_coord2[0])/2.0f, 0.0f, (_coord1[1]+_coord2[1])/2.0f);
	glScalef((float)xDim, 1, (float)zDim);
	glColor3f(0.3,0.3,0.3);

	glBegin(GL_QUADS);
		glNormal3d(0, 1, 0);
		glTexCoord2f(0, 0);
		glVertex3f(1,0,1);
		glTexCoord2f(1, 0);
		glVertex3f(1,0,0);
		glTexCoord2f(1, 1);
		glVertex3f(0,0,0);
		glTexCoord2f(0, 1);
		glVertex3f(0,0,1);
	glEnd(); 

	glPopMatrix();
}


std::vector<Street*> Street::Div()
{
	std::vector<Street*> tmp;
	return tmp;
}


GRAPHICSLAB_NAMESPACE_END