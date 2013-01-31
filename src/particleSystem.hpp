/**
 * particle.hpp
 *
 * Author   :   Fabian Gehri
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_PARTICLESYSTEM_HPP
#define GRAPHICSLAB_PARTICLESYSTEM_HPP

// Base class include
#include "drawable.hpp"

// Includes
#include "types.hpp"
#include "particle.hpp"
#include "CollisionModel.h"
#include <vector>

GRAPHICSLAB_NAMESPACE_BEGIN

enum PSystemState
{
    PSYSTEM_ACTIVE,
    PSYSTEM_DYING,
    PSYSTEM_DEAD
};

class ParticleSystem : public Drawable
{
public:
    /** Constructor */
    ParticleSystem();

    /** Destructor */
    ~ParticleSystem();

    /** reset the object */
    void reset();

    /** Draw the object to the screen. */
    void draw() const;

    /** emit particles */
    void emit();

    /** update the system */
    void update(float timeSinceLastUpdate);

    /** SET & GET */
    const Point &getPosition() const { return _position; }
	void setPosition( const Point &position ) { _position = position;}

    const Vector3D &getVelocity() const { return _velocity; }
    void setVelocity( const Vector3D &velocity ) {_velocity = velocity;}

    const Vector3D getPeriod() const { return _period; }
    void setPeriod( const float period ) {_period = period;}

    const Vector3D getVelRange() const { return _velrange; }
    void setVelRange( const float velrange ) {_velrange = velrange;}

    const float getLifetime() const { return _lifetime; }
    void setLifetime( const float lifetime ) {_lifetime = lifetime;}

    const PSystemState getState() const { return _state; }
    void setState( const PSystemState state ) {_state = state;}

    const int getNParticles() const { return _nparticles; }
    void setNParticles( const int nparticles ) {_nparticles = nparticles;}

    const float getR() const { return _r; }
    const float getG() const { return _g; }
    const float getB() const { return _b; }
    void setRGB( const float r, const float g, const float b ) {_r = r; _g = g; _b = b;}

private:

    Point _position;
    Vector3D _velocity;
    float _timer;
    float _period;
    float _lifetime;
    float _r, _g, _b;
    float _velrange;
    int _nparticles;

    PSystemState _state;

    typedef std::vector<Particle> particleVector;
    particleVector _particles;

}; // class Particle

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_PARTICLE_HPP


