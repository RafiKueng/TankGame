/**
 * powerUp.hpp
 *
 * Author   :   Fabian Gehri, Rafael Küng
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_POWERUP_HPP
#define GRAPHICSLAB_POWERUP_HPP

// Base class include
#include "drawable.hpp"

// Includes
#include "types.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

/** PowerUp */
class PowerUp : public Drawable
{
public:
    /** Constructor */
    PowerUp();
	PowerUp(int *location, PowerupType);

    /** Destructor */
    ~PowerUp();

    /** Draw the object to the screen. */
    void draw() const;
    void draw(float angle) const;

    /** Reset the powerUp parameters */
    void reset();

    int* getLocation() {return _loc;}

    PowerupType getType() {return _type;}

	// set angle
	static void addTime(float timeSLU);
 	static float _angle; //actually, this shoud be static
	static bool firstTime;

private:
	PowerupType _type;
	int _loc[2];

}; // class PowerUp


GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_POWERUP_HPP


