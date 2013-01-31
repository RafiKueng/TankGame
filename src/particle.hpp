/**
 * particle.hpp
 *
 * Author   :   Fabian Gehri
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_PARTICLE_HPP
#define GRAPHICSLAB_PARTICLE_HPP

// Base class include
#include "drawable.hpp"

// Includes
#include "types.hpp"
#include "CollisionModel.h"

GRAPHICSLAB_NAMESPACE_BEGIN

enum ParticleState
{
    PARTICLE_FLYING,
    PARTICLE_DEAD
};

/** Particle */
class Particle : public Drawable
{
public:
    /** Constructor */
    Particle(float lifetime);

    /** Destructor */
    ~Particle();

    /** Draw the object to the screen. */
    void draw() const;

    /** Reset the particle parameters */
    void reset();

    /** Reset the particle parameters */
    void update(float timeSinceLastUpdate);

    /** GET & SET */

    ParticleState getState() const {return _state;}
    void setState(ParticleState s) {_state = s; }

    const Point &getPosition() const { return _position; }
	void setPosition( const Point &position ) { _position = position;}

    const Vector3D &getVelocity() const { return _velocity; }
    void setVelocity( const Vector3D &velocity ) {_velocity = velocity;};

    const float getLifetime() const { return _lifetime; }
    void setLifetime( const float lf ) {_lifetime = lf;}

    const float getR() const { return _r; }
    const float getG() const { return _g; }
    const float getB() const { return _b; }
    void setRGB( const float r, const float g, const float b ) {_r = r; _g = g; _b = b;}

    const float getRadius() const { return _radius; }
    void setRadius( const float r ) {_radius = r;}

    /** collision model stuff */
    CollisionModel *getCollisionModel(){ return &_collmod; }
    void updateCollisionModel();

private:

    Point _position;
    Vector3D _velocity;
    float _lifetime;
    float _r, _g, _b;
    float _radius;

    ParticleState _state;

    CollisionModel _collmod;

}; // class Particle

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_PARTICLE_HPP


