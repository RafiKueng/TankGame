/**
 * Generator.cpp
 *
 * Author   :   Rkueng
 * Date     :
 *
 * All rights reserved.
 */

// Class declaration include
#include "generator.hpp"

// Includes
#include "gl_includes.hpp"

#include <cmath>
#include <math.h>
#include <iostream>
#include <fstream>
#include <time.h>

#include "types.hpp"
#include "parameters.hpp"

#include "section.hpp"
#include "block.hpp"
#include "street.hpp"
#include "place.hpp"
#include "house.hpp"

//#include "texture.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

Generator::Generator()
{
	_rnd = time(NULL);
}

Generator::Generator(int rnd)
{
	_rnd = rnd;
}

Generator::~Generator()
{
}

void Generator::Terrain(
	int dim,
	TerrainType** map,
	//std::vector<int> & Sections,
	std::vector<Block*> & Blocks,
	std::vector<Street*> & Streets,
	std::vector<Place*> & Places)
{
	for (int s = 0; s < Blocks.size(); s++)
		delete Blocks[s];
	Blocks.clear();
	for (int s = 0; s < Streets.size(); s++)
		delete Streets[s];
	Streets.clear();
	for (int s = 0; s < Places.size(); s++)
		delete Places[s];
	Places.clear();

	// CONFIG: outsource this to config file later

	//int Amax = 9*12*10; //minimal block area
	int lmin = 20; //minimal sidelength of remaining blocks
	int depthlimit = 50;// iteration depth for block generation
	float pPlaceInsteadHouses = 0.1; // percentage of houseblocks that become open places
	bool debug = false; //bool debug = LANDSCAPE_TERRAIN_DEBUG //outputs the generated array to file map.txt

	/********************************************************
	 INIT
	********************************************************/

	srand(_rnd);

	//int k=0;

	/*
	std::vector<Section*> s2;

	s2 = s1->Div();
	Section tmp1 = *s2[0];
	Section tmp2 = *s2[1];
	Section tmp3 = *s2[2];
	*/

	std::vector<Section*> ToBig; //stores the sections still to big
	//std::vector<Section*> Street;
	//std::vector<Section*> Blocks;

	ToBig.push_back(new Section(0, 0, dim-1, dim-1, 0)); //create section containing the whole map and store it's pointer
	/*Section * wholeMap = new Section(0,0,dim,dim,0);
	ToBig.push_back(wholeMap);
	wholeMap = 0;*/

	Section * current;
	std::vector<Section*> currVect;

	/********************************************************
	 generate city layout, until blocks
	********************************************************/

	do
	{
		current = ToBig[ToBig.size()-1];
		ToBig.pop_back();

		//check if iteration limit reached..
		if (current->depth() >= depthlimit || current->getSmallestSide()<lmin)
		{
			//maybe find better version for converting this..
			Block *b = Block::convert(current);
			Blocks.push_back(b);
			continue;
		}

        std::vector<Section*> bla;
		currVect = current->Div();
		delete current; current = 0; //delete object and set pointer to 0

		Streets.push_back(Street::convert(currVect.at(2)));
		currVect.pop_back();

		//if (tempV[0]->A()>Amax){ToBig.push_back(tempV[0]);}
		if (currVect[0]->getSmallestSide() > lmin) {ToBig.push_back(currVect[0]);}
		else
		{
			//make a place instead a block of houses?
			if (rand()%100<pPlaceInsteadHouses*100)
			{
				Places.push_back(Place::convert(currVect[0]));
			}
			else
			{
				Block *b = Block::convert(currVect[0]);
				Blocks.push_back(b);
			}
		}
		//if (tempV[1]->A()>Amax){ToBig.push_back(tempV[1]);}
		if (currVect[1]->getSmallestSide() > lmin) {ToBig.push_back(currVect[1]);}
		else
		{
			//make a place instead a block of houses?
			if (rand()%100<pPlaceInsteadHouses*100)
			{
				Places.push_back(Place::convert(currVect[1]));
			}
			else
			{
				Block *b = Block::convert(currVect[1]);
				Blocks.push_back(b);
			}
		}
	} while (ToBig.size() > 0);


	/********************************************************
	 create map array
	*********************************************************
	  reihenfolge spielt eine rolle, am schluss sollen alle
	  streets in der nähe buildings mit building border
	  ersetzt werden
	********************************************************/

	//map = new TerrainType*[dim]; //int ** arr = new int*[dim];

	// init everything to invalid
	for (int i=0; i<dim; i++)
	{
		*(map+i) = new TerrainType[dim];
		for (int j=0; j<dim; j++)
		{
			map[i][j] = INVALID;
		}
	}

	Section * currAS; //use abstractSection here
	int depth;
	TerrainType type;

	//put in streets
	for (unsigned int i = 0; i<Streets.size(); ++i)
	{
		currAS = Streets.at(i);
		for (int x=currAS->x1(); x < currAS->x2(); ++x)
		{
			for (int z=currAS->z1(); z<currAS->z2(); ++z)
			{
				depth = currAS->depth();
				if(true){type = STREET_SLOW;}
				if(depth<=4){type = STREET_MED;}
				if(depth<=2){type = STREET_FAST;}
				if(x<0 || x>dim || z<0 || z>dim){continue;} //TODO this shouldnt happen, but it does, check why

				map[x][z] = type;
			}
		}
	}

	//put in places
	for (unsigned int i = 0; i<Places.size(); ++i)
	{
		currAS = Places.at(i);
		for (int x=currAS->x1(); x < currAS->x2(); ++x)
		{
			for (int z=currAS->z1(); z<currAS->z2(); ++z)
			{
				if(x<0 || x>dim || z<0 || z>dim){continue;} //TODO this shouldnt happen, but it does, check why
				map[x][z] = PLACE;
			}
		}
	}

	//put in blocks
	int x1, x2, z1, z2;
	for (unsigned int i = 0; i<Blocks.size(); ++i)
	{
		currAS = Blocks.at(i);
		x1 = currAS->x1();
		x2 = currAS->x2();
		z1 = currAS->z1();
		z2 = currAS->z2();

		//fill blocks
		for (int x=x1; x < x2; ++x)
		{
			for (int z=z1; z<z2; ++z)
			{
				if(x<0 || x>dim || z<0 || z>dim){continue;} //TODO this shouldnt happen, but it does, check why
				map[x][z] = BUILDING;
			}
		}

		//do the border
		// NOT ANYMORE NEEDED
		/*
		for (int x = x1-1; x<x2+1; ++x)
		{
			if (x>=0 && x<dim && map[x][z1] == STREET_MED)
			{map[x][z1] = BUILDING_BORDER;}
			if (x>=0 && x<dim && map[x][z2] == STREET_MED)
			{map[x][z2] = BUILDING_BORDER;}
		}
		for (int z = z1-1; z<z2+1; ++z)
		{
			if (z>=0 && z<dim && map[x1][z] == STREET_MED)
			{map[x1][z] = BUILDING_BORDER;}
			if (z>=0 && z<dim && map[x2][z] == STREET_MED)
			{map[x2][z] = BUILDING_BORDER;}
		}
		*/
	}

	/* DEBUG OUTPUT - very slow for big arrays...
	for (int i=0; i<dim; i++){
		for (int j=0; j<dim; j++){
			std::cerr << arr[i][j];
		}
		std::cerr << std::endl;
	}
	*/

	// debug out to file
	if(debug)
	{
		std::ofstream myfile;
		myfile.open ("map.txt");
		for (int i=0; i<dim; i++){
			for (int j=0; j<dim; j++){
				myfile << map[i][j];
			}
			myfile << std::endl;
		}
		myfile.close();
	}
}


