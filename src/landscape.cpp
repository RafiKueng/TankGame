/**
 * landscape.cpp
 *
 * Author   :   Fabian Gehri, Rafael Kueng
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "landscape.hpp"

// Includes
#include <iostream>

#include "config.hpp"
#include "parameters.hpp"
#include "gl_includes.hpp"
#include "types.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

Landscape::Landscape() :
    Drawable(),
	_levelNr(0),
	_xSlices(FIELD_SIZE),
	_zSlices(FIELD_SIZE),
	_planeX(FIELD_SIZE),
	_planeZ(FIELD_SIZE)
{
/*	_xSlices = LANDSCAPE_GRID_XSIZE;
	_zSlices = LANDSCAPE_GRID_ZSIZE;
	_planeX = LANDSCAPE_PLANE_XSIZE;
	_planeZ = LANDSCAPE_PLANE_ZSIZE;*/

	reset();
}

Landscape::~Landscape()
{
	delete generate;
}

void Landscape::init()
{
	// init variables
	int dim = FIELD_SIZE;
	generate = new Generator(_levelNr); //init generator with fix random seed, change this later to TIME or something..

	//generating textures
	//texStreet = generate->gStreetTexture();

	//generating skybox
	_skybox = new Skybox();

	// generating map
	map = new TerrainType*[dim];
	generate->Terrain(dim, map, Blocks, Streets, Places);

	// for pop density
	//choose the center of the town, somewhere near the center of map
	int midX = rand()%(FIELD_SIZE/2)+FIELD_SIZE/4;
	int midZ = rand()%(FIELD_SIZE/2)+FIELD_SIZE/4;

	//generating houses
	Houses.clear();
	for (unsigned int i = 0; i<Blocks.size(); ++i)
	{
		Block * b = Blocks.at(i);
		int * mp = b->getMidPoint();
		int x = mp[0];
		int z = mp[1];
		int pd = 100 - sqrtf( (mp[0]-midX)*(mp[0]-midX)+(mp[1]-midZ)*(mp[1]-midZ) ) * 100 / (FIELD_SIZE);
		b->setPopDens(pd);
		generate->gBlock(b, Houses); //, mapHouse)
	}

	for (unsigned int i = 0; i<Houses.size(); ++i)
	{
		generate->gHouse(Houses.at(i));
	}

	if(DEBUG_HOUSES)
	{
		for (int i = 0; i<Houses.size(); ++i)
		{
			House * h = Houses.at(i);
			std::cout << "house nr."<<i<<": "<<h->x1() <<"/"<<h->z1()<<" ; "<<h->x2() <<"/"<<h->z2()<<std::endl;
		}
	}

	//generate powerups
	//////////////////////////

	// map for quick access to power ups
	PowerupPointerMap = new PowerUp**[dim];
	for (int i = 0; i<dim;++i)
	{
		*(PowerupPointerMap+i) = new PowerUp*[dim];
		for (int j=0; j<dim; j++)
		{
			PowerupPointerMap[i][j] = 0;
		}
	}

	//create all the power ups..
	for (unsigned int i = 0; i<N_POWERUPS; ++i)
	{
		int *loc = this->getFreeLocation();
		PowerUp * pu = new PowerUp(loc, PowerupType(rand()%2+1));
		if(DEBUG_LANDSCAPE){std::cout << "Created powerup nr."<<i<<" at "<<loc[0]<<" / "<<loc[1]<<std::endl;}
		Powerups.push_back(pu);
		PowerupPointerMap[loc[0]][loc[1]] = pu;
	}
}

void Landscape::reset()
{
	init();
}

