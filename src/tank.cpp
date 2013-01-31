/**
 * tank.cpp
 *
 * Author   :   Fatih Erol
 * Date     :   21.02.2011
 * Adapted  :   Fabian Gehri, Simon Schwegler
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "tank.hpp"

// Includes
#include "config.hpp"
#include "parameters.hpp"
#include "gl_includes.hpp"
#include <iostream>
#include <cmath>

GRAPHICSLAB_NAMESPACE_BEGIN

// Tank surface material
const float Tank::_materialAmbient[3] = { 0.07, 0.1, 0.07 };
const float Tank::_materialDiffuse[3] = { 0.25, 0.3, 0.25 };
const float Tank::_materialSpecular[3] = { 0.45, 0.5, 0.45 };
const float Tank::_materialEmission[3] = { 0.0, 0.0, 0.0 };
const int Tank::_shininess = 24;

// Crawler material
const float Tank::_crawlerMaterialAmbient[3] = { 0.0, 0.0, 0.0 };
const float Tank::_crawlerMaterialDiffuse[3] = { 0.1, 0.1, 0.1 };
const float Tank::_crawlerMaterialSpecular[3] = { 0.1, 0.1, 0.1 };
const float Tank::_crawlerMaterialEmission[3] = { 0.0, 0.0, 0.0 };
const int Tank::_crawlerShininess = 128;

// geometric parameters
const float Tank::_cubeLength = 1.0f;
const float Tank::_sphereRadius = 0.4f;
const float Tank::_cylinderHeight = 1.0f;
const float Tank::_cylinderRadius = 0.1f;

Tank::Tank() :
    Drawable(),
	_initialAngle(0.0f),
	_initialPosition(0.0f, 0.0f, 0.0f, 1.0f),
	_angularSpeed(0.0f),
	_firingStateMG( READY ),
	_firingStateGrenade( READY ),
    _firingTimerMG(0.0),
	_firingTimerGrenade(0.0),
	_collmod(COLL_RECTANGLE),
	_soundSource(0),
	_hasSoundSource(false),
	_state(TANK_LIVING)//,
	//_weapon(WEAPON_MG),
	//_firingDelay(FIRING_DELAY_MG)
{
	reset();
}

Tank::~Tank()
{
}

void Tank::reset()
{
    _elevation = 45.0;
    _azimuth = 0.0;
    _power = 0.5;
	_health = MAX_HEALTH;

	_position = _initialPosition;
	_angle = _initialAngle;
	_speed = 0.0f;
	_angularSpeed = 0.0f;

	setAccelerating(false);
	setDecelerating(false);
	setAcceleratingLeft(false);
	setAcceleratingRight(false);
	setIncreasingElevation(false);
	setDecreasingElevation(false);
	setIncreasingAzimuth(false);
	setDecreasingAzimuth(false);
	setIncreasingPower(false);
	setDecreasingPower(false);

	setShootingMG(false);
	setShootingGrenade(false);

	_firingStateMG = READY;
	_firingStateGrenade = READY;
    _firingTimerMG = 0.0;
	_firingTimerGrenade = 0.0;

	_state = TANK_LIVING;

    // init collision model
    _collmod.setYBottom(0.0);
    _collmod.setYTop(_cubeLength*0.5 + _sphereRadius);

    // init field indices for collision
    findFields();

	if (_hasSoundSource)
		Sound::deleteStaticSource(_soundSource);
	_soundSource = Sound::addEngineSource(_position);
	_hasSoundSource = true;
}

//void Tank::setElevation( float elevation )
//{
//    _elevation = elevation;
//}
//
//void Tank::setAzimuth( float azimuth )
//{
//    _azimuth = azimuth;
//}
//
//void Tank::setPower( float power )
//{
//    _power = power;
//}


Point Tank::getMuzzlePosition() const
{
    Point point;
    /*
    point.x = _position.x + 1.0*std::cos( TO_RADIAN( _elevation ) )*std::cos( TO_RADIAN( _azimuth ) )
                + std::cos( TO_RADIAN( -_angle ) );
    point.y = 0.6 + std::sin( TO_RADIAN( _elevation ) );
    point.z = _position.z - 1.0*std::cos( TO_RADIAN( _elevation ) )*std::sin( TO_RADIAN( _azimuth ) )
                + + std::sin( TO_RADIAN( -_angle ) );
    */
    float muzzle_len = 1.0;                 /** TODO: get the exact value */
    float el = TO_RADIAN( _elevation );
    float az = TO_RADIAN( -_azimuth );
    float ang =  TO_RADIAN( -_angle );
    float x = getPosition().x;
    float y = getPosition().y;
    float z = getPosition().z;

    point.x = x + muzzle_len*cos(el) * ( cos(az + ang));
    point.y = 0.6 + sin( el );
    point.z = z + muzzle_len*cos(el) * ( sin(az + ang));

    point.w = 1.0;

    return point;
}

