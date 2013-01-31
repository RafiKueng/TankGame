/**
 * Block.hpp
 *
 * Author   :   Rkueng
 * Date     :   21.02.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_BLOCK_HPP
#define GRAPHICSLAB_BLOCK_HPP

// Base class include
#include "section.hpp"
//#include "drawable.hpp"

// Includes
#include "types.hpp"
#include <vector>
#include <iostream>

//#include "block.hpp"
//#include "street.hpp"
//#include "place.hpp"
#include "house.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

/** Block */
class Block : public Section
{
public:
    /** Constructor */
	Block();
	Block(int x1, int z1, int x2, int z2, int d)
	{
		_coord1[0] = x1;
		_coord1[1] = z1;
		_coord2[0] = x2;
		_coord2[1] = z2;
		_depth = d;
	};


	//convert from Section to Block (DELETE original!!)
	static Block * convert(Section* s)
	{
		Block* b = new Block(s->x1(), s->z1(), s->x2(), s->z2(), s->depth());//Block(s);
		delete s; s=0;
		return b;
	}

    /** Destructor */
	~Block(){};

	std::vector<Block*> Div(); //

	void setPopDens(int pd) {_popDens = pd;}
	int getPopDens() {return _popDens;}

	std::vector<House*> _houses;

private:
	int _popDens;


}; // class Block

GRAPHICSLAB_NAMESPACE_END

#endif // GRAPHICSLAB_BLOCK_HPP