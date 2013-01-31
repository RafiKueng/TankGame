/**
 * scene.cpp
 *
 * Author   :   Fatih Erol
 * Date     :   16.02.2011
 * Adapted  :   Simon Schwegler, Rafael Kueng, Fabian Gehri
 * All rights reserved.
 */

// Class declaration include
#include "scene.hpp"

// Includes
#include "config.hpp"

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <conio.h>

#include "landscape.hpp"
#include "sound.hpp"



GRAPHICSLAB_NAMESPACE_BEGIN

const short Scene::_nrEnemies[NR_LEVELS]		=	{1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
const short Scene::_nrKillsToWin[NR_LEVELS]		=	{1, 2, 2, 4, 3, 5, 5, 8, 8, 10};
const short Scene::_nrDeathsToLose[NR_LEVELS]	=	{5, 5, 5, 5, 5, 5, 5, 5, 5, 5};

void drawField(double x1, double z1, double x2, double z2);

Scene::Scene():
    _fullScreen( false ),
    _posX( -1 ), _posY( -1 ),
    _width( 1024 ), _height( 768 ),
	_frozen( false ),
    _rotating( false ),
    _rotationX( 0.0 ), _rotationY( 0.0 ),
	_freeCamera(false),
    _FCdist(-5.0), _FCx(0.0), _FCz(0),
	_firstUpdate( true ),
	_secondsPlayed( 0.0f ),
	_nrKills( 0 ),
	_nrDeaths( 0 ),
	_levelNr(1)
{
	_playerTank.setBullets(&_bullets);

	_enemies = new Enemy[MAX_NR_ENEMIES];
	for (int i = 0; i < MAX_NR_ENEMIES; i++)
		_enemies[i].setBullets(&_bullets);
}


Scene::~Scene()
{
	delete[] _enemies;
}

void Scene::initialize()
{
    // Initialize lights
    float lightAmbient[3] = { 0.4, 0.4, 0.4 };
    float lightDiffuse[3] = {  0.5, 0.5, 0.5 };
    float lightSpecular[3] = {  0.5, 0.5, 0.5 };
    float light1Position[4] = { -1.0, 1.0, 1.0, 1.0 };
    float light2Position[4] = {  1.0, 1.0, -1.0, 1.0 };

    glLightfv( GL_LIGHT0, GL_POSITION, light1Position );
    glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbient );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, lightSpecular );

    glLightfv( GL_LIGHT1, GL_POSITION, light2Position );
    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_SPECULAR, lightSpecular );

    glEnable( GL_LIGHT0 );
    glEnable( GL_LIGHT1 );

    // Anti-aliasing
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_POLYGON_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    glEnable( GL_NORMALIZE );

	// Set global ambient light present in the scene
	float globalAmbientArray[4] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, globalAmbientArray );

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	// Set clear color for the buffer
	glClearColor( 0.0, 0.0, 0.0, 0.0 );

	// r: init terrain data
	// (for backwards compability.. _xSlices ect could be replaced and LANDSCAPE_GRID_SIYE directly used)
	_xSlices = FIELD_SIZE;
	_zSlices = FIELD_SIZE;
	_planeX = FIELD_SIZE;
	_planeZ = FIELD_SIZE;

    reset(false);
}

void Scene::reset(bool nextLevel)
{
	if (nextLevel && _levelNr < NR_LEVELS)
		_levelNr++;

	_frozen = false;
	int* coord;
    float offset = float(FIELD_SIZE)/2.0 - 0.5;

    //_rotating = false;
    //_rotationX = _rotationY = 0.0;

	//_freeCamera = false;

	_landscape.setLevelNr(_levelNr);
	_landscape.reset();

	// init player
	_playerTank.reset();
	coord = _landscape.getFreeLocation();
    //_playerTank.setPosition(Point(-1.0, 0.0, 0.0));
    _playerTank.setPosition(Point(coord[0]-offset, 0.0, coord[1]-offset));


	for (int i = 0; i < getNrEnemies(); i++)
	{
		_enemies[i].reset();
		coord = _landscape.getFreeLocation();
		_enemies[i].setPosition(Point(coord[0] - offset, 0.0, coord[1] - offset));
	}

    _bullets.clear();

    _firstUpdate = true;

	_nrKills = 0;
	_nrDeaths = 0;

	_secondsPlayed = 0.0f;

	findBuildingFields();       // must be called before reset
                                // because reset uses the generated
                                // data to place the enemy-tanks

    // reset weapon states
    _playerTank.disableGrenadeLauncher();
    //_playerTank.setWeapon(WEAPON_MG);
    for(int i = 0; i < getNrEnemies(); i++)
    {
        _enemies[i].disableGrenadeLauncher();
        //_enemies[i].setWeapon(WEAPON_MG);
    }



}

