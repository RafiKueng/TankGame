/**
 * Section.hpp
 *
 * Author   :   Rkueng
 * Date     :   21.02.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_SECTION_HPP
#define GRAPHICSLAB_SECTION_HPP

// Base class include
#include "drawable.hpp"

// Includes
#include "types.hpp"
#include <stdlib.h>
#include <vector>
#include <iostream>


GRAPHICSLAB_NAMESPACE_BEGIN

/** Section */
class Section
{
public:
    /** Constructor */
	Section(){};
	Section(int x1, int z1, int x2, int z2, int d)
	{
		_coord1[0] = x1;
		_coord1[1] = z1;
		_coord2[0] = x2;
		_coord2[1] = z2;
		_depth = d;
	};

    /** Destructor */
	~Section(){};

	// some simple math funcions..
	int getA(){return ((_coord2[0]-_coord1[0]) * (_coord2[1]-_coord1[1]));}; //a = area
	int getSmallestSide()
	{
		//returns the smaller of the two sides of this Section
		int a=(_coord2[0]-_coord1[0]);
		int b=(_coord2[1]-_coord1[1]);
		//std::cerr<<a<<";"<<b<<"; "<<(a>b?b:a)<<std::endl;
		return a>b?b:a;
	};

	//TODO delete all this memory lecks...
	int* getMidPoint()
	{
		int * mp = new int[2];
		mp[0] = (int)ceil((_coord2[0]-_coord1[0])/2.0 + _coord1[0] + 0.5);
		mp[1] = (int)ceil((_coord2[1]-_coord1[1])/2.0 + _coord1[1] + 0.5);
		return mp;
	}

	int* getRandomPoint()
	{
		int * pnt = new int[2];
		int a=(_coord2[0]-_coord1[0]);
		int b=(_coord2[1]-_coord1[1]);
		pnt[0] = rand()%a+_coord1[0];
		pnt[1] = rand()%b+_coord1[1];
		return pnt;
	}

	// getter / setter
	int x1(){return _coord1[0];};
	int z1(){return _coord1[1];};
	int x2(){return _coord2[0];};
	int z2(){return _coord2[1];};

	int depth(){return _depth;};

	std::vector<Section*> Div(); //returns 1st part, 2nd part and the street in between

protected:
	int _coord1[2]; //upper left corner
	int _coord2[2]; //lower right corner
	int _depth; //iteration depth

}; // class Section

GRAPHICSLAB_NAMESPACE_END


#endif // GRAPHICSLAB_SECTION_HPP