Point Tank::getMGPosition() const
{
    Point point;
    /*
    point.x = _position.x + 1.0*std::cos( TO_RADIAN( _elevation ) )*std::cos( TO_RADIAN( _azimuth ) )
                + std::cos( TO_RADIAN( -_angle ) );
    point.y = 0.6 + std::sin( TO_RADIAN( _elevation ) );
    point.z = _position.z - 1.0*std::cos( TO_RADIAN( _elevation ) )*std::sin( TO_RADIAN( _azimuth ) )
                + + std::sin( TO_RADIAN( -_angle ) );
    */
    float mg_len = 0.7;                 /** TODO: get the exact value */
    float ang =  TO_RADIAN( -_angle );
    float x = getPosition().x;
    float y = getPosition().y;
    float z = getPosition().z;

    point.x = x + mg_len * cos(ang);
    point.y = 0.5;
    point.z = z + mg_len * sin(ang);

    point.w = 1.0;

    return point;
}

const Vector3D &Tank::getDirection() const
{
	return Vector3D(cos(TO_RADIAN(_angle)), 0.0f, -sin(TO_RADIAN(_angle)));
}

const Vector3D &Tank::getVelocity() const
{
	return Vector3D(_speed * cos(TO_RADIAN(_angle)), 0.0f, -_speed * sin(TO_RADIAN(_angle)));
}

/**
 * Return the coordinate of the n'th vertex on the ground
 *
 * @return      2-array with 2D-coordinates
 */
double* Tank::get2DVertex(int n) const
{
    // compute tanks untransformed vertex coordinates (vertices on ground)
    double _xu[4], _zu[4];
    double xu, zu;
    _xu[0] = -0.5*_cubeLength; _zu[0] = -0.5*_cubeLength;    // UL
    _xu[1] = -0.5*_cubeLength; _zu[1] = +0.5*_cubeLength;    // LL
    _xu[2] = +0.5*_cubeLength; _zu[2] = +0.5*_cubeLength;    // LR
    _xu[3] = +0.5*_cubeLength; _zu[3] = -0.5*_cubeLength;    // UR
    xu = _xu[n];
    zu = _zu[n];

    // rotate
    double x, z;
    double phi = (_angle/360.0)*(2.0*PI);
    x = cos(-phi)*xu - sin(-phi)*zu;
    z = sin(-phi)*xu + cos(-phi)*zu;

    // translate vertices
    x += _position.x;
    z += _position.z;

    // return the value
    double *nv = new double[2];
    nv[0] = x;
    nv[1] = z;
    return nv;
}

void Tank::accelerate()
{
	_speed += ACCELERATION;

	//if (_speed > MAX_SPEED)
	//	_speed = MAX_SPEED;
}

void Tank::decelerate()
{
	_speed -= DECELERATION;

	//if (_speed < MAX_SPEED_REVERSE)
	//	_speed = MAX_SPEED_REVERSE;
}

void Tank::accelerateLeft()
{
    _angularSpeed += TURN_ACCELERATION;
}