void Scene::onPaint()
{
    // Update data in the scene
	if ( !_frozen )
		updateScene();

    // Clear the screen
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

    // Set camera parameters
    int width = glutGet( GLUT_WINDOW_WIDTH );
    int height = glutGet( GLUT_WINDOW_HEIGHT );
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 90, width/height, 1.0, 2048.0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	if (_freeCamera)
	{
		gluLookAt( _FCdist, 3.0, 0.0, _FCx, 0.0, _FCz, 0.0, 1.0, 0.0 );

		glRotatef( _rotationX, 0.0, 0.0, 1.0 );
	    glRotatef( _rotationY, 0.0, 1.0, 0.0 );
    }
	else
	{
		Point position = _playerTank.getPosition();
		Vector3D direction = _playerTank.getDirection();
		float offsetBack = 5.0f;
		float offsetUp = 3.0f;
		float eye[3] = { position.x - offsetBack * direction.x, position.y + offsetUp, position.z - offsetBack * direction.z };
		gluLookAt( eye[0], eye[1], eye[2], position.x, position.y, position.z, 0.0f, 1.0f, 0.0f );

		// set values for sound listener
		float ear[3] = { position.x, position.y, position.z };
		float orientation[6] = { position.x - eye[0], position.y - eye[1], position.z - eye[2], 0.0f, 1.0f, 0.0f };
		Sound::setListener(ear, orientation);
	}

    // Draw screen
	_landscape.draw(); //make sure _landscape.settimesincelastupdate(f) has been called first)
    //drawPlane();
    drawScene();

    drawOverlay();

    glutSwapBuffers();
}


void Scene::updateScene()
{
	if ( _firstUpdate )
    {
        _firstUpdate = false;
		_timer.start();
		return;
    }

	float timeSinceLastUpdate = _timer.getMilliseconds()/1000;
	_secondsPlayed += timeSinceLastUpdate;
	_updateTime = timeSinceLastUpdate;
	_landscape.setTimeSinceLastUpdate(timeSinceLastUpdate);
	_timer.start();

	if (_nrKills >= getNrKillsToWin())
	{
		reset(true);
		return;
	}
	else if (_nrDeaths >= getNrDeathsToLose())
	{
		reset(false);
		return;
	}

    /**
    // save old configuration of enemies and playertank
    // (this will be used for the collision handling below)
    */
    Point *enemies_pos_old = new Point[getNrEnemies()];
    double *enemies_angle_old = new double[getNrEnemies()];
    Point player_pos_old = _playerTank.getPosition();
    double player_angle_old = _playerTank.getAngle();
    for(int i = 0; i < getNrEnemies(); i++)
    {
        enemies_pos_old[i] = _enemies[i].getPosition();
        enemies_angle_old[i] = _enemies[i].getAngle();
    }

	// let enemies choose their next action
	for (int i = 0; i < getNrEnemies(); i++)
	{
		_enemies[i].updateAction(&_landscape, _playerTank.getPosition());
	}

    /**
    // Update
    */
	// update tanks
	_playerTank.update(timeSinceLastUpdate);
	for (int i = 0; i < getNrEnemies(); i++)
	{
		_enemies[i].update(timeSinceLastUpdate);
		//_enemies[i].setSpeed(3.0);
	}
	// update bullets
    for ( BulletVector::iterator bulletIter = _bullets.begin();
         bulletIter != _bullets.end(); bulletIter++)
    {
        Bullet &bullet = *bulletIter;
        bullet.update(timeSinceLastUpdate, _xSlices, _zSlices, _planeX, _planeZ);
    }
	// check player tank if it is dead
	if(_playerTank.getState() == TANK_DYING)
	{
		ParticleSystem p;
        p.setPosition(Point(_playerTank.getPosition().x, _playerTank.getPosition().y + 1.0, _playerTank.getPosition().z));
        p.setVelRange(10.0);
        p.setLifetime(3.0);
        p.setRGB(0.5, 0.2, 0.0);
        _psystems.push_back(p);
        _playerTank.setState(TANK_DEAD);

		_nrDeaths++;
		_playerTank.reset();
	}
    // check enemies if they are dead
	for (int i = 0; i < getNrEnemies(); i++)
	{
		if(_enemies[i].getState() == TANK_DYING)
		{
		    ParticleSystem p;
            p.setPosition(Point(_enemies[i].getPosition().x, _enemies[i].getPosition().y + 1.0, _enemies[i].getPosition().z));
            p.setVelRange(10.0);
            p.setLifetime(3.0);
            p.setRGB(0.5, 0.2, 0.0);
            _psystems.push_back(p);
            _enemies[i].setState(TANK_DEAD);

			_nrKills++;
			_enemies[i].reset();
		}
	}
	//update particle systems
    //psystem.setPosition(Point(_playerTank.getPosition().x, _playerTank.getPosition().y + 1.0, _playerTank.getPosition().z));
	for (unsigned int i = 0; i < _psystems.size(); i++)
		_psystems[i].update(timeSinceLastUpdate);

    /**
    // Deletion and Death animation
    */
    // bullets
    for(unsigned int i = 0; i < _bullets.size(); i++)
    {
        if(_bullets[i].getState() == BULLET_DELETE)
        {
            ParticleSystem p;
            p.setPosition(Point(_bullets[i].getPosition().x, _bullets[i].getPosition().y + 0.2, _bullets[i].getPosition().z));
            p.setVelRange(5.0);
            p.setLifetime(3.0);
            p.setPeriod(5.0);
            p.setRGB(0.1, 0.1, 0.1);
            if(_bullets[i].getType() == BULLET_MG)
                p.setNParticles(3);
            _psystems.push_back(p);
            _bullets.erase(_bullets.begin()+i);
        }
    }
    // particle systems
    for(unsigned int i = 0; i < _psystems.size(); i++)
    {
        if(_psystems[i].getState() == PSYSTEM_DEAD)
        {
            _psystems.erase(_psystems.begin()+i);
        }
    }

    /**
    // Collision Check
    */
    collisionCheck(enemies_pos_old, enemies_angle_old, player_pos_old, player_angle_old);

    /**
    // Cleanup
    */
    delete [] enemies_pos_old;
    delete [] enemies_angle_old;
}

