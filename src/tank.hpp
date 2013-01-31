/**
 * tank.hpp
 *
 * Author   :   Fatih Erol
 * Date     :   21.02.2011
 * Adapted  :   Fabian Gehri, Simon Schwegler
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_TANK_HPP
#define GRAPHICSLAB_TANK_HPP

// Base class include
#include "drawable.hpp"

// Includes
#include "types.hpp"
#include "CollisionModel.h"
#include "sound.hpp"
#include "config.hpp"
#include "bullet.hpp"
#include <iostream>
#include <vector>

GRAPHICSLAB_NAMESPACE_BEGIN

enum TankState
{
    TANK_LIVING,
    TANK_DYING,
    TANK_DEAD
};

// the current weapon type
enum Weapon
{
    WEAPON_MG,
    WEAPON_GRENADE
};

/** Tank */
class Tank : public Drawable
{
public:
    /** Constructor */
    Tank();

    /** Destructor */
    ~Tank();

    /** Draw the object to the screen. */
    virtual void draw() const;
	virtual void activateMaterial() const;
	virtual void activateCrawlerMaterial() const;

	// set bullets vector
	typedef std::vector<Bullet> BulletVector;
	void setBullets(BulletVector* bullets) { _bullets = bullets; }

    /** Reset the tank parameters */
    virtual void reset();

    float getElevation() const { return _elevation; }
    //void setElevation( float elevation );

    float getAzimuth() const { return _azimuth; }
    //void setAzimuth( float azimuth );

    float getPower() const { return _power; }
    //void setPower( float power );

    /**
     * collision model stuff
     */
    double* get2DVertex(int n) const;
    CollisionModel *getCollisionModel(){ return &_collmod; }
    void updateCollisionModel();

    /**
     * Return the coordinates of the tip of the cannon muzzle
     *
     * @return      Coordinates of the tip of the cannon muzzle
     */
    Point getMuzzlePosition() const;
	Point getMGPosition() const;

	float getHealth() const { return _health; }
	void setHealth( float health );

	const Point &getPosition() const { return _position; }
	void setPosition( const Point &position ) { _position = position; if (_hasSoundSource) Sound::moveStaticSource(_soundSource, position); }

	float getAngle() const { return _angle; }
	void setAngle( float angle ) { _angle = angle; }

	const Vector3D &getDirection() const;

    const Vector3D &getVelocity() const;
	float getSpeed() const { return _speed; }
	void setSpeed( float speed ) { _speed = speed; }

    float getAngularSpeed() const {return _angularSpeed;}
    void setAngularSpeed(float angularSpeed) {_angularSpeed = angularSpeed;}

	void setAccelerating(bool b) { if (_isAccelerating != b) { _isAccelerating = b; if (_hasSoundSource) Sound::setEngineRunning(_soundSource, _isAccelerating); } }
	void setDecelerating(bool b) { _isDecelerating = b; }
	void setAcceleratingLeft(bool b) { _isAcceleratingLeft = b; }
	void setAcceleratingRight(bool b) { _isAcceleratingRight = b; }
	void setIncreasingElevation(bool b) { _isIncreasingElevation = b; }
	void setDecreasingElevation(bool b) { _isDecreasingElevation = b; }
	void setIncreasingAzimuth(bool b) { _isIncreasingAzimuth = b; }
	void setDecreasingAzimuth(bool b) { _isDecreasingAzimuth = b; }
	void setIncreasingPower(bool b) { _isIncreasingPower = b; }
	void setDecreasingPower(bool b) { _isDecreasingPower = b; }

	void setShootingMG(bool b) { _isShootingMG = b; }
	void setShootingGrenade(bool b) { _isShootingGrenade = b; }

    void setState(TankState s) {_state = s;}
    TankState getState() const {return _state;}

    //void setWeapon(Weapon w);
    //Weapon getWeapon() const {return _weapon;}
    //void setNextWeapon();
    void enableGrenadeLauncher() {_hasGrenadeLauncherEnabled = true;}
    void disableGrenadeLauncher() {_hasGrenadeLauncherEnabled = false;}

    IntVector getFields() const { return _fields; }
    void printFields();
    void drawFields();

	void update( float timeSinceLastUpdate );

protected:
	// vector with all bullets in scene
    BulletVector* _bullets;

    // Angle of the muzzle from y=0 plane
    float _elevation;
    // Angle of the muzzle from z=0 plane
    float _azimuth;
    // Projectile throwing power of the cannon
    float _power;

	// tank's health points
	float _health;

	// tank's position, orientation & velocity
	Point _position;
	float _angle;
	float _speed;
	float _angularSpeed;

	// tank's initial position & velocity
	Point _initialPosition;
	float _initialAngle;

	// firing state for grenades
	enum FiringState
    {
        READY,
        FIRING
    } _firingStateMG;
    float _firingTimerMG;
    //float _firingDelay;
	FiringState _firingStateGrenade;
	float _firingTimerGrenade;

	// indices of fields the tank is possibly standing on
	IntVector _fields;
	void findFields();

	// Tank surface material
    static const float _materialAmbient[3];
    static const float _materialDiffuse[3];
    static const float _materialSpecular[3];
    static const float _materialEmission[3];
    static const int _shininess;
	static const float _crawlerMaterialAmbient[3];
    static const float _crawlerMaterialDiffuse[3];
    static const float _crawlerMaterialSpecular[3];
    static const float _crawlerMaterialEmission[3];
    static const int _crawlerShininess;

	// geometric parameters
	static const float _cubeLength;
    static const float _sphereRadius;
    static const float _cylinderHeight;
    static const float _cylinderRadius;

    // Tank collision model
    CollisionModel _collmod;

    TankState _state;
    //Weapon _weapon;
    bool _hasGrenadeLauncherEnabled;

	// sound source for tank's engine
	ALuint _soundSource;
	bool _hasSoundSource; // whether a sound source has already been created (i.e. _soundSource really holds a valid source)

private:
	// current key state of tank
	bool _isAccelerating;
	bool _isDecelerating;
	bool _isAcceleratingLeft;
	bool _isAcceleratingRight;
	bool _isIncreasingElevation;
	bool _isDecreasingElevation;
	bool _isIncreasingAzimuth;
	bool _isDecreasingAzimuth;
	bool _isIncreasingPower;
	bool _isDecreasingPower;

	bool _isShootingMG;
	bool _isShootingGrenade;

	void accelerate();
	void decelerate();
	void accelerateLeft();
	void accelerateRight();
	void increaseElevation();
	void decreaseElevation();
	void increaseAzimuth();
	void decreaseAzimuth();
	void increasePower();
	void decreasePower();

	// creates and adds a new bullet to the _bullets vector
	void shootMG();
	void shootGrenade();

}; // class Tank

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_TANK_HPP


