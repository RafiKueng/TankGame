/**
 * Street.hpp
 *
 * Author   :   Rkueng
 * Date     :   21.02.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_Street_HPP
#define GRAPHICSLAB_Street_HPP

// Base class include
#include "section.hpp"
#include "drawable.hpp"

// Includes
#include "types.hpp"
#include <vector>
#include <iostream>

//#include "Street.hpp"
//#include "street.hpp"
//#include "place.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

/** Street */
class Street : public Section
{
public:
    /** Constructor */
	Street();
	Street(int x1, int z1, int x2, int z2, int d)
	{
		_coord1[0] = x1;
		_coord1[1] = z1;
		_coord2[0] = x2;
		_coord2[1] = z2;
		_depth = d;
	};

	//convert from Section to Street (DELETE original!!)
	static Street * convert(Section* s)
	{
		Street* b = new Street(s->x1(), s->z1(), s->x2(), s->z2(), s->depth());//Block(s);
		delete s; s=0;
		return b;
	}

    /** Destructor */
	~Street(){};

	void draw() const;

	std::vector<Street*> Div(); //


private:

}; // class Street

GRAPHICSLAB_NAMESPACE_END

#endif // GRAPHICSLAB_Street_HPP