int Scene::findFreeField()
{
    TerrainType **map = _landscape.getMap();
    int xmax = FIELD_SIZE - 1;
    int zmax = FIELD_SIZE - 1;
    for(int i = 0; i < MAX_INSERT_TRIALS; i++)
    {
        int x = rand()%FIELD_SIZE;
        int z = rand()%FIELD_SIZE;

        // (-1, 0)
        if(x > 0)
        {
            if(map[x-1][z] == BUILDING)
                continue;
        }
        // (+1, 0)
        if(x < xmax)
        {
            if(map[x+1][z] == BUILDING)
                continue;
        }
        // (0, -1)
        if(z > 0)
        {
            if(map[x][z-1] == BUILDING)
                continue;
        }
        // (0, +1)
        if(z < zmax)
        {
            if(map[x][z+1] == BUILDING)
                continue;
        }
        // (-1, -1)
        if(x > 0 && z > 0)
        {
            if(map[x-1][z-1] == BUILDING)
                continue;
        }
        // (+1, -1)
        if(x < xmax && z > 0)
        {
            if(map[x+1][z-1] == BUILDING)
                continue;
        }
        // (-1, +1)
        if(x > 0 && z < zmax)
        {
            if(map[x-1][z+1] == BUILDING)
                continue;
        }
        // (+1, +1)
        if(x < xmax && z < zmax)
        {
            if(map[x+1][z+1] == BUILDING)
                continue;
        }

        // found a valid field
        int idx = z*FIELD_SIZE + x;
        return idx;
    }

    /** TODO: throw an exception insted, this will most likely
    // cause the program to crash! */
    std::cout << "warning, could not insert enemy" << std::endl;
}

// check whether the two field-vectors v1 and v2 have an overlap
bool fieldsCollide(IntVector v1, IntVector v2)
{
    for(unsigned int i = 0; i < v1.size(); i++)
    {
        int idx1 = v1[i];
        for(unsigned int j = 0; j < v2.size(); j++)
        {
            int idx2 = v2[j];
            if(idx1 == idx2)
            {
                return true;
            }
        }
    }

    return false;
}

// check whether the field vector v collides with any building fields
// returns indices of occupied building fields
IntVector Scene::fieldsCollideWithBuilding(IntVector v)
{
    IntVector colliding_fields;
    colliding_fields.clear();
    TerrainType **map = _landscape.getMap();
    int xmax = FIELD_SIZE - 1;
    int zmax = FIELD_SIZE - 1;
    int s = FIELD_SIZE;

    for(unsigned int i= 0; i < v.size(); i++)
    {
        int idx = v[i];
        int x = (idx % FIELD_SIZE);
        int z = int (floor( float(idx)/float(FIELD_SIZE)) );

        if(0 <= x && x < xmax && 0 <= z && z < zmax && map[x][z] == BUILDING)
        {
            colliding_fields.push_back(idx);
        }
    }

    return colliding_fields;
}

// check whether a rectangle cm collides with the fields
bool Scene::RectCollidesWithFields(CollisionModel *cm, IntVector fields)
{
    // read out data

    for(unsigned int i= 0; i < fields.size(); i++)
    {
        int idx = fields[i];
        float x = (idx % FIELD_SIZE);
        float z = float (floor( float(idx)/float(FIELD_SIZE)) );
        float offset = float(FIELD_SIZE)/2.0;

        // create a rectangle model for the current field
        CollisionModel field_cm(COLL_RECTANGLE);
        field_cm.setVertex(0, x - offset, z - offset);
        field_cm.setVertex(1, x - offset, z - offset + 1.0);
        field_cm.setVertex(2, x - offset + 1.0, z - offset + 1.0);
        field_cm.setVertex(3, x - offset + 1.0, z - offset);
        field_cm.setYBottom(0.0);
        field_cm.setYTop(50.0);      /** TODO: this should be read from
                                    // the landscape data structure */

        // check for collisions between rectangle and field
        bool res = cm->bCollision(&field_cm);
        if(res == true)
        {
            return true;
        }
    }

    return false;
}