void Tank::accelerateRight()
{
	_angularSpeed -= TURN_ACCELERATION;
}

void Tank::increaseElevation()
{
	_elevation = ( _elevation < 80 ) ? ( _elevation + 1 ) : _elevation;
}

void Tank::decreaseElevation()
{
	_elevation = ( _elevation > 0 ) ? ( _elevation - 1 ) : _elevation;
}

void Tank::increaseAzimuth()
{
	_azimuth += 5;
	if ( _azimuth > 180 )
		_azimuth -= 360;
}

void Tank::decreaseAzimuth()
{
	_azimuth -= 5;
	if ( _azimuth < -180 )
		_azimuth += 360;
}

void Tank::increasePower()
{
	_power += 0.02;
    if ( _power > 1.0 )
		_power = 1.0;
}

void Tank::decreasePower()
{
	_power -= 0.02;
    if ( _power <= 0.02 )
		_power = 0.02;
}

void Tank::update( float timeSinceLastUpdate )
{
    if(_state == TANK_DEAD)
    {
        updateCollisionModel();
        return;
    }

    if(_health <= 0)
    {
        _state = TANK_DYING;
    }

	// update firing state and timer
    _firingTimerMG += timeSinceLastUpdate;
	if(_firingTimerMG >= FIRING_DELAY_MG)
    {
        _firingTimerMG = 0.0;
        _firingStateMG = READY;
    }
	_firingTimerGrenade += timeSinceLastUpdate;
	if(_firingTimerGrenade >= FIRING_DELAY_GRENADE)
    {
        _firingTimerGrenade = 0.0;
		_firingStateGrenade = READY;
    }

	// update by key events
	if (_isAccelerating)
		accelerate();
	if (_isDecelerating)
		decelerate();
	if (_isAcceleratingLeft)
		accelerateLeft();
	if (_isAcceleratingRight)
		accelerateRight();
	if (_isIncreasingElevation)
		increaseElevation();
	if (_isDecreasingElevation)
		decreaseElevation();
	if (_isIncreasingAzimuth)
		increaseAzimuth();
	if (_isDecreasingAzimuth)
		decreaseAzimuth();
	if (_isIncreasingPower)
		increasePower();
	if (_isDecreasingPower)
		decreasePower();

	if (_isShootingMG)
		shootMG();
	if (_isShootingGrenade)
		shootGrenade();

	// slow down due to friction
	_speed *= FRICTION_DECELERATION_COEFF;

    /* Angular slow down due to friction */
    _angularSpeed *= FRICTION_TURN_DECELERATION_COEFF;

    /* Linear movement */
	Vector3D velocity = getVelocity();
	setPosition(Point(_position.x + velocity.x * timeSinceLastUpdate, _position.y + velocity.y * timeSinceLastUpdate, _position.z + velocity.z * timeSinceLastUpdate));

    /* Angular movement */
	_angle += _angularSpeed * timeSinceLastUpdate;
	if (_angle > 360.0f)
		_angle -= 360.0f;
	else if (_angle < 0.0f)
		_angle += 360.0f;

    /* update the collision model */
    updateCollisionModel();

    /* update field indices */
    findFields();

}

void Tank::setHealth( float health )
{
    if(_state == TANK_DEAD)
    {
        return;
    }

    _health = health;
    if(_health <= 0)
    {
        _health = 0;
        _state = TANK_DYING;
    }
	else if (_health > MAX_HEALTH)
		_health = MAX_HEALTH;

    if(DEBUG_TANK)
        //std::cout << "health: " << _health <<  std::endl;
        ;
}

void Tank::updateCollisionModel()
{
    // udpate vertices
    for(int i = 0; i < 4; i++)
    {
        double *v = get2DVertex(i);
        _collmod.setVertex(i, v[0], v[1]);
        delete [] v;
    }
}

