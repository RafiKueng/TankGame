/**
 * particle.cpp
 *
 * Author   :   Fabian Gehri
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "particleSystem.hpp"

// I needed this for M_PI to work (FG)
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

// Includes
#include "config.hpp"
#include "gl_includes.hpp"
#include <iostream>
#include <cmath>
#include <math.h>

GRAPHICSLAB_NAMESPACE_BEGIN

ParticleSystem::ParticleSystem() :
    _period(0.5),
    _r(0.0), _g(0.6), _b(0.0),
    _velrange(5.0),
    _lifetime(1.0),
    _state(PSYSTEM_ACTIVE),
    _nparticles(30)
{
    reset();
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::reset()
{
    _timer = -0.1;
}

void ParticleSystem::update(float timeSinceLastUpdate)
{
    // update systems lifetime
    if(_state == PSYSTEM_ACTIVE)
    {
        _lifetime -= timeSinceLastUpdate;
        if(_lifetime <= 0)
        {
            _state = PSYSTEM_DYING;
        }
    }

    // emit particles
    if(_state == PSYSTEM_ACTIVE)
    {
        _timer -= timeSinceLastUpdate;
        if(_timer < 0)
        {
            emit();
            _timer = _period;
        }
    }

    // update particles
    for(unsigned int i = 0; i < _particles.size(); i++)
    {
        _particles[i].update(timeSinceLastUpdate);
    }

    // delete outdated particles
    for(unsigned int i = 0; i < _particles.size(); i++)
    {
        if(_particles[i].getLifetime() <= 0.0)
        {
            _particles.erase(_particles.begin()+i);
        }
    }

    // if system is dying, wait, until all the particles
    // are outdated
    if(_state == PSYSTEM_DYING && _particles.size() == 0)
    {
        _state = PSYSTEM_DEAD;
    }
}

void ParticleSystem::draw() const
{
    for(unsigned int i = 0; i < _particles.size(); i++)
    {
        _particles[i].draw();
    }
}

void ParticleSystem::emit()
{
    for(int i = 0; i < _nparticles; i++)
    {
        Particle p(1.0);
        float r1 = -1.0 + 2.0*float(rand())/float(RAND_MAX);
        float r2 = -1.0 + 2.0*float(rand())/float(RAND_MAX);
        float r3 = -1.0 + 2.0*float(rand())/float(RAND_MAX);
        float r4 = float(rand())/float(RAND_MAX);
        float r5 = float(rand())/float(RAND_MAX);

        // set position
        p.setPosition(Point(_position.x + r1*0.5, _position.y + r2*0.5, _position.z + r3*0.5));

        // set velocity
        float velocityScale = _velrange*float(rand())/float(RAND_MAX);
        float theta = r4*M_PI;
        float phi = r5*2.0*M_PI;

        Vector3D velocity(
                              velocityScale * cos(theta) * cos(phi) ,
                              velocityScale * sin(theta),
                              velocityScale * -1 * cos(theta) * sin(phi)         );

        p.setVelocity(velocity);

        // set color
        p.setRGB(_r, _g, _b);

        _particles.push_back(p);
    }
}



GRAPHICSLAB_NAMESPACE_END