void Scene::collisionCheck(Point *enemies_pos_old, double *enemies_angle_old,
                            Point player_pos_old, double player_angle_old )
{
    // playertank - enemy
    CollisionModel *cm1 = _playerTank.getCollisionModel();
    IntVector v1 = _playerTank.getFields();
    for(int i = 0; i < getNrEnemies(); i++)
    {
        // coarse check
        IntVector v2 = _enemies[i].getFields();
        if(!fieldsCollide(v1, v2))
        {
            continue;
        }

        // fine check
        CollisionModel *cm2 = _enemies[i].getCollisionModel();
        if(cm1->bCollision(cm2))
        {
            // collision sound
            Sound::playCollision(_enemies[i].getPosition());

            // recover old configurations
            _playerTank.setPosition(player_pos_old);
            _playerTank.setAngle(player_angle_old);
            _enemies[i].setPosition(enemies_pos_old[i]);
            _enemies[i].setAngle(enemies_angle_old[i]);
            _playerTank.updateCollisionModel();
            _enemies[i].updateCollisionModel();
            // swap velocities
            float temp = _playerTank.getSpeed();
            _playerTank.setSpeed(-_enemies[i].getSpeed());
            _enemies[i].setSpeed(-temp);
            // do a little bit of deleceration due to collisional friction
            _playerTank.setSpeed(_playerTank.getSpeed()*COLLISION_DECELERATION_COEFF);
            _playerTank.setAngularSpeed(_playerTank.getAngularSpeed()*COLLISION_DECELERATION_COEFF);
            _enemies[i].setSpeed(_enemies[i].getSpeed()*COLLISION_DECELERATION_COEFF);
            _enemies[i].setAngularSpeed(_enemies[i].getAngularSpeed()*COLLISION_DECELERATION_COEFF);
            // do check of full list again, due to possible collision cascade
            //i = 0;
        }
    }

    // enemy - enemy
    for(int i = 0; i < getNrEnemies(); i++)
    {
        // coarse check
        CollisionModel *cm1 = _enemies[i].getCollisionModel();
        IntVector v1 = _enemies[i].getFields();
        for(int j = i+1; j < getNrEnemies(); j++)
        {
            IntVector v2 = _enemies[j].getFields();
            if(!fieldsCollide(v1, v2))
            {
                continue;
            }

            // fine check
            CollisionModel *cm2 = _enemies[j].getCollisionModel();
            if(cm1->bCollision(cm2))
            {
                // collision sound
                Sound::playCollision(_enemies[i].getPosition());
                Sound::playCollision(_enemies[j].getPosition());

                // recover old configurations
                _enemies[i].setPosition(enemies_pos_old[i]);
                _enemies[i].setAngle(enemies_angle_old[i]);
                _enemies[j].setPosition(enemies_pos_old[j]);
                _enemies[j].setAngle(enemies_angle_old[j]);
                _enemies[i].updateCollisionModel();
                _enemies[j].updateCollisionModel();
                // swap velocities
                float temp = _enemies[i].getSpeed();
                _enemies[i].setSpeed(-_enemies[j].getSpeed());
                _enemies[j].setSpeed(-temp);
                // do a little bit of deleceration due to collisional friction
                _enemies[i].setSpeed(_enemies[i].getSpeed()*COLLISION_DECELERATION_COEFF);
                _enemies[i].setAngularSpeed(_enemies[i].getAngularSpeed()*COLLISION_DECELERATION_COEFF);
                _enemies[j].setSpeed(_enemies[j].getSpeed()*COLLISION_DECELERATION_COEFF);
                _enemies[j].setAngularSpeed(_enemies[j].getAngularSpeed()*COLLISION_DECELERATION_COEFF);
                // do check of full list again, due to possible collision cascade
                //i = 0;
            }
        }
    }

    // playertank - bullet
    v1 = _playerTank.getFields();
    for(unsigned int i = 0; i < _bullets.size(); i++)
    {
		if (_bullets[i].getState() == BULLET_DELETE)
			continue;

        // coarse check
        IntVector v2 = _bullets[i].getFields();
        if(!fieldsCollide(v1, v2))
        {
            continue;
        }

        // fine check
        CollisionModel *cm2 = _bullets[i].getCollisionModel();
        if(cm1->bCollision(cm2))
        {
			// play explosion sound
			if (_bullets[i].getType() == BULLET_GRENADE)
				Sound::playGrenade(_playerTank.getPosition());
			else if (_bullets[i].getType() == BULLET_MG)
				Sound::playProjectile(_playerTank.getPosition());
            // deal damage to playertank
            int damage = GRENADE_DAMAGE;
            if(_bullets[i].getType() == BULLET_MG)
                damage = MG_DAMAGE;
            _playerTank.setHealth(_playerTank.getHealth() - damage);
            // delete the bullet
            _bullets[i].setState(BULLET_DELETE);
			// generate particle system
            ParticleSystem p;
            p.setPosition(Point(_playerTank.getPosition().x, _playerTank.getPosition().y + 1.0, _playerTank.getPosition().z));
            p.setVelRange(10.0);
            p.setLifetime(3.0);
            p.setRGB(0.07, 0.1, 0.07);
            p.setNParticles(10);
            _psystems.push_back(p);
        }
    }

    // enemy - bullet
    for(int i = 0; i < getNrEnemies(); i++)
    {
        CollisionModel *cm1 = _enemies[i].getCollisionModel();
        v1 = _enemies[i].getFields();
        for(unsigned int j = 0; j < _bullets.size(); j++)
        {
			if (_bullets[j].getState() == BULLET_DELETE)
				continue;

            // coarse check
            IntVector v2 = _bullets[j].getFields();
            if(!fieldsCollide(v1, v2))
            {
                continue;
            }

            // fine check
            CollisionModel *cm2 = _bullets[j].getCollisionModel();
            if(cm1->bCollision(cm2))
            {
				// play explosion sound
				if (_bullets[j].getType() == BULLET_GRENADE)
					Sound::playGrenade(_enemies[i].getPosition());
				else if (_bullets[j].getType() == BULLET_MG)
					Sound::playProjectile(_enemies[i].getPosition());
                // deal damage to enemy
                int damage = GRENADE_DAMAGE;
                if(_bullets[j].getType() == BULLET_MG)
                    damage = MG_DAMAGE;
                _enemies[i].setHealth(_enemies[i].getHealth() - damage);
                // delete the bullet
                _bullets[j].setState(BULLET_DELETE);
                // generate particle system
                ParticleSystem p;
                p.setPosition(Point(_enemies[i].getPosition().x, _enemies[i].getPosition().y + 1.0, _enemies[i].getPosition().z));
                p.setVelRange(10.0);
                p.setLifetime(3.0);
                p.setRGB(0.1, 0.05, 0.05);
                p.setNParticles(10);
                _psystems.push_back(p);
            }
        }
    }

    // building - playertank
    v1 = _playerTank.getFields();
    IntVector coarse_collisions = fieldsCollideWithBuilding(v1);
    if(coarse_collisions.size() != 0)           // coarse check
    {
        if(RectCollidesWithFields(_playerTank.getCollisionModel(), coarse_collisions)) // fine check
        {
            // play a collision sound
            Sound::playCollision(_playerTank.getPosition());
            // recover old configuration
            _playerTank.setPosition(player_pos_old);
            _playerTank.setAngle(player_angle_old);
            _playerTank.updateCollisionModel();
            // reflect velocity
            _playerTank.setSpeed(-_playerTank.getSpeed());
            // do a little bit of deleceration due to collisional friction
            _playerTank.setSpeed(_playerTank.getSpeed()*COLLISION_DECELERATION_COEFF);
            _playerTank.setAngularSpeed(_playerTank.getAngularSpeed()*COLLISION_DECELERATION_COEFF);
        }

    }

    // building - bullet
    for(unsigned int i = 0; i < _bullets.size(); i++)
    {
        // coarse check
        v1 = _bullets[i].getFields();
        IntVector coarse_collisions = fieldsCollideWithBuilding(v1);
        //fine check
        if(coarse_collisions.size() != 0)
        {
            if(RectCollidesWithFields(_bullets[i].getCollisionModel(), coarse_collisions)) // fine check
            {
				// play explosion sound
				if (_bullets[i].getType() == BULLET_GRENADE)
					Sound::playGrenade(_bullets[i].getPosition());
				else if (_bullets[i].getType() == BULLET_MG)
					Sound::playProjectile(_bullets[i].getPosition());
				// delete bullet
                _bullets[i].setState(BULLET_DELETE);
            }
        }
    }

     // building - enemy
    for(int i = 0; i < getNrEnemies(); i++)
    {
        // coarse check
        v1 = _enemies[i].getFields();
        IntVector coarse_collisions = fieldsCollideWithBuilding(v1);
        //fine check
        if(coarse_collisions.size() != 0)
        {
            if(RectCollidesWithFields(_enemies[i].getCollisionModel(), coarse_collisions)) // fine check
            {
                // play a collision sound
                Sound::playCollision(_enemies[i].getPosition());
                // recover old configuration
                _enemies[i].setPosition(enemies_pos_old[i]);
                _enemies[i].setAngle(enemies_angle_old[i]);
                _enemies[i].updateCollisionModel();
				// reflect velocity
				_enemies[i].setSpeed(-_enemies[i].getSpeed());
                // do a little bit of deleceration due to collisional friction
                _enemies[i].setSpeed(_enemies[i].getSpeed()*COLLISION_DECELERATION_COEFF);
                _enemies[i].setAngularSpeed(_enemies[i].getAngularSpeed()*COLLISION_DECELERATION_COEFF);
            }
        }
    }

    // powerup - playertank
    std::vector<PowerUp*> powerups = _landscape.getPowerupVector();
    float x = _playerTank.getPosition().x;
    float z = _playerTank.getPosition().z;
    int tank_x_idx = int(floor(x));
    int tank_z_idx = int(floor(z));
    for(unsigned int i = 0; i < powerups.size(); i++)
    {
        int *loc = powerups[i]->getLocation();
        int pu_x_idx = loc[0] - FIELD_SIZE/2.0;
        int pu_z_idx = loc[1] - FIELD_SIZE/2.0;
        if(tank_x_idx == pu_x_idx && tank_z_idx == pu_z_idx)
        {
            if( powerups[i]->getType() == POWERUP_GRENADE)
            {
                _playerTank.enableGrenadeLauncher();
            }
            else
				_playerTank.setHealth(_playerTank.getHealth() + POWERUP_HEALTH_BONUS);

            /** TODO: uncomment this function when it is fixed */
            _landscape.powerupPickedUp(loc);
        }
    }

    // enemies - playertank
    for(int i = 0; i < getNrEnemies(); i++)
    {
        float x = _enemies[i].getPosition().x;
        float z = _enemies[i].getPosition().z;
        int enemy_x_idx = int(floor(x));
        int enemy_z_idx = int(floor(z));
        for(unsigned int j = 0; j < powerups.size(); j++)
        {
            int *loc = powerups[j]->getLocation();
            int pu_x_idx = loc[0] - FIELD_SIZE/2.0;
            int pu_z_idx = loc[1] - FIELD_SIZE/2.0;
            if(enemy_x_idx == pu_x_idx && enemy_z_idx == pu_z_idx)
            {
                if( powerups[j]->getType() == POWERUP_GRENADE)
                {
                    _enemies[i].enableGrenadeLauncher();
                }
                else
                    _enemies[i].setHealth(_enemies[i].getHealth() + POWERUP_HEALTH_BONUS);

                /** TODO: uncomment this function when it is fixed */
                //_landscape.powerupPickedUp(loc);
            }
        }
    }

}

