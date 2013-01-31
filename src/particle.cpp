/**
 * particle.cpp
 *
 * Author   :   Fabian Gehri
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "particle.hpp"

// Includes
#include "config.hpp"
#include "gl_includes.hpp"
#include <iostream>
#include <cmath>


GRAPHICSLAB_NAMESPACE_BEGIN

Particle::Particle(float lifetime) :
    Drawable(),
    _state(PARTICLE_FLYING),
    _collmod(COLL_RECTANGLE),
    _lifetime(lifetime),
    _r(0.6), _g(0.0), _b(0.0),
    _radius(0.02)
{
    _collmod.setYBottom(0.0);
    _collmod.setYTop(1.0);
    reset();
}

Particle::~Particle()
{
}

void Particle::reset()
{
    void update(float timeSinceLastUpdate);
}

void Particle::update(float timeSinceLastUpdate)
{
    const Point &position = getPosition();
    const Vector3D &velocity = getVelocity();

    // decrease lifetime
    _lifetime -= timeSinceLastUpdate;

    // compute new position
    float speed = sqrt( velocity.x*velocity.x + velocity.y*velocity.y + velocity.z*velocity.z );
    if ( speed < 0.1 )
    {
        setVelocity( Vector3D( 0.0, 0.0, 0.0 ) );
    }
    Point newPosition( position.x + velocity.x * timeSinceLastUpdate,
                      position.y + velocity.y * timeSinceLastUpdate + 0.5 * GRAVITATIONAL_ACCELARATION * timeSinceLastUpdate * timeSinceLastUpdate,
                      position.z + velocity.z * timeSinceLastUpdate );
    Vector3D newVelocity( velocity.x, velocity.y + GRAVITATIONAL_ACCELARATION * timeSinceLastUpdate, velocity.z );

    // reflect and slow down the particle, if it hits the ground
    if( newPosition.y < _radius)
    {
        newVelocity = Vector3D(newVelocity.x * 0.4, -newVelocity.y * 0.4, newVelocity.z * 0.4);
        newPosition = Point(newPosition.x, 0.0 + _radius, newPosition.z);
    }

    // update configuration
    setPosition( newPosition );
    setVelocity( newVelocity );

}

void Particle::draw() const
{
    // Particle surface material
/*    float materialAmbient[3] = { 0.1, 0.1, 0.1 };
    float materialDiffuse[3] = { 0.6, 0.3, 0.3 };
    float materialSpecular[3] = { 0.5, 0.5, 0.5 };
    float materialEmission[3] = { 0.0, 0.0, 0.0 };*/
    float materialAmbient[3] = { 0.1, 0.1, 0.1 };
    float materialDiffuse[3] = { _r, _g, _b };
    float materialSpecular[3] = { 0.5, 0.5, 0.5 };
    float materialEmission[3] = { 0.0, 0.0, 0.0 };
    int shininess = 24;

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular );
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, materialEmission );
    glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, shininess );

    // draw the particle
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    const Point &position = getPosition();
    glTranslatef( position.x, position.y, position.z );
    glutSolidSphere( _radius, 50, 50 );
    glPopMatrix();
}

/** collision model stuff */
void Particle::updateCollisionModel()
{
    float vertices[8];
    vertices[0] = -0.5; vertices[1] = -0.5;
    vertices[2] = -0.5; vertices[3] = +0.5;
    vertices[4] = +0.5; vertices[5] = +0.5;
    vertices[6] = +0.5; vertices[7] = -0.5;

    for(int i = 0; i < 4; i++)
    {
        vertices[2*i] += _position.x;
        vertices[2*i + 1] += _position.z;
        _collmod.setVertex(i, vertices[2*i], vertices[2*i+1]);
    }

    _collmod.setYBottom(_position.y - 0.5);
    _collmod.setYTop(_position.y + 0.5);
}

GRAPHICSLAB_NAMESPACE_END


