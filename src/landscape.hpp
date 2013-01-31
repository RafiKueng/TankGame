/**
 * landscape.hpp
 *
 * Author   :   Fabian Gehri, Rafael Kueng
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_LANDSCAPE_HPP
#define GRAPHICSLAB_LANDSCAPE_HPP

// Base class include
#include "drawable.hpp"

// Includes
#include <vector>

#include "types.hpp"

#include "generator.hpp"
#include "section.hpp"
#include "block.hpp"
#include "street.hpp"
#include "place.hpp"
#include "house.hpp"
#include "skybox.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

/** Landscape */
class Landscape : public Drawable
{
public:
    /** Constructor */
    Landscape();

    /** Destructor */
    ~Landscape();

    /** Draw the object to the screen. */
    void draw() const;

	// initialises all the data, objects in this level..
	void init();

    /** Reset the landscape parameters */
    void reset();

	TerrainType** getMap(); //returns pointer: DO NOT CHANGE
	TerrainType** getMap(Point location, int witdh); // returns a part of the map around the point with width.., it's a save deep copy!

	std::vector<PowerUp*> getPowerupVector()
	{
		return Powerups;
	};


	int * getFreeLocation();

	void setTimeSinceLastUpdate(float time)
	{_timeSLU = time;}

	void powerupPickedUp(int* coord);

	void setLevelNr (int nr) { _levelNr = nr; }

private:
	Generator* generate;// the almighty generator

	//Map data arrays
	TerrainType** map; //contains the terrain data, basically int[dim][dim] array
	PowerupType** mapPowerup; // basically int[dim][dim] array, with type of powerup
	// House*** mapPHouse; //for each field a pointer to the house witch is standing on this field
	PowerUp*** PowerupPointerMap; //for each field a pointer to the Powerup it contains


	//object Lists
	std::vector<Section*> Sections;
	std::vector<Block*> Blocks;
	std::vector<Street*> Streets;
	std::vector<Place*> Places;
	std::vector<House*> Houses;
	std::vector<PowerUp*> Powerups;

	Skybox * _skybox;

	//std::vector<Texture*> texStreet;

	int nHouses;

	//time since last update
	float _timeSLU;

	// level number
	int _levelNr;

	//--------------------------------------------------
	// OLD, get rid of these stuff
	int _xSlices;
	int _zSlices;
	int _planeX;
	int _planeZ;




}; // class Landscape

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_LANDSCAPE_HPP