// NOT USED ANYMORE, moved to landscape.cpp
void Scene::drawPlane()
{
	/*
    glEnable( GL_DEPTH_TEST );
    glDisable( GL_BLEND );
	glDisable( GL_LIGHTING );

    glPushMatrix();

	float yHover = 0.01;

	glBegin( GL_QUADS );
	for ( int x = 0; x < _xSlices; x++ )
	{
		for ( int z = 0; z < _zSlices; z++ )
		{
			glColor3f( 0.2, 0.2, 0.2 );
            //glColor3f( 0.2 + 0.8*_damage[x][z], 0.3, 0.6 );
            glVertex3f( -_planeX/2.0 + 1.0*x*_planeX/_xSlices, yHover, -_planeZ/2.0 + 1.0*z*_planeZ/_zSlices );

            //glColor3f( 0.2 + 0.8*_damage[x][z+1], 0.3, 0.6 );
            glVertex3f( -_planeX/2.0 + 1.0*x*_planeX/_xSlices, yHover, -_planeZ/2.0 + 1.0*(z+1)*_planeZ/_zSlices );

            //glColor3f( 0.2 + 0.8*_damage[x+1][z+1], 0.3, 0.6 );
            glVertex3f( -_planeX/2.0 + 1.0*(x+1)*_planeX/_xSlices, yHover, -_planeZ/2.0 + 1.0*(z+1)*_planeZ/_zSlices );

            //glColor3f( 0.2 + 0.8*_damage[x+1][z], 0.3, 0.6 );
            glVertex3f( -_planeX/2.0 + 1.0*(x+1)*_planeX/_xSlices, yHover, -_planeZ/2.0 + 1.0*z*_planeZ/_zSlices );
		}
	}
	glEnd();


    glPopMatrix();
    */
}


