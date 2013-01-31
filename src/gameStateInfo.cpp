/**
 * gameStateInfo.cpp
 * 
 * Author   :   Fabian Gehri
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "gameStateInfo.hpp"

// Includes
#include "gl_includes.hpp"
#include "parameters.hpp"
#include "textBox.hpp"
#include "config.hpp"

#include <cmath>
#include <sstream>
#include <iomanip>

GRAPHICSLAB_NAMESPACE_BEGIN

GameStateInfo::GameStateInfo() :
    Drawable(),
    _width( 1.0 ),
    _height( 1.0 ),
	_seconds( 0 ),
	_health( 0 ),
    _kills( 0 ),
	_deaths( 0 )
{
}

GameStateInfo::~GameStateInfo()
{
}

void GameStateInfo::setWidth( float width )
{
    _width = width;
}

void GameStateInfo::setHeight( float height )
{
    _height = height;
}

void GameStateInfo::setHealth( int health )
{
	_health=health;
}

void GameStateInfo::setTime( int seconds )
{
	_seconds = seconds;
}

void GameStateInfo::setLevelNr (short nr)
{
	_levelNr = nr;
}

void GameStateInfo::setKills( short kills )
{
    _kills = kills;
}

void GameStateInfo::setDeaths( short deaths )
{
    _deaths = deaths;
}

void GameStateInfo::setKillsToWin( short killsToWin )
{
    _killsToWin = killsToWin;
}

void GameStateInfo::setDeathsToLose( short deathsToLose )
{
    _deathsToLose = deathsToLose;
}

void GameStateInfo::setUpdateTime(float time)
{
	_updateTime = time;
}

void GameStateInfo::draw() const
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
    
	float textHeight = DEBUG_PERFORMANCE ? 21.0 : 26.0;

	glColor4f( 1.0, 0.0, 0.0, alpha );

	if (DEBUG_PERFORMANCE)
	{
		// draw text that shows time since last update
		glPushMatrix();
		std::stringstream txtUpdateTime; txtUpdateTime << "ms per frame: " << std::setfill('0') << std::setw(0) << std::fixed << _updateTime * 1000;
		TextBox textUpdateTime( txtUpdateTime.str(), textHeight, 2 );
		glTranslatef( xMargin, 6.0 * yMargin + 5.0 * textHeight, 0.0 );
		textUpdateTime.draw();
		glPopMatrix();
	}

	// draw text that shows time since last update
	glPushMatrix();
	std::stringstream txtLevelNr; txtLevelNr << "Level " << std::setfill('0') << std::setw(0) << std::fixed << _levelNr;
	TextBox textLevelNr( txtLevelNr.str(), textHeight, 2 );
	glTranslatef( xMargin, 5.0 * yMargin + 4.0 * textHeight, 0.0 );
    textLevelNr.draw();
    glPopMatrix();

	// Draw text that shows time
    glPushMatrix();
	std::stringstream txtTime; txtTime << "Time: " << std::setfill('0') << std::setw(2) << std::fixed << _seconds / 60 << ":"
		<< std::setfill('0') << std::setw(2) << std::fixed << _seconds % 60;
	TextBox textTime( txtTime.str(), textHeight, 2 );
	glTranslatef( xMargin, 4.0 * yMargin + 3.0 * textHeight, 0.0 );
    textTime.draw();
    glPopMatrix();

	// Draw text that shows health
    glPushMatrix();
	std::stringstream txtHealth; txtHealth << "Health: " << _health << "/" << MAX_HEALTH;
	TextBox textHealth( txtHealth.str(), textHeight, 2 );
	glTranslatef( xMargin, 3.0 * yMargin + 2.0 * textHeight, 0.0 );
    textHealth.draw();
    glPopMatrix();

	// Draw text that shows kills
    glPushMatrix();
	std::stringstream txtKills; txtKills << "Kills: " << _kills << "/" << _killsToWin;
	TextBox textKills( txtKills.str(), textHeight, 2 );
	glTranslatef( xMargin, 2.0 * yMargin + textHeight, 0.0 );
    textKills.draw();
    glPopMatrix();

	// Draw text that shows deaths
    glPushMatrix();
	std::stringstream txtDeaths; txtDeaths << "Deaths: " << _deaths << "/" << _deathsToLose;
	TextBox textDeaths( txtDeaths.str(), textHeight, 2 );
	glTranslatef( xMargin, yMargin, 0.0 );
    textDeaths.draw();
    glPopMatrix();

    glPopMatrix();
}


GRAPHICSLAB_NAMESPACE_END


