/**
 * scene.hpp
 *
 * Author   :   Fatih Erol
 * Date     :   16.02.2011
 * Adapted  :   Fabian Gehri
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_SCENE_HPP
#define GRAPHICSLAB_SCENE_HPP

// Includes
#include "defs.hpp"
#include "parameters.hpp"
#include "gl_includes.hpp"
#include "time.hpp"
#include "tank.hpp"
#include "bullet.hpp"
#include "powerBar.hpp"
#include "elevationBar.hpp"
#include "azimuthBar.hpp"
#include "gameStateInfo.hpp"
#include "enemy.hpp"
#include "landscape.hpp"
#include "particleSystem.hpp"

#include <vector>


GRAPHICSLAB_NAMESPACE_BEGIN

/** Class that contains contents of the screen to display */
class Scene
{
public:
    /** Constructor. */
    Scene();

    /** Destructor */
    ~Scene();

    /** Initialize the scene data */
    void initialize();

    /** Reset scene data */
    void reset(bool nextLevel);

    // EVENT HANDLERS
    void onPaint();
    void onResize( int width, int height );
    void onKey( unsigned char key );
	void onKeyUp( unsigned char key );
    void onSpecialKey( int key );
	void onSpecialKeyUp( int key );
    void onMouseEntry( int state );
    void onMouseClick( int button, int state, int x, int y );
    void onMouseMove( int x, int y );
    void onMousePassiveMove( int x, int y );
    void onVisible( int state );
    void onTimer( int value );
    void onIdle();

	static bool RectCollidesWithFields(CollisionModel *cm, IntVector fields);
private:
    // Draw parts of the scene
    void drawPlane();
    void drawScene();
    void drawOverlay();

    // Modify data
    void updateScene();

    // Resolve all collisions
    void collisionCheck(Point *enemies_pos_old, double *enemies_angle_old,
                            Point player_pos_old, double player_angle_old );

    void drawBuildingFields();          // draw the fields that are occupied by a building
    int findFreeField();                // find a field, whose neighbours do not overlap
                                        // with any building sites
    IntVector fieldsCollideWithBuilding(IntVector v);   // returns map-indices of occupied
                                                        // building fields
    Time _timer;            // Timer

private:
    void findBuildingFields();          // find the fields occupied by a building

private:
    bool _fullScreen;       // Whether fullscreen mode is on
    int _posX, _posY;       // Position of the window
    int _width, _height;    // Width and height of the window
	bool _frozen;

    int _mouseX, _mouseY;   // Last mouse position
    bool _rotating;         // Whether in rotation mode
    float _rotationX, _rotationY;   // Rotation angles

	bool _freeCamera;		// whether in free camera mode
	float _FCdist;			//distance of free cam
	float _FCx;				//x center of free cam
	float _FCz;				//x center of free cam

    bool _firstUpdate;

    // Scene data
    Tank _playerTank;
	Enemy* _enemies;
    typedef std::vector<Bullet> BulletVector;
    BulletVector _bullets;
    typedef std::vector<ParticleSystem> PSysVector;
    PSysVector _psystems;

	/*
	static const int _xSlices = 100;
	static const int _zSlices = 100;
	static const int _planeX = 20;
	static const int _planeZ = 20;
	*/
	int _xSlices;
	int _zSlices;
	int _planeX;
	int _planeZ;

	// overlay elements
    PowerBar _powerBar;
    ElevationBar _elevationBar;
    AzimuthBar _azimuthBar;
	GameStateInfo _gameStateInfo;

	// the map
	Landscape _landscape;
    IntVector _buildingFields;

	// game state
	float _secondsPlayed;
	short _nrKills;
	short _nrDeaths;
	short _levelNr;

	// level info
	static const short _nrEnemies[NR_LEVELS];
	static const short _nrKillsToWin[NR_LEVELS];
	static const short _nrDeathsToLose[NR_LEVELS];
	short getNrEnemies() { return _nrEnemies[_levelNr - 1]; }
	short getNrKillsToWin() { return _nrKillsToWin[_levelNr - 1]; }
	short getNrDeathsToLose() { return _nrDeathsToLose[_levelNr - 1]; }

	// performance measuring
	float _updateTime;

}; // class Scene

GRAPHICSLAB_NAMESPACE_END

#endif  //  GRAPHICSLAB_SCENE_HPP