void Scene::findBuildingFields()
{
    TerrainType** map = _landscape.getMap();
    for(int i = 0; i < FIELD_SIZE; i++)
    {
        for(int j = 0; j < FIELD_SIZE; j++)
        {
            if(map[i][j] != BUILDING)
            {
                continue;
            }

            int x = floorl(i);
            int z = floorl(j);
            int idx = z*FIELD_SIZE + x;
            _buildingFields.push_back(idx);
        }
    }
}

void drawField(double x1, double z1, double x2, double z2)
{
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(x1, 0.5, z1);
    glVertex3f(x2, 0.5, z1);
    glVertex3f(x2, 0.5, z2);
    glVertex3f(x1, 0.5, z2);
    glVertex3f(x1, 0.5, z1);
    glEnd();
    glPopMatrix();
}

void Scene::drawBuildingFields()
{
    /*
    TerrainType** map = _landscape.getMap();
    for(int i = 0; i < FIELD_SIZE; i++)
    {
        for(int j = 0; j < FIELD_SIZE; j++)
        {
            if(map[i][j] != BUILDING)
            {
                continue;
            }

            float x = floor(i);
            float z = floor(j);
            float offset = float(FIELD_SIZE)/2.0;

            glMatrixMode( GL_MODELVIEW );
            glPushMatrix();
            glColor3f(1.0, 0.0, 0.0);
            glBegin(GL_LINE_STRIP);
            glVertex3f(x - offset, 0.4, z - offset);
            glVertex3f(x + 1.0 - offset, 0.4, z - offset);
            glVertex3f(x + 1.0 - offset, 0.4, z + 1.0 - offset);
            glVertex3f(x - offset, 0.4, z + 1.0 - offset);
            glVertex3f(x - offset, 0.4, z - offset);
            glEnd();
            glPopMatrix();
        }
        std::cout << std::endl;
    }
    */

    IntVector buildings = _buildingFields;
    for(unsigned int i = 0; i < _buildingFields.size(); i++)
    {
        int idx = _buildingFields[i];
        float x = (idx % FIELD_SIZE);
        float z = (floor( float(idx)/float(FIELD_SIZE)) );
        float offset = float(FIELD_SIZE)/2.0;

        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINE_STRIP);
        glVertex3f(x - offset, 0.4, z - offset);
        glVertex3f(x + 1.0 - offset, 0.4, z - offset);
        glVertex3f(x + 1.0 - offset, 0.4, z + 1.0 - offset);
        glVertex3f(x - offset, 0.4, z + 1.0 - offset);
        glVertex3f(x - offset, 0.4, z - offset);
        glEnd();
        glPopMatrix();
    }

}



void Scene::drawScene()
{
    glEnable( GL_DEPTH_TEST );
    glDisable( GL_BLEND );
    glEnable( GL_LIGHTING );

    glPushMatrix();

	// draw tanks
    _playerTank.draw();
    if(DEBUG_TANK)
        _playerTank.drawFields();
	for (int i = 0; i < getNrEnemies(); i++)
	{
		_enemies[i].draw();
		if(DEBUG_TANK)
		{
		    _enemies[i].drawFields();
		}
	}

    // draw bullets
    for (unsigned int i = 0; i < _bullets.size(); i++)
	{
		_bullets[i].draw();
		if(DEBUG_TANK)
		{
		    _bullets[i].drawFields();
		}
	}

	// draw grid of fields occupied by a building
    if(DEBUG_TANK)
    {
        drawBuildingFields();
    }

    // draw particle systems
	for (unsigned int i = 0; i < _psystems.size(); i++)
		_psystems[i].draw();



    glPopMatrix();
}