void Generator::gBlock(
	Block * block,
	std::vector<House*> & houses
	)
{
	int x1 = block->x1();
	int z1 = block->z1();
	int x2 = block->x2();
	int z2 = block->z2();

	int lx = x2-x1;
	int lz = z2-z1;
	float stdlength;

	int nx1, nx2, nz1, nz2;
	House * h;
	PopulationDensity pd;

	if (block->getPopDens() > HOUSETYPE_HI){stdlength = 20; pd = POP_HIGH;}
	else if (block->getPopDens() > HOUSETYPE_MED){stdlength = 10; pd = POP_MID;}
	//else if (block->getPopDens() > HOUSETYPE_LOW){stdlength = 3; pd = POP_LOW;}
	else {stdlength = 7; pd = POP_LOW;}

	int divX = std::ceil(lx/(float)stdlength);
	int divZ = std::ceil(lz/(float)stdlength);

	for (int i = 0; i<divX;++i)
	{
		for(int j=0;j<divZ;++j)
		{
			nx1 = std::floor(i*(lx/(float)divX) + 0.5); //floor(x+0.5) rundet exakt..
			nx2 = std::floor((i+1)*(lx/(float)divX) + 0.5);
			nz1 = std::floor(j*(lz/(float)divZ) + 0.5);
			nz2 = std::floor((j+1)*(lz/(float)divZ) + 0.5);
			h = new House(nx1+x1,nz1+z1,nx2+x1,nz2+z1,block->getPopDens(),pd);
			houses.push_back(h);
			block->_houses.push_back(h);
		}
	}

	// unterteilen
	// hauser bauen
		// geometrie generieren
		// texturen generieren
		// display liste haus erstellen
		// mapHouses matrix updaten
	// displayliste block erstellen
		//for each house:
			//glTransform
			//glCallList odr so...
}

void Generator::gHouse(
	House * house
	)
{
	int xDim = house->x2()-house->x1();
	int zDim = house->z2()-house->z1();
	float height = house->getPopDens() / 2.0f + rand()%20-10;

	GLuint index = glGenLists(1);

	glNewList(index, GL_COMPILE);
		glPushMatrix();
			glTranslatef(0.0f, height/2.0f, 0.0f);
			glScalef((float)xDim, height, (float)zDim);
			glutSolidCube(1);
		glPopMatrix();
	glEndList();

	house->setDLIndex(index);

}

/*
void Generator::gPowerUp(
	PowerUp * pu,
	int location[2]
	)
{
	int rnd = rand();
	pu = new PowerUp(location, PowerupType((rnd%5)+1));

}
*/

/*
std::vector<Texture*> Generator::gStreetTexture()
{
	std::vector<Texture*> t;
	Texture * tex = new Texture();
	tex->SetSize(4,32);
	tex->Fill(0,0,0);
	t.push_back(tex);
	return t;
}*/


GRAPHICSLAB_NAMESPACE_END
