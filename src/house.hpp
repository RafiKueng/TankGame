/**
 * House.hpp
 *
 * Author   :   Rkueng
 * Date     :   21.02.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_HOUSE_HPP
#define GRAPHICSLAB_HOUSE_HPP

// Base class include
#include "section.hpp"
#include "drawable.hpp"

// Includes
#include "types.hpp"
#include <vector>
#include <iostream>

//#include "House.hpp"
//#include "street.hpp"
//#include "place.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

/** House */
class House : public Section, public Drawable
{
public:
    /** Constructor */
	House();
	House(int x1, int z1, int x2, int z2, int pd, PopulationDensity pdt)
	{
		_coord1[0] = x1;
		_coord1[1] = z1;
		_coord2[0] = x2;
		_coord2[1] = z2;
		_popDens = pd;
		_popDensType = pdt;
	};


	//convert from Section to House (DELETE original!!)
	/*
	static House * convert(Section* s)
	{
		House* b = new House(s->x1(), s->z1(), s->x2(), s->z2(), s->depth());//House(s);
		delete s; s=0;
		return b;
	}
	*/

    /** Destructor */
	~House(){};

	void draw() const;

	//std::vector<House*> generate(); //
	void setDLIndex(unsigned int i){_displayListIndex = i;};
	unsigned int getDLIndex(){return _displayListIndex;};
	
	int getPopDens(){return _popDens;};


private:
	int _popDens;
	PopulationDensity _popDensType;
	unsigned int _displayListIndex;

}; // class House

GRAPHICSLAB_NAMESPACE_END

#endif // GRAPHICSLAB_House_HPP