void Scene::drawOverlay()
{
    int width = glutGet( GLUT_WINDOW_WIDTH );
    int height = glutGet( GLUT_WINDOW_HEIGHT ) / 5;

    glViewport( 0, 0, width, height );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, width, 0.0, height );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 );

    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glPushMatrix();

    glColor4f( 0.1, 0.5, 0.1, 0.3 );
    glRectd( 0, 0, width, height );

	glPushMatrix();
	glTranslatef( 1.0 * width / 50.0, 1.0 * height / 50.0, 0.0 );
    _powerBar.setWidth( 4.0 * width / 50.0 );
    _powerBar.setHeight( 48.0 * height / 50.0 );
    _powerBar.setPower( _playerTank.getPower() );
    _powerBar.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef( 6.0 * width / 50.0, 1.0 * height / 50.0, 0.0 );
    _azimuthBar.setWidth( 8.0 * width / 50.0 );
    _azimuthBar.setHeight( 48.0 * height / 50.0 );
    _azimuthBar.setAzimuth( _playerTank.getAzimuth() );
    _azimuthBar.draw();
	glPopMatrix();

	glPushMatrix();
    glTranslatef( 15 * width / 50.0, 1.0 * height / 50.0, 0.0 );
    _elevationBar.setWidth( 8.0 * width / 50.0 );
    _elevationBar.setHeight( 48.0 * height / 50.0 );
    _elevationBar.setElevation( _playerTank.getElevation() );
    _elevationBar.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef( 25.0 * width / 50.0, 1.0 * height / 50.0, 0.0 );
	_gameStateInfo.setWidth( 12.0 * width / 50.0 );
	_gameStateInfo.setHeight( 48.0 * height / 50.0 );
	_gameStateInfo.setTime(_secondsPlayed);
	_gameStateInfo.setHealth(_playerTank.getHealth());
	_gameStateInfo.setLevelNr(_levelNr);
	_gameStateInfo.setKills(_nrKills);
	_gameStateInfo.setDeaths(_nrDeaths);
	_gameStateInfo.setKillsToWin(getNrKillsToWin());
	_gameStateInfo.setDeathsToLose(getNrDeathsToLose());
	_gameStateInfo.setUpdateTime(_updateTime);
	_gameStateInfo.draw();
	glPopMatrix();

    glPopMatrix();
}

void Scene::onResize( int width, int height )
{
    glutPostRedisplay();
}

void Scene::onKey( unsigned char key )
{
    int modifier = glutGetModifiers();
    bool shiftPressed = ( modifier & GLUT_ACTIVE_SHIFT )!=0;
    bool ctrlPressed = ( modifier & GLUT_ACTIVE_CTRL )!=0;
    bool altPressed = ( modifier & GLUT_ACTIVE_ALT )!=0;

    switch ( key )
    {
        case 27: // Escape
        case 'q':
        case 'Q':
        {
            exit( EXIT_SUCCESS );
            break;
        }

        case 'r':
        case 'R':
        {
            reset(false);
            glutPostRedisplay();
            break;
        }

        case 'f':
        case 'F':
        {
            _fullScreen = !_fullScreen;
            if ( _fullScreen )
            {
                // Remember window position and size
                _posX = glutGet( GLUT_WINDOW_X );
                _posY = glutGet( GLUT_WINDOW_Y );
                _width = glutGet( GLUT_WINDOW_WIDTH );
                _height = glutGet( GLUT_WINDOW_HEIGHT );

                // Switch to fullscreen mode
                glutFullScreen();
            }
            else
            {
                // Position and size window to previous values
                glutPositionWindow( _posX, _posY );
                glutReshapeWindow( _width, _height );
            }

            break;
        }

		case 'a':
		case 'A':
        {
            _playerTank.setIncreasingAzimuth(true);
            break;
        }
		case 'd':
		case 'D':
        {
            _playerTank.setDecreasingAzimuth(true);
            break;
        }
        case 'w':
		case 'W':
        {
            _playerTank.setIncreasingElevation(true);
            break;
        }
        case 's':
		case 'S':
        {
            _playerTank.setDecreasingElevation(true);
            break;
        }

        case '+':
        {
            _playerTank.setIncreasingPower(true);
            break;
        }
        case '-':
        {
			_playerTank.setDecreasingPower(true);
            break;
        }
        case ' ':
        {
			_playerTank.setShootingMG(true);
            break;
        }
		case 'x':
		{
			_playerTank.setShootingGrenade(true);
			break;
		}
		case 'z':
		{
			_frozen = !_frozen;
			_firstUpdate = true;
			break;
		}
		case 'c':
			_freeCamera = !_freeCamera;
			break;

		case '.':
			_FCdist*=2.0;
			break;

		case ',':
			_FCdist*=0.5;
			break;

		case '8':
			_FCx+=5;
			break;

		case '2':
			_FCx-=5;
			break;

		case '4':
			_FCz+=5;
			break;

		case '6':
			_FCz-=5;
			break;

		// output maparray around player for debug
		case 'M':
		case 'm':
			{
				int maparoundwidth = 5;
				TerrainType** maparound = _landscape.getMap(_playerTank.getPosition(),maparoundwidth);
				for (int x = 0; x<2*maparoundwidth+1; ++x)
				{
					for (int z = 0; z<2*maparoundwidth+1; ++z)
					{
						std::cout << (int)maparound[x][z];
					}
					std::cout << std::endl;
					delete[] maparound[x];
				}
				std::cout << std::endl;
				delete[] maparound;
			}
			break;
   //     case 'b':
			//{
			//	//_playerTank.enableGrenadeLauncher();
			//	_playerTank.setNextWeapon();
			//	break;
			//}

		default:
            break;
    }
}

