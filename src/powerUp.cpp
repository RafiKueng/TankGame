/**
 * powerUp.cpp
 *
 * Author   :   Fabian Gehri, rafael küng
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "powerUp.hpp"

// Includes
#include "config.hpp"
#include "parameters.hpp"

#include "gl_includes.hpp"
#include <iostream>
#include <cmath>

GRAPHICSLAB_NAMESPACE_BEGIN

float PowerUp::_angle = 0.0f;
bool PowerUp::firstTime = true;

PowerUp::PowerUp() :
    Drawable()
{
	_angle = 0.0;
    reset();
}

PowerUp::PowerUp(int *location, PowerupType put)
{
	_type = put;
	_loc[0] = location[0];
	_loc[1] = location[1];
	_angle = 0.0;
}

PowerUp::~PowerUp()
{
}

void PowerUp::reset()
{
}



void PowerUp::addTime(float timeSLU)
{
	if(firstTime)
	{
		PowerUp::_angle = 0;
		firstTime = false;
	}
	else
	{
		PowerUp::_angle += timeSLU * 180.0f;
		if (PowerUp::_angle >= 360.0f)
			PowerUp::_angle -= 360.0f;
		else if (PowerUp::_angle < 0.0f)
			PowerUp::_angle += 360.0f;
	}
}

void PowerUp::draw() const
{
    glPushMatrix();
	glTranslatef((float)_loc[0]-FIELD_SIZE/2.0f+0.5f,0.0f,(float)_loc[1]-FIELD_SIZE/2.0f+0.5f); //move to right place
	//glTranslatef(-2.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.1f * std::sin(TO_RADIAN(PowerUp::_angle) * 2.0f), 0.0f); // bounce up and down
	glRotatef(PowerUp::_angle, 0.0f, 1.0f, 0.0f); // spin around

	if (_type == POWERUP_HP)
	{
		GLUquadric *quad = gluNewQuadric();
		// red cross
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glPushMatrix();
		glutSolidCube(0.2f);
		glTranslatef(-0.2f, 0.0f, 0.0f);
		glutSolidCube(0.2f);
		glTranslatef(0.4f, 0.0f, 0.0f);
		glutSolidCube(0.2f);
		glTranslatef(-0.2f, -0.2f, 0.0f);
		glutSolidCube(0.2f);
		glTranslatef(0.0f, 0.4f, 0.0f);
		glutSolidCube(0.2f);
		glPopMatrix();
		// white cylinder
		glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
		glTranslatef(0.0f, 0.0f, -0.1f);
		gluCylinder(quad, 0.5f, 0.5f, 0.2f, 20, 2);
		for (int i = 0; i < 2; i++)
		{
			glPushMatrix();
			glTranslatef(0.0f, 0.0f, i * 0.2f);
			// white disk
			glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
			gluDisk(quad, 0.0f, 0.5f, 20, 2);
			glPopMatrix();
		}
	}
	else if (_type == POWERUP_GRENADE)
	{
		GLUquadric *quad = gluNewQuadric();
		// green sphere
		glColor4f(0.0f, 0.2f, 0.0f, 1.0f);
		glutSolidSphere(0.1f, 20, 20);
		// white cylinder
		glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
		glTranslatef(0.0f, 0.0f, -0.1f);
		gluCylinder(quad, 0.5f, 0.5f, 0.2f, 20, 2);
		for (int i = 0; i < 2; i++)
		{
			glPushMatrix();
			glTranslatef(0.0f, 0.0f, i * 0.2f);
			// white disk
			glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
			gluDisk(quad, 0.0f, 0.5f, 20, 2);
			glPopMatrix();
		}
	}
	// add other power ups here
	else
	{
		GLUquadric *quad = gluNewQuadric();
		glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
		gluSphere(quad, 0.5f, 10, 10);
	}

    glPopMatrix();
}

GRAPHICSLAB_NAMESPACE_END