void Tank::draw() const
{
	activateMaterial();

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();

	GLUquadric *quadric = gluNewQuadric();

	glTranslatef(_position.x, _position.y, _position.z);
	glRotatef(_angle, 0.0f, 1.0f, 0.0f);

	glPushMatrix();
	glTranslatef( 0.0, _cubeLength*3.0/8.0, 0.0 );
	glScalef(1.0f, 0.25f, 1.0f);
    glutSolidCube( _cubeLength );
	glPopMatrix();

	glPushMatrix();
	glTranslatef( 0.0, _cubeLength*3.0/8.0, 0.0 );
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadric, 0.05f, 0.05f, _cubeLength / 2.0f + 0.2f, 20, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef( 0.0, _cubeLength/2.0, 0.0 );
    glutSolidSphere( _sphereRadius, 50, 50 );

    glTranslatef( 0.0, _cylinderRadius, 0.0 );
    glRotatef( _azimuth + 90, 0.0, 1.0, 0.0 );
    glRotatef( _elevation, -1.0, 0.0, 0.0 );

    gluCylinder( quadric, _cylinderRadius, _cylinderRadius, _cylinderHeight, 50, 50 );
    
	glPopMatrix();

	// crawler
	activateCrawlerMaterial();
	glPushMatrix();
	glTranslatef( 0.0f, _cubeLength / 8.0f, -_cubeLength * 4.0f / 8.0f);
	glScalef(4.0f, 1.0f, 1.0f);
	gluCylinder(quadric, _cubeLength / 8.0f, _cubeLength / 8.0f, _cubeLength / 4.0f, 20, 2);
	gluDisk(quadric, 0.0f, _cubeLength / 8.0f, 20, 5);
	glTranslatef(0.0f, 0.0f, _cubeLength / 4.0f);
	gluDisk(quadric, 0.0f, _cubeLength / 8.0f, 20, 5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef( 0.0f, _cubeLength / 8.0f, _cubeLength * 2.0f / 8.0f);
	glScalef(4.0f, 1.0f, 1.0f);
	gluCylinder(quadric, _cubeLength / 8.0f, _cubeLength / 8.0f, _cubeLength / 4.0f, 20, 2);
	gluDisk(quadric, 0.0f, _cubeLength / 8.0f, 20, 5);
	glTranslatef(0.0f, 0.0f, _cubeLength / 4.0f);
	gluDisk(quadric, 0.0f, _cubeLength / 8.0f, 20, 5);
	glPopMatrix();

	gluDeleteQuadric( quadric );

    glPopMatrix();

    // draw the position of the collision box
    if(DEBUG_TANK)
    {
        _collmod.draw();
    }
}

void Tank::activateMaterial() const
{
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, _materialAmbient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, _materialDiffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, _materialSpecular );
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, _materialEmission );
    glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, _shininess );
}

void Tank::activateCrawlerMaterial() const
{
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, _crawlerMaterialAmbient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, _crawlerMaterialDiffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, _crawlerMaterialSpecular );
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, _crawlerMaterialEmission );
    glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, _crawlerShininess );
}

void Tank::findFields()
{
    _fields.clear();

    // field containing center of tank
    float x = float(FIELD_SIZE)/2.0 + _position.x;
    float z = float(FIELD_SIZE)/2.0 + _position.z;
    int x_idx = int(floor(x));
    int z_idx = int(floor(z));
    int idx = z_idx*FIELD_SIZE + x_idx;
    _fields.push_back(idx);

    int xmax = FIELD_SIZE - 1;
    int zmax = FIELD_SIZE - 1;
    int s = FIELD_SIZE;

    // (-1, 0)
    if(x_idx > 0)
    {
        _fields.push_back(idx - 1);
    }
    // (+1, 0)
    if(x_idx < xmax)
    {
        _fields.push_back(idx + 1);
    }
    // (0, -1)
    if(z_idx > 0)
    {
        _fields.push_back(idx - s);
    }
    // (0, +1)
    if(z_idx < zmax)
    {
        _fields.push_back(idx + s);
    }
    // (-1, -1)
    if(x_idx > 0 && z_idx > 0)
    {
        _fields.push_back(idx - s - 1);
    }
    // (+1, -1)
    if(x_idx < xmax && z_idx > 0)
    {
        _fields.push_back(idx - s + 1);
    }
    // (-1, +1)
    if(x_idx > 0 && z_idx < zmax)
    {
        _fields.push_back(idx + s - 1);
    }
    // (+1, +1)
    if(x_idx < xmax && z_idx < zmax)
    {
        _fields.push_back(idx + s + 1);
    }

}