void Scene::onKeyUp( unsigned char key )
{
    int modifier = glutGetModifiers();
    bool shiftPressed = ( modifier & GLUT_ACTIVE_SHIFT )!=0;
    bool ctrlPressed = ( modifier & GLUT_ACTIVE_CTRL )!=0;
    bool altPressed = ( modifier & GLUT_ACTIVE_ALT )!=0;

    switch ( key )
    {
		case 'a':
		case 'A':
        {
            _playerTank.setIncreasingAzimuth(false);
            break;
        }
		case 'd':
		case 'D':
        {
            _playerTank.setDecreasingAzimuth(false);
            break;
        }
        case 'w':
		case 'W':
        {
            _playerTank.setIncreasingElevation(false);
            break;
        }
        case 's':
		case 'S':
        {
            _playerTank.setDecreasingElevation(false);
            break;
        }

        case '+':
        {
            _playerTank.setIncreasingPower(false);
            break;
        }
        case '-':
        {
			_playerTank.setDecreasingPower(false);
            break;
        }
        case ' ':
        {
			_playerTank.setShootingMG(false);
            break;
        }
		case 'x':
		{
			_playerTank.setShootingGrenade(false);
			break;
		}
		default:
            break;
    }
}

void Scene::onSpecialKey( int key )
{
    int modifier = glutGetModifiers();
    bool shiftPressed = ( modifier & GLUT_ACTIVE_SHIFT )!=0;
    bool ctrlPressed = ( modifier & GLUT_ACTIVE_CTRL )!=0;
    bool altPressed = ( modifier & GLUT_ACTIVE_ALT )!=0;

    switch ( key )
    {
        case GLUT_KEY_LEFT:
        {
			_playerTank.setAcceleratingLeft(true);
            break;
        }
        case GLUT_KEY_RIGHT:
        {
			_playerTank.setAcceleratingRight(true);
            break;
        }
        case GLUT_KEY_UP:
        {
			_playerTank.setAccelerating(true);
            break;
        }
        case GLUT_KEY_DOWN:
        {
			_playerTank.setDecelerating(true);
            break;
        }
    }
}

void Scene::onSpecialKeyUp( int key )
{
    int modifier = glutGetModifiers();
    bool shiftPressed = ( modifier & GLUT_ACTIVE_SHIFT )!=0;
    bool ctrlPressed = ( modifier & GLUT_ACTIVE_CTRL )!=0;
    bool altPressed = ( modifier & GLUT_ACTIVE_ALT )!=0;

    switch ( key )
    {
        case GLUT_KEY_LEFT:
        {
			_playerTank.setAcceleratingLeft(false);
            break;
        }
        case GLUT_KEY_RIGHT:
        {
			_playerTank.setAcceleratingRight(false);
            break;
        }
        case GLUT_KEY_UP:
        {
			_playerTank.setAccelerating(false);
            break;
        }
        case GLUT_KEY_DOWN:
        {
			_playerTank.setDecelerating(false);
            break;
        }
    }
}

void Scene::onMouseEntry( int state )
{
    if ( state == GLUT_ENTERED )
    {
    }
    else if ( state == GLUT_LEFT )
    {
    }
}

void Scene::onMouseClick( int button, int state, int x, int y )
{
    _rotating = false;
    _mouseX = x;
    _mouseY = y;

    bool pressed = ( state == GLUT_DOWN );
    switch ( button )
    {
        case GLUT_LEFT_BUTTON:
        {
            if ( pressed )
            {
                _rotating = true;
            }
            break;
        }
        case GLUT_MIDDLE_BUTTON:
        {
            break;
        }
        case GLUT_RIGHT_BUTTON:
        {
            break;
        }
    }
}


void Scene::onMouseMove( int x, int y )
{
    int xMove = x - _mouseX;
    int yMove = _mouseY - y;
    int width = glutGet( GLUT_WINDOW_WIDTH );
    int height = glutGet( GLUT_WINDOW_HEIGHT );
    _mouseX = x;
    _mouseY = y;

    if ( _rotating )
    {
        float rotationScale = 360.0;

        _rotationY += rotationScale*xMove/width;
        if ( _rotationY > 360 )
            _rotationY -= 360;
        else if ( _rotationY < 0 )
            _rotationY += 360;
        _rotationX += rotationScale*yMove/height;

        if ( _rotationX > 360 )
            _rotationX -= 360;
        else if ( _rotationX < 0 )
            _rotationX += 360;
    }

	glutPostRedisplay();
}

void Scene::onMousePassiveMove( int x, int y )
{
}

void Scene::onVisible( int state )
{
    if ( state == GLUT_VISIBLE )
        glutPostRedisplay();
}

void Scene::onTimer( int value )
{
	if ( !_frozen )
		glutPostRedisplay();
}

void Scene::onIdle()
{
}


GRAPHICSLAB_NAMESPACE_END



