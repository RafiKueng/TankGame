/**
 * gameStateInfo.hpp
 * 
 * Author   :   Fabian Gehri
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_GAMESTATEINFO_HPP
#define GRAPHICSLAB_GAMESTATEINFO_HPP

// Base class include
#include "drawable.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

/** GameStateInfo */
class GameStateInfo : public Drawable
{
public:
    /** Constructor */
    GameStateInfo();
    
    /** Destructor */
    ~GameStateInfo();
    
    /** Draw the object to the screen. */
    void draw() const;
    
    
    void setWidth( float width );
    void setHeight( float height );
	void setTime( int seconds );
	void setHealth( int health );
	void setLevelNr (short nr);
    void setKills( short kills );
	void setDeaths( short deaths );
	void setKillsToWin( short killsToWin );
	void setDeathsToLose( short deathsToLose );
	void setUpdateTime(float time);
    
private:
    float _width, _height;
	int _seconds, _health;
	short _levelNr;
    short _kills, _deaths;
	short _killsToWin, _deathsToLose;
	float _updateTime;
    
}; // class GameStateInfo

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_GAMESTATEINFO_HPP