void Landscape::draw() const
{
	glEnable( GL_DEPTH_TEST );
    glDisable( GL_BLEND );
	glDisable( GL_LIGHTING );


	//texture debug
	//GLuint id = texStreet.at(0)->TexName();
    
	/*
	glBegin(GL_QUADS);
		//glTranslatef(0,10,0);
		glTexCoord2f(0,0); glVertex3f(0,5,0);  //lo
		glTexCoord2f(0,1); glVertex3f(0,0,0); //lu
		glTexCoord2f(1,1); glVertex3f(5,0,0);  //ru
		glTexCoord2f(1,0); glVertex3f(5,5,0);   //ro
	glEnd;
	*/
	

    glPushMatrix();

	// DRAW SKYBOX
	_skybox->draw();

	// DRAW TERRAIN
    glPushMatrix();
	/*
	for (int i = 0; i<Places.size(); ++i)
		//Places.at(i)->draw();

	for (int i = 0; i<Streets.size(); ++i)
		Streets.at(i)->draw();
	*/
	
	float yHover = 0.00;

	glBegin( GL_QUADS );
	for ( int x = 0; x < _xSlices; x++ )
	{
		for ( int z = 0; z < _zSlices; z++ )
		{
			TerrainType t = map[x][z];
			if		(t == INVALID)			{glColor3f( 0.0, 0.0, 0.0 );yHover=1.0;}
			else if (t == BUILDING)			{glColor3f( 0.0, 0.0, 0.0 );yHover=0.0;}
			else if (t == STREET_FAST)		{glColor3f( 0.1, 0.1, 0.1 );yHover=0.0;}
			else if (t == STREET_MED)		{glColor3f( 0.2, 0.2, 0.2 );yHover=0.0;}
			else if (t == STREET_SLOW)		{glColor3f( 0.3, 0.3, 0.3 );yHover=0.0;}
			//else if (t == BUILDING_BORDER)	{glColor3f( 0.2, 0.2, 0.5 );yHover=0.0;}
			else if (t == PLACE)			{glColor3f( 0.25, 0.2, 0.2 );yHover=0.0;}
			

			//glColor3f( 0.2, 0.2, 0.2 );
            //glColor3f( 0.2 + 0.8*_damage[x][z], 0.3, 0.6 );
            glVertex3f( -_planeX/2.0 + 1.0*x*_planeX/_xSlices, yHover, -_planeZ/2.0 + 1.0*z*_planeZ/_zSlices );

            //glColor3f( 0.2 + 0.8*_damage[x][z+1], 0.3, 0.6 );
            glVertex3f( -_planeX/2.0 + 1.0*x*_planeX/_xSlices, yHover, -_planeZ/2.0 + 1.0*(z+1)*_planeZ/_zSlices );

            //glColor3f( 0.2 + 0.8*_damage[x+1][z+1], 0.3, 0.6 );
            glVertex3f( -_planeX/2.0 + 1.0*(x+1)*_planeX/_xSlices, yHover, -_planeZ/2.0 + 1.0*(z+1)*_planeZ/_zSlices );

            //glColor3f( 0.2 + 0.8*_damage[x+1][z], 0.3, 0.6 );
            glVertex3f( -_planeX/2.0 + 1.0*(x+1)*_planeX/_xSlices, yHover, -_planeZ/2.0 + 1.0*z*_planeZ/_zSlices );
		}
	}
	glEnd();


	glPopMatrix();

	//DRAW HOUSES
    glPushMatrix();
	
	glTranslatef(-FIELD_SIZE/2.0f,0.0f,-FIELD_SIZE/2.0f);
	for (int i = 0; i<Houses.size(); ++i) //
	{
		Houses.at(i)->draw();
	}
	//Houses.at(Houses.size()-1)->draw();
    glPopMatrix();

	//DRAW POWERUPS
    glPushMatrix();
	glDisable( GL_DEPTH_TEST );
    glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable( GL_LIGHTING );

	PowerUp::addTime(_timeSLU);
	for (int i = 0; i<N_POWERUPS ; ++i)
	{
		Powerups.at(i)->draw();
	}
    glPopMatrix();



    glPopMatrix();
}


TerrainType** Landscape::getMap()
{
	return map;
}


TerrainType** Landscape::getMap(Point location, int width)
	// TODO: please delete returned array after usage!!
{

	int x,z;
	int dim = FIELD_SIZE;

	TerrainType** mappart = new TerrainType*[width*2+1];

	for (int i=0; i<width*2+1; i++)
	{
		//x = (int)location.x - width + i;
		x = (int)location.x + FIELD_SIZE/2 - width + i; //temp for field ranging from -dim/2...dim/2
		*(mappart+i) = new TerrainType[width*2+1];
		for (int j=0; j<width*2+1; j++)
		{
			//z = location.z - width + j;
			z = location.z + FIELD_SIZE/2 - width + j;  //temp for field ranging from -dim/2...dim/2
			mappart[i][j] = (x>=0 && z>=0 && x<dim && z<dim )? map[x][z] : INVALID;
		}
	}
	return mappart;
}

int* Landscape::getFreeLocation()
	// TODO: please delete returned int after use!!!
{
	int * location;
	//first choose either streets or places
	if(rand()%100 > 50 || Places.empty())
	{
		int n = rand()%Streets.size();
		location = Streets.at(n)->getRandomPoint();
	}
	else
	{
		int n = rand()%Places.size();
		location = Places.at(n)->getRandomPoint();
	}

	//check whether its near building
	TerrainType ** smap = getMap(Point(location[0],0,location[1]),1);

	/*
	if(smap[1][0] == BUILDING) {location[1]+=1;}
	else if(smap[1][2] == BUILDING) {location[1]-=1;}

	if(smap[0][1] == BUILDING) {location[1]+=1;}
	else if(smap[2][1] == BUILDING) {location[1]-=1;}
	*/
	/*
	//if no valid point, get another one..
	if (location[0]<0 || location[1]<0 || location[0]>=FIELD_SIZE || location[1]>=FIELD_SIZE)
	{
		location = getFreeLocation();
	}
	*/

	return location;
}

void Landscape::powerupPickedUp(int * coord)
{
	//PowerUp * pu = PowerupPointerMap[coord[0]][coord[1]];

	for (int i = 0; i<N_POWERUPS; ++i) //this is slow, but there are not so many poweup picked up, and so too many powerups
	{
		int* loc = Powerups.at(i)->getLocation();
		if (coord[0] == loc[0] && coord[1] == loc[1])
		{
			//delete the power up
			PowerUp * pu = Powerups.at(i);
			Powerups.erase(Powerups.begin()+i);
			//PowerupPointerMap[coord[0]][coord[1]] = 0;
			delete pu;

			//create new one?
			//nope

			//exit loop
			break;
		}
	}
}

GRAPHICSLAB_NAMESPACE_END