void Tank::printFields()
{
    std::cout << "fields:" << std::endl;
    for(unsigned int i = 0; i < _fields.size(); i++)
    {
        int idx = _fields[i];
        int x = (idx % FIELD_SIZE);
        int z = int (floor( float(idx)/float(FIELD_SIZE)) );
        std::cout << "(" << x << "," << z << ")" << std::endl;
    }
}

void Tank::drawFields()
{
    for(unsigned int i = 0; i < _fields.size(); i++)
    {
        int idx = _fields[i];
        float x = float(idx % FIELD_SIZE);
        float z = floor( float(idx)/float(FIELD_SIZE) );
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

void Tank::shootMG()
{
	if ( _firingStateMG != READY )
        return;

    Bullet bullet;

    Point position = getMGPosition();
    //std::cout << position.x << " " << position.z << std::endl;
    bullet.setPosition( position );

    float velocityScale = 30.0;

	float ang =  TO_RADIAN( _angle );
	Vector3D vel = getVelocity();
    Vector3D velocity(velocityScale * _power * cos(ang) + vel.x, 0.0f, velocityScale * -1 * _power * sin(ang) + vel.z);

    bullet.setVelocity( velocity );

    // set state of bullet
    bullet.setType(BULLET_MG);

	_bullets->push_back( bullet );

    _firingStateMG = FIRING;

	// play sound of machine gun
	Sound::playGun(position);
}

void Tank::shootGrenade()
{
	if ( !_hasGrenadeLauncherEnabled || _firingStateGrenade != READY )
        return;

    Bullet bullet;

    Point position = getMuzzlePosition();
    //std::cout << position.x << " " << position.z << std::endl;
    bullet.setPosition( position );

    float velocityScale = 15.0;

	float el = TO_RADIAN( _elevation );
    float az = TO_RADIAN( _azimuth );
	float ang =  TO_RADIAN( _angle );
	Vector3D vel = getVelocity();
    Vector3D velocity(velocityScale * _power * cos(el) * cos(ang + az) + vel.x, velocityScale * _power * sin(el) + vel.y, velocityScale * -1 * _power * cos(el) * sin(ang + az) + vel.z);

    bullet.setVelocity( velocity );

    // set state of bullet
    bullet.setType(BULLET_GRENADE);

	_bullets->push_back( bullet );

    _firingStateGrenade = FIRING;

	// play sound of grenade launcher
	Sound::playGrenadeLauncher(position);
}

//void Tank::setWeapon(Weapon w)
//{
//
//    if(w == WEAPON_GRENADE && !_hasGrenadeLauncherEnabled)
//    {
//        if(DEBUG_TANK)
//            std::cout << "stayed with mg" << std::endl;
//        return;
//    }
//
//    _weapon = w;
//
//    if(_weapon == WEAPON_GRENADE)
//    {
//        _firingDelay = FIRING_DELAY_GRENADE;
//        if(DEBUG_TANK)
//            std::cout << "changed to grenade launcher" << std::endl;
//    }
//    if(_weapon == WEAPON_MG)
//    {
//        _firingDelay = FIRING_DELAY_MG;
//        if(DEBUG_TANK)
//            std::cout << "changed to mg" << std::endl;
//    }
//}

//void Tank::setNextWeapon()
//{
//    if(_weapon == WEAPON_GRENADE)
//    {
//        setWeapon(WEAPON_MG);
//    }
//    else
//    {
//        setWeapon(WEAPON_GRENADE);
//    }
//}






GRAPHICSLAB_NAMESPACE_END


