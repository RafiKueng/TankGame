/**
 * enemy.hpp
 * 
 * Author   :   Fabian Gehri
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_ENEMY_HPP
#define GRAPHICSLAB_ENEMY_HPP

// Base class include
#include "tank.hpp"

// Includes
#include "landscape.hpp"
#include "types.hpp"
#include <queue>

GRAPHICSLAB_NAMESPACE_BEGIN

/** Enemy */
class Enemy : public Tank
{
public:
    /** Constructor */
    Enemy();
    
    /** Destructor */
    ~Enemy();

	/** Draw the object to the screen. */
    void draw() const;
	void activateMaterial() const;
    
    /** Reset the enemy parameters */
    void reset();
    
	// choose next action
	void updateAction(Landscape* landscape, Point playerPosition);

private:
	// distance from position (i,j) to position
	float dist(int i, int j, Point position);

	// distance from position (i,j) to ((k,l)
	float dist(int i, int j, int k, int l);

	// check if (x2, z2) is visible from (x1, z1) despite buildings in fields
	bool isVisible(float x1, float z1, float x2, float z2, IntVector fields);

	// check if field (i,j) (measured from (x,z) = enemy's field) is visible from (x1, z1) despite buildings in surroundings
	bool isFieldVisible(float x1, float z1, int i, int j, int x, int z, TerrainType** surroundings);

	// maximum speed of a tank
	static const float v_max;

	// maximum turn speed of a tank
	static const float omega_max;

	// all fields expanded in A* search
	IntVector _AStarFields;
	
	// field chosen to aim at in A* search
	int _AStarAimField;

	// field chosen to be best in A* search
	int _AStarBestField;

	// Enemy tank surface material
    static const float _materialAmbientEnemy[3];
    static const float _materialDiffuseEnemy[3];
    static const float _materialSpecularEnemy[3];
    static const float _materialEmissionEnemy[3];
    static const int _shininessEnemy;
    
}; // class Enemy

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_ENEMY_HPP


