/**
 * bullet.hpp
 *
 * Author   :   Fatih Erol
 * Date     :   21.02.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_BULLET_HPP
#define GRAPHICSLAB_BULLET_HPP

// Base class include
#include "drawable.hpp"

// Includes
#include "types.hpp"
#include "CollisionModel.h"

GRAPHICSLAB_NAMESPACE_BEGIN

enum BulletState
{
    BULLET_FLYING, BULLET_HIT, BULLET_DELETE
};

enum BulletType
{
    BULLET_MG,
    BULLET_GRENADE
};

/** Bullet */
class Bullet : public Drawable
{
public:
    /** Constructor */
    Bullet( float radius = 0.1 );

    /** Destructor */
    ~Bullet();

    /** Draw the object to the screen. */
    void draw() const;

    /** Update our bullet */
    void update( float timeSinceLastUpdate, float xSlices, float zSlices, float planeX, float planeZ);

    float getRadius() const { return _radius; }

    BulletState getState() const {return _state;}
    void setState(BulletState s) {_state = s; }

    const Point &getPosition() const { return _position; }
    void setPosition( const Point &position );

    const Vector3D &getVelocity() const { return _velocity; }
    void setVelocity( const Vector3D &velocity );

    /** collision model stuff */
    double* get2DVertex(int n) const;
    CollisionModel *getCollisionModel(){ return &_collmod; }
    void updateCollisionModel();

    void setType(BulletType t);
    BulletType getType() const {return _type;}

    IntVector getFields() const { return _fields; }
    void printFields();
    void drawFields();

private:
    float _radius;

    Point _position;
    Vector3D _velocity;

	// indices of fields the bullet is possibly flying over
	IntVector _fields;
	void findFields();

    BulletState _state;

    CollisionModel _collmod;
    BulletType _type;

}; // class Bullet

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_BULLET_HPP


