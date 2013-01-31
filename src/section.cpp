/**
 * Section.cpp
 *
 * Author   :   Rkueng
 * Date     :   21.02.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "section.hpp"

// Includes
#include "gl_includes.hpp"
#include <cmath>
#include <math.h>
#include <iostream>

GRAPHICSLAB_NAMESPACE_BEGIN


std::vector<Section*> Section::Div()
{
	// konstants
	float pBorder = 0.25;

	/*
	if (_depth%2==0)
	{
		_coord1[0]
	}
	else
	{
	}
	*/

	int dim = _depth%2;
	int span = _coord2[dim] - _coord1[dim];
	int mid = (span*(1-2*pBorder));
	int random = rand();
	int rnd = random%mid;

	int mpnkt = _coord1[dim]+pBorder*span + rnd;
	//std::cout << mpnkt<<" mpnkt, "<< rnd<<" rand, "<<mid<<"mid, "<<span<<"span, "<<random<<"random, ";

	int width;
	if (_depth<2) {
		width = 6;
	} else if (_depth<4){
		width = 4;
	} else {
		width = 2;
	}


	int p1, p2, p3, p4;
	if (dim==0)
	{
		p1 = mpnkt - width;
		p2 = _coord2[1];
		p3 = mpnkt + width;
		p4 = _coord1[1];
	}
	else
	{
		p1 = _coord2[0];
		p2 = mpnkt-width;
		p3 = _coord1[0];
		p4 = mpnkt+width;
	}

	Section *s[3];

	s[0] = new Section(_coord1[0],_coord1[1],p1,p2,_depth+1);
	s[1] = new Section(p3,p4,_coord2[0],_coord2[1],_depth+1);
	s[2] = new Section(p1<p3?p1:p3,p2<p4?p2:p4,p1<p3?p3:p1,p2<p4?p4:p2,_depth+1);

	std::vector<Section*> vs;
	
	vs.push_back(s[0]);
	vs.push_back(s[1]);
	vs.push_back(s[2]);

	//Section s[3];

	//s[0] = *s1;
	//s[1] = *s2;
	//s[2] = *s3;

	return vs;
}


GRAPHICSLAB_NAMESPACE_END