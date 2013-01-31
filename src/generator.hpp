/**
 * Generator.hpp
 *
 * Author   :   Rkueng
 * Date     :   
 *
 * All rights reserved.
 */


// Base class include
#include "drawable.hpp"

// Includes
#include "types.hpp"
#include <vector>

#include "section.hpp"
#include "block.hpp"
#include "street.hpp"
#include "place.hpp"
#include "house.hpp"
#include "powerUp.hpp"

//#include "texture.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

/** Generator */
class Generator
{
public:
    /** Constructor */
    Generator();
	Generator(int);

    /** Destructor */
    ~Generator();

	void Terrain(
		int dim,
		TerrainType** map,
		//std::vector<int> & Sections,
		std::vector<Block*> & Blocks,
		std::vector<Street*> & Streets,
		std::vector<Place*> & Places);

	
	void gBlock(
		Block * block,
		std::vector<House*> & houses
		);
	
	void gHouse(
		House * house
		);

	/* this is done direclty in landscape
	void gPowerUp(
		PowerUp * pu,
		int location[2]
		);
	*/
	
	//std::vector<Texture*> Generator::gStreetTexture();


private:
	int _rnd;
	int loaction[2];



}; // class Generator

GRAPHICSLAB_NAMESPACE_END