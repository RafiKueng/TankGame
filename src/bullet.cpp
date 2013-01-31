/**
 * bullet.cpp
 *
 * Author   :   Fatih Erol
 * Date     :   21.02.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "bullet.hpp"

// Includes
#include <math.h>
#include <iostream>
#include "sound.hpp"
#include "gl_includes.hpp"
#include "config.hpp"
#include "parameters.hpp"
using namespace std;

GRAPHICSLAB_NAMESPACE_BEGIN

Bullet::Bullet( float radius ) :
    Drawable(),
    _radius( radius ),
    _state(BULLET_FLYING),
    _collmod(COLL_RECTANGLE),
    _type(BULLET_GRENADE)
{
    _collmod.setYBottom(0.0);
    _collmod.setYTop(1.0);
    findFields();
    setType(BULLET_GRENADE);
}

Bullet::~Bullet()
{
}

void Bullet::update( float timeSinceLastUpdate, float xSlices, float zSlices, float planeX, float planeZ)
{
    const Point &position = getPosition();
    const Vector3D &velocity = getVelocity();

    // compute new position
    float speed = sqrt( velocity.x*velocity.x + velocity.y*velocity.y + velocity.z*velocity.z );
    if ( speed < 0.1 )
    {
        setVelocity( Vector3D( 0.0, 0.0, 0.0 ) );
    }
    Point newPosition;
    Vector3D newVelocity;
    if(_type == BULLET_GRENADE)
    {
        newPosition = Point( position.x + velocity.x * timeSinceLastUpdate,
                          position.y + velocity.y * timeSinceLastUpdate + 0.5 * GRAVITATIONAL_ACCELARATION * timeSinceLastUpdate * timeSinceLastUpdate,
                          position.z + velocity.z * timeSinceLastUpdate );
        newVelocity = Vector3D( velocity.x, velocity.y + GRAVITATIONAL_ACCELARATION * timeSinceLastUpdate, velocity.z );
    }
    else
    {
        newPosition = Point( position.x + velocity.x * timeSinceLastUpdate,
                          position.y + velocity.y * timeSinceLastUpdate,
                          position.z + velocity.z * timeSinceLastUpdate );
        newVelocity = Vector3D( velocity.x, velocity.y, velocity.z );
    }

    // check if bullet has hit the ground
    if( newPosition.y < getRadius())
    {
		if (_type == BULLET_GRENADE)
			Sound::playGrenade(newPosition);
		else if (_type == BULLET_MG)
			Sound::playProjectile(newPosition);
        _state = BULLET_DELETE;
        return;
    }

    // check if bullet position exceeds map limits
    if ( newPosition.x > planeX / 2.0 || newPosition.x < -planeX / 2.0 ||
        newPosition.z > planeZ / 2.0 || newPosition.z < -planeZ / 2.0 )
    {
        _state = BULLET_DELETE;
        return;
    }

    // update position
    setPosition( newPosition );
    setVelocity( newVelocity );

    // update collision model
    updateCollisionModel();

    // set field indices for coarse collision
    findFields();

}

void Bullet::draw() const
{
    // Bullet surface material
    float materialAmbient[3] = { 0.1, 0.1, 0.1 };
    float materialDiffuse[3] = { 0.3, 0.3, 0.3 };
    float materialSpecular[3] = { 0.5, 0.5, 0.5 };
    float materialEmission[3] = { 0.0, 0.0, 0.0 };
    int shininess = 24;

	if(_type == BULLET_GRENADE)
	{
		materialAmbient[0] = 0.05;
		materialAmbient[2] = 0.05;
	}

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular );
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, materialEmission );
    glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, shininess );

    // draw the actual bullet
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    const Point &position = getPosition();
    glTranslatef( position.x, position.y, position.z );
    float actual_radius = _radius;
    if(_type == BULLET_MG)
        actual_radius = _radius/2.0;
    glutSolidSphere( actual_radius, 50, 50 );
    glPopMatrix();

    // draw a mark on the ground
    /** TODO maybe make this look like a shadow in the final game ? */
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glTranslatef( position.x, 0.0, position.z );
    if(_type == BULLET_GRENADE)
        glutSolidSphere( _radius, 50, 50 );
    glPopMatrix();

    if(DEBUG_TANK)
    {
        //_collmod.printVertices();
        _collmod.draw();
    }
}

void Bullet::setPosition( const Point &position )
{
    _position = position;
}

void Bullet::setVelocity( const Vector3D &velocity )
{
    _velocity = velocity;
}

/**
 * collision model stuff
 */

double* Bullet::get2DVertex(int n) const
{
    return NULL;
}

void Bullet::updateCollisionModel()
{
    float vertices[8];
    vertices[0] = -0.5*_radius; vertices[1] = -0.5*_radius;
    vertices[2] = -0.5*_radius; vertices[3] = +0.5*_radius;
    vertices[4] = +0.5*_radius; vertices[5] = +0.5*_radius;
    vertices[6] = +0.5*_radius; vertices[7] = -0.5*_radius;

    for(int i = 0; i < 4; i++)
    {
        vertices[2*i] += _position.x;
        vertices[2*i + 1] += _position.z;
        _collmod.setVertex(i, vertices[2*i], vertices[2*i+1]);
    }

    _collmod.setYBottom(_position.y - _radius);
    _collmod.setYTop(_position.y + _radius);
}

void Bullet::findFields()
{
    _fields.clear();

    // field containing center of the bullet
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

void Bullet::printFields()
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

void Bullet::drawFields()
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

void Bullet::setType(BulletType t)
{
    _type = t;

    if(_type == BULLET_MG)
    {
        _velocity.y = 0.0;
        _position.y = 0.5;
    }

}




GRAPHICSLAB_NAMESPACE_END


