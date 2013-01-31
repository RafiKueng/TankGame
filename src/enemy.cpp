/**
 * enemy.cpp
 *
 * Author   :   Fabian Gehri
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "enemy.hpp"

// Includes
#include "config.hpp"
#include "gl_includes.hpp"
#include "parameters.hpp"
#include "AStarNode.hpp"
#include "scene.hpp"
#include <iostream>
#include <cmath>

GRAPHICSLAB_NAMESPACE_BEGIN

// static constants
const float Enemy::v_max = ACCELERATION * FRICTION_DECELERATION_COEFF / (1.0f - FRICTION_DECELERATION_COEFF);
const float Enemy::omega_max = TURN_ACCELERATION * FRICTION_TURN_DECELERATION_COEFF / (1.0f - FRICTION_TURN_DECELERATION_COEFF);

// Enemy tank surface material
const float Enemy::_materialAmbientEnemy[3] = { 0.1, 0.05, 0.05 };
const float Enemy::_materialDiffuseEnemy[3] = { 0.3, 0.25, 0.25 };
const float Enemy::_materialSpecularEnemy[3] = { 0.5, 0.45, 0.45 };
const float Enemy::_materialEmissionEnemy[3] = { 0.0, 0.0, 0.0 };
const int Enemy::_shininessEnemy = 24;

Enemy::Enemy() :
	Tank()
{
	_initialPosition = Point(
		((rand()%100)-50)/100.0 * FIELD_SIZE*0.1,
		0.0f,
		((rand()%100)-50)/100.0 * FIELD_SIZE*0.1);

	_initialAngle = (float)rand() / (float)RAND_MAX * 360.0f;

	reset();
}

Enemy::~Enemy()
{
}

void Enemy::reset()
{
	Tank::reset();
}

void Enemy::updateAction(Landscape* landscape, Point playerPosition)
{
	_AStarFields.clear();
	_AStarAimField = -1;
	_AStarBestField = -1;

	// reset all
	setAccelerating(false);
	setDecelerating(false);
	setAcceleratingLeft(false);
	setAcceleratingRight(false);
	setIncreasingElevation(false);
	setDecreasingElevation(false);
	setIncreasingAzimuth(false);
	setDecreasingAzimuth(false);
	setIncreasingPower(false);
	setDecreasingPower(false);
	setShootingMG(false);
	setShootingGrenade(false);

	// get surrounding of this enemy
	TerrainType** surroundings = landscape->getMap(_position, AI_VIEW_DISTANCE);

	// make sure Point.w is 1 for _position and playerPosition
	_position.x /= _position.w;
	_position.y /= _position.w;
	_position.z /= _position.w;
	_position.w = 1.0f;
	playerPosition.x /= playerPosition.w;
	playerPosition.y /= playerPosition.w;
	playerPosition.z /= playerPosition.w;
	playerPosition.w = 1.0f;

	// field of this enemy
	int x = (int)_position.x + FIELD_SIZE / 2;
	int z = (int)_position.z + FIELD_SIZE / 2;

	//// if this enemy is facing a building, just go backwards and do nothing else
	//TerrainType t11 = surroundings[AI_VIEW_DISTANCE + 1][AI_VIEW_DISTANCE];
	//TerrainType t21 = surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE - 1];
	//TerrainType t31 = surroundings[AI_VIEW_DISTANCE - 1][AI_VIEW_DISTANCE];
	//TerrainType t41 = surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE + 1];
	//TerrainType t12 = surroundings[AI_VIEW_DISTANCE + 2][AI_VIEW_DISTANCE];
	//TerrainType t22 = surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE - 2];
	//TerrainType t32 = surroundings[AI_VIEW_DISTANCE - 2][AI_VIEW_DISTANCE];
	//TerrainType t42 = surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE + 2];
	//if (_speed >= 0.0f && ((_angle < 45.0f || 315.0f <= _angle) && (surroundings[AI_VIEW_DISTANCE + 1][AI_VIEW_DISTANCE] == BUILDING || surroundings[AI_VIEW_DISTANCE + 2][AI_VIEW_DISTANCE] == BUILDING)
	//	|| 45.0f <= _angle && _angle < 135.0f && (surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE - 1] == BUILDING || surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE - 2] == BUILDING)
	//	|| 135.0f <= _angle && _angle < 225.0f && (surroundings[AI_VIEW_DISTANCE - 1][AI_VIEW_DISTANCE] == BUILDING || surroundings[AI_VIEW_DISTANCE - 2][AI_VIEW_DISTANCE] == BUILDING)
	//	|| 225.0f <= _angle && _angle < 315.0f && (surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE + 1] == BUILDING || surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE + 2] == BUILDING)))
	//{
	//	setDecelerating(true);
	//	return;
	//}

	// field of player
	int x_player = (int)playerPosition.x + FIELD_SIZE / 2;
	int z_player = (int)playerPosition.z + FIELD_SIZE / 2;

	// calculate aim vector = vector from this enemy's position to player's position
	Vector3D aim = Vector3D(playerPosition.x - _position.x, 0.0f, playerPosition.z - _position.z);
	float aimAngle = TO_DEGREE(atan2f(-aim.z, aim.x));
	if (aimAngle >= 360.0f)
		aimAngle -= 360.0f;
	else if (aimAngle < 0.0f)
		aimAngle += 360.0f;
	float aimDist = sqrtf(aim.x * aim.x + aim.z * aim.z);

	// find and store all fields in surrounding having a building (used for visibility check)
	IntVector buildingFieldsOfSurrounding;
	for (int ii = 0; ii < 2 * AI_VIEW_DISTANCE + 1; ii++)
	{
		for (int jj = 0; jj < 2 * AI_VIEW_DISTANCE + 1; jj++)
		{
			if (surroundings[ii][jj] == BUILDING)
				buildingFieldsOfSurrounding.push_back((z + jj - AI_VIEW_DISTANCE) * FIELD_SIZE + (x + ii - AI_VIEW_DISTANCE));
		}
	}

	// check if player is within surrounding
	bool inSurrounding = x - AI_VIEW_DISTANCE <= x_player && x_player <= x + AI_VIEW_DISTANCE && z - AI_VIEW_DISTANCE <= z_player && z_player <= z + AI_VIEW_DISTANCE;
	bool visible = false;

	if (inSurrounding)
	{
		// PLAYER IS IN SURROUNDING

		// check if player is directly visible
		visible = isVisible(_position.x, _position.z, playerPosition.x, playerPosition.z, buildingFieldsOfSurrounding);

		if (visible)
		{
			// PLAYER IS DIRECTLY VISIBLE -> aim at player and shoot (do not drive)

			// aim at player
			if (_angle < aimAngle && aimAngle - _angle <= 180.0f || _angle > aimAngle && _angle - aimAngle > 180.0f)
				setAcceleratingLeft(true);
			else
				setAcceleratingRight(true);

			// shoot with machine gun
			setShootingMG(true);
		}

		// shoot grenade if player tank is near (or aim grenade launcher if not ready)
		if (aimDist <= AI_GRENADE_DIST && _hasGrenadeLauncherEnabled)
		{
			if (_firingStateGrenade == READY)
				setShootingGrenade(true);
			else
			{
				float totalAngle = _angle + _azimuth;
				if (totalAngle >= 360.0f)
					totalAngle -= 360.0f;
				else if (totalAngle < 0.0f)
					totalAngle += 360.0f;
				if (totalAngle < aimAngle && aimAngle - totalAngle <= 180.0f || totalAngle > aimAngle && totalAngle - aimAngle > 180.0f)
					setIncreasingAzimuth(true);
				else
					setDecreasingAzimuth(true);

				if (_elevation > 20.0f)
					setDecreasingElevation(true);
			}
		}
	}

	if (!inSurrounding || !visible)
	{
		// PLAYER IS OUTSIDE OF SURROUNDING OR NOT DIRECTLY VISIBLE -> try to get as near as possible to player (do not shoot)

		// find goal state ( = field within surroundings which is closest to player position)
		int i_goal = AI_VIEW_DISTANCE;
		int j_goal = AI_VIEW_DISTANCE;
		float d, min_dist = dist(x, z, playerPosition);
		for (int ii = 0; ii < 2 * AI_VIEW_DISTANCE + 1; ii++)
		{
			for (int jj = 0; jj < 2 * AI_VIEW_DISTANCE + 1; jj++)
			{
				if (surroundings[ii][jj] != BUILDING)
				{
					d = dist(x + ii - AI_VIEW_DISTANCE, z + jj - AI_VIEW_DISTANCE, playerPosition);
					if (d < min_dist)
					{
						min_dist = d;
						i_goal = ii;
						j_goal = jj;
					}
				}
			}
		}

		// variables needed in A* search
		Fringe fringe;
		unsigned int step = 0;
		AStarNode *node = new AStarNode(0.0f, aimDist, AI_VIEW_DISTANCE, AI_VIEW_DISTANCE, AI_VIEW_DISTANCE, AI_VIEW_DISTANCE, true, NULL);
		int i, j;
		bool fieldVisible;

		// add initial state to fringe
		fringe.push(node);

		// do actual A* search
		while (step < AI_MAX_STEPS && !fringe.empty())
		{
			node = fringe.top();
			fringe.pop();
			i = node->i;
			j = node->j;

			if (DEBUG_ENEMY)
				_AStarFields.push_back((z + j - AI_VIEW_DISTANCE) * FIELD_SIZE + (x + i - AI_VIEW_DISTANCE));

			// goal test (stop iteration when goal is reached)
			if (i == i_goal && j == j_goal)
				break;

			// expand node
			if (0 <= i - 1 && surroundings[i - 1][j] != BUILDING && !node->hasVisited(i - 1, j))
			{
				if (node->isVisible)
					fieldVisible = isFieldVisible(_position.x, _position.z, i - 1, j, x, z, surroundings);
				else
					fieldVisible = false;
				//fringe.push(AStarNode(node.cost_so_far + 1.0f, dist(x + i - 1 - AI_VIEW_DISTANCE, z + j - AI_VIEW_DISTANCE, playerPosition),
				//	i - 1, j, fieldVisible ? i - 1 : node.i_aim, fieldVisible ? j : node.j_aim, fieldVisible));
				fringe.push(new AStarNode(fieldVisible ? dist(x + i - 1 - AI_VIEW_DISTANCE, z + j - AI_VIEW_DISTANCE, _position) : node->cost_so_far + 1.0f,
					dist(x + i - 1 - AI_VIEW_DISTANCE, z + j - AI_VIEW_DISTANCE, playerPosition),
					i - 1, j, fieldVisible ? i - 1 : node->i_aim, fieldVisible ? j : node->j_aim, fieldVisible, node));
			}
			if (i + 1 < 2 * AI_VIEW_DISTANCE + 1 && surroundings[i + 1][j] != BUILDING && !node->hasVisited(i + 1, j))
			{
				if (node->isVisible)
					fieldVisible = isFieldVisible(_position.x, _position.z, i + 1, j, x, z, surroundings);
				else
					fieldVisible = false;
				fringe.push(new AStarNode(fieldVisible ? dist(x + i + 1 - AI_VIEW_DISTANCE, z + j - AI_VIEW_DISTANCE, _position) : node->cost_so_far + 1.0f,
					dist(x + i + 1 - AI_VIEW_DISTANCE, z + j - AI_VIEW_DISTANCE, playerPosition),
					i + 1, j, fieldVisible ? i + 1 : node->i_aim, fieldVisible ? j : node->j_aim, fieldVisible, node));
			}
			if (0 <= j - 1 && surroundings[i][j - 1] != BUILDING && !node->hasVisited(i, j - 1))
			{
				if (node->isVisible)
					fieldVisible = isFieldVisible(_position.x, _position.z, i, j - 1, x, z, surroundings);
				else
					fieldVisible = false;
				fringe.push(new AStarNode(fieldVisible ? dist(x + i - AI_VIEW_DISTANCE, z + j - 1 - AI_VIEW_DISTANCE, _position) : node->cost_so_far + 1.0f,
					dist(x + i - AI_VIEW_DISTANCE, z + j - 1 - AI_VIEW_DISTANCE, playerPosition),
					i, j - 1, fieldVisible ? i : node->i_aim, fieldVisible ? j - 1 : node->j_aim, fieldVisible, node));
			}
			if (j + 1 < 2 * AI_VIEW_DISTANCE + 1 && surroundings[i][j + 1] != BUILDING && !node->hasVisited(i, j + 1))
			{
				if (node->isVisible)
					fieldVisible = isFieldVisible(_position.x, _position.z, i, j + 1, x, z, surroundings);
				else
					fieldVisible = false;
				fringe.push(new AStarNode(fieldVisible ? dist(x + i - AI_VIEW_DISTANCE, z + j + 1 - AI_VIEW_DISTANCE, _position) : node->cost_so_far + 1.0f,
					dist(x + i - AI_VIEW_DISTANCE, z + j + 1 - AI_VIEW_DISTANCE, playerPosition),
					i, j + 1, fieldVisible ? i : node->i_aim, fieldVisible ? j + 1 : node->j_aim, fieldVisible, node));
			}

			step++;
		}

		if (DEBUG_ENEMY)
		{
			_AStarAimField = (z + node->j_aim - AI_VIEW_DISTANCE) * FIELD_SIZE + (x + node->i_aim - AI_VIEW_DISTANCE);
			_AStarBestField = (z + node->j - AI_VIEW_DISTANCE) * FIELD_SIZE + (x + node->i - AI_VIEW_DISTANCE);
			std::cout << step << std::endl;
		}

		// set new aim according to A* search (aim at farthest visible field in path to the chosen node)
		aim = Vector3D(x + node->i_aim - AI_VIEW_DISTANCE - FIELD_SIZE / 2 + 0.5f - _position.x,
			0.0f,
			z + node->j_aim - AI_VIEW_DISTANCE - FIELD_SIZE / 2 + 0.5f - _position.z);
		aimAngle = TO_DEGREE(atan2f(-aim.z, aim.x));
		if (aimAngle >= 360.0f)
			aimAngle -= 360.0f;
		else if (aimAngle < 0.0f)
			aimAngle += 360.0f;
		aimDist = sqrtf(aim.x * aim.x + aim.z * aim.z);

		// calculate angle difference alpha and steer in the desired direction to reach aim
		float alpha;
		if (_angle < aimAngle)
		{
			alpha = aimAngle - _angle;
			if (alpha <= 180.0f)
				setAcceleratingLeft(true);
			else
			{
				alpha = 360.0f - alpha;
				setAcceleratingRight(true);
			}
		}
		else
		{
			alpha = _angle - aimAngle;
			if (alpha <= 180.0f)
				setAcceleratingRight(true);
			else
			{
				alpha = 360.0f - alpha;
				setAcceleratingLeft(true);
			}
		}

		//// if this enemy is facing a building, go backwards, no matter where the aim is
		//if (/*_speed >= 0.0f &&*/ ((_angle < 45.0f || 315.0f <= _angle) && surroundings[AI_VIEW_DISTANCE + 1][AI_VIEW_DISTANCE] == BUILDING /*|| surroundings[AI_VIEW_DISTANCE + 2][AI_VIEW_DISTANCE] == BUILDING*/
		//	|| 45.0f <= _angle && _angle < 135.0f && surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE - 1] == BUILDING /*|| surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE - 2] == BUILDING*/
		//	|| 135.0f <= _angle && _angle < 225.0f && surroundings[AI_VIEW_DISTANCE - 1][AI_VIEW_DISTANCE] == BUILDING /*|| surroundings[AI_VIEW_DISTANCE - 2][AI_VIEW_DISTANCE] == BUILDING*/
		//	|| 225.0f <= _angle && _angle < 315.0f && surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE + 1] == BUILDING /*|| surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE + 2] == BUILDING*/))
		//{
		//	setDecelerating(true);
		//}
		//else
		//{
			// else: using alpha, maximum turn speed and distance to aim to adapt speed to reach aim
			if (alpha != 0.0f)
			{
				float v_optimal = AI_SPEED_FACTOR * omega_max * aimDist / 2.0f / std::sin(TO_RADIAN(alpha) / 2.0f);
				if (_speed < v_optimal)
					setAccelerating(true);
				else
					setDecelerating(true);
			}
			else
				setAccelerating(true);
		//}

		// delete nodes
		while (!fringe.empty())
		{
			delete fringe.top();
			fringe.pop();
		}
	}

	// if this enemy is facing a building, go backwards
	TerrainType t11 = surroundings[AI_VIEW_DISTANCE + 1][AI_VIEW_DISTANCE];
	TerrainType t11l = surroundings[AI_VIEW_DISTANCE + 1][AI_VIEW_DISTANCE - 1];
	TerrainType t11r = surroundings[AI_VIEW_DISTANCE + 1][AI_VIEW_DISTANCE + 1];
	TerrainType t12 = surroundings[AI_VIEW_DISTANCE + 2][AI_VIEW_DISTANCE];
	TerrainType t12l = surroundings[AI_VIEW_DISTANCE + 2][AI_VIEW_DISTANCE - 1];
	TerrainType t12r = surroundings[AI_VIEW_DISTANCE + 2][AI_VIEW_DISTANCE + 1];
	TerrainType t21 = surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE - 1];
	TerrainType t21l = surroundings[AI_VIEW_DISTANCE - 1][AI_VIEW_DISTANCE - 1];
	TerrainType t21r = surroundings[AI_VIEW_DISTANCE + 1][AI_VIEW_DISTANCE - 1];
	TerrainType t22 = surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE - 2];
	TerrainType t22l = surroundings[AI_VIEW_DISTANCE - 1][AI_VIEW_DISTANCE - 2];
	TerrainType t22r = surroundings[AI_VIEW_DISTANCE + 1][AI_VIEW_DISTANCE - 2];
	TerrainType t31 = surroundings[AI_VIEW_DISTANCE - 1][AI_VIEW_DISTANCE];
	TerrainType t31l = surroundings[AI_VIEW_DISTANCE - 1][AI_VIEW_DISTANCE - 1];
	TerrainType t31r = surroundings[AI_VIEW_DISTANCE - 1][AI_VIEW_DISTANCE + 1];
	TerrainType t32 = surroundings[AI_VIEW_DISTANCE - 2][AI_VIEW_DISTANCE];
	TerrainType t32l = surroundings[AI_VIEW_DISTANCE - 2][AI_VIEW_DISTANCE - 1];
	TerrainType t32r = surroundings[AI_VIEW_DISTANCE - 2][AI_VIEW_DISTANCE + 1];
	TerrainType t41 = surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE + 1];
	TerrainType t41l = surroundings[AI_VIEW_DISTANCE - 1][AI_VIEW_DISTANCE + 1];
	TerrainType t41r = surroundings[AI_VIEW_DISTANCE + 1][AI_VIEW_DISTANCE + 1];
	TerrainType t42 = surroundings[AI_VIEW_DISTANCE][AI_VIEW_DISTANCE + 2];
	TerrainType t42l = surroundings[AI_VIEW_DISTANCE - 1][AI_VIEW_DISTANCE + 2];
	TerrainType t42r = surroundings[AI_VIEW_DISTANCE + 1][AI_VIEW_DISTANCE + 2];
	if (/*_speed >= 0.0f &&*/ ((_angle < 45.0f || 315.0f <= _angle) && (t11 == BUILDING /*|| t11l == BUILDING || t11r == BUILDING*/ || t12 == BUILDING /*|| t12l == BUILDING || t12r == BUILDING*/)
		|| 45.0f <= _angle && _angle < 135.0f && (t21 == BUILDING /*|| t21l == BUILDING || t21r == BUILDING */|| t22 == BUILDING /*|| t22l == BUILDING || t22r == BUILDING*/)
		|| 135.0f <= _angle && _angle < 225.0f && (t31 == BUILDING /*|| t31l == BUILDING || t31r == BUILDING */|| t32 == BUILDING /*|| t32l == BUILDING || t32r == BUILDING*/)
		|| 225.0f <= _angle && _angle < 315.0f && (t41 == BUILDING /*|| t41l == BUILDING || t41r == BUILDING */|| t42 == BUILDING /*|| t42l == BUILDING || t42r == BUILDING*/)))
	{
		setAccelerating(false);
		setDecelerating(true);
	}

	// delete surroundings
	for (int s = 0; s < 2 * AI_VIEW_DISTANCE + 1; s++)
		delete[] surroundings[s];
	delete[] surroundings;

	// random movement
	/*setAccelerating(rand()%2==0);
	setDecelerating(rand()%2==0);
	setAcceleratingLeft(rand()%2==0);
	setAcceleratingRight(rand()%5==0);
	setIncreasingElevation(rand()%2==0);
	setDecreasingElevation(rand()%2==0);
	setIncreasingAzimuth(rand()%2==0);
	setDecreasingAzimuth(rand()%2==0);
	setIncreasingPower(rand()%2==0);
	setDecreasingPower(rand()%2==0);*/

	/*if(rand()%10==0)
		shoot();*/
}

float Enemy::dist(int i, int j, Point position)
{
	float dx = i - FIELD_SIZE / 2 + 0.5f - position.x;
	float dz = j - FIELD_SIZE / 2 + 0.5f - position.z;

	return sqrtf(dx * dx + dz * dz);
}

float Enemy::dist(int i, int j, int k, int l)
{
	float dx = (float)(i - k);
	float dz = (float)(j - l);

	return sqrtf(dx * dx + dz * dz);
}

bool Enemy::isVisible(float x1, float z1, float x2, float z2, IntVector fields)
{
	if (fields.empty())
		return true;

	CollisionModel collMod(COLL_RECTANGLE);
	//collMod.setYBottom(0.0f);
	//collMod.setYTop(0.1f);
	float m = (z2 - z1) / (x2 - x1);
	float ax = AI_COLL_WIDTH / 2.0f / sqrtf(1.0f + 1.0f / (m * m));
	float az = -ax / m;
	collMod.setVertex(0, x1 + ax, z1 + az);
	collMod.setVertex(1, x1 - ax, z1 - az);
	collMod.setVertex(2, x2 + ax, z2 + az);
	collMod.setVertex(3, x2 - ax, z2 - az);

	return !Scene::RectCollidesWithFields(&collMod, fields);
}

bool Enemy::isFieldVisible(float x1, float z1, int i, int j, int x, int z, TerrainType** surroundings)
{
	IntVector fields;
	int imin, imax, jmin, jmax;
	if (i < AI_VIEW_DISTANCE)
	{
		imin = i;
		imax = AI_VIEW_DISTANCE;
	}
	else
	{
		imax = i;
		imin = AI_VIEW_DISTANCE;
	}
	if (j < AI_VIEW_DISTANCE)
	{
		jmin = j;
		jmax = AI_VIEW_DISTANCE;
	}
	else
	{
		jmax = j;
		jmin = AI_VIEW_DISTANCE;
	}
	for (int ii = imin; ii < imax; ii++)
	{
		for (int jj = jmin; jj < jmax; jj++)
		{
			if (surroundings[ii][jj] == BUILDING)
				fields.push_back((z + jj - AI_VIEW_DISTANCE) * FIELD_SIZE + (x + ii - AI_VIEW_DISTANCE));
		}
	}

	return isVisible(x1, z1, x + i - AI_VIEW_DISTANCE - FIELD_SIZE / 2 + 0.5f, z + j - AI_VIEW_DISTANCE - FIELD_SIZE / 2 + 0.5f, fields);
}

void Enemy::draw() const
{
	Tank::draw();

	if (DEBUG_ENEMY)
	{
		// draw all fields expanded in A* search
		for(unsigned int i = 0; i < _AStarFields.size(); i++)
		{
			int idx = _AStarFields[i];
			float x = float(idx % FIELD_SIZE);
			float z = floor( float(idx)/float(FIELD_SIZE) );
			float offset = float(FIELD_SIZE)/2.0;

			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_LINE_STRIP);
			glVertex3f(x - offset, 0.4, z - offset);
			glVertex3f(x + 1.0 - offset, 0.4, z - offset);
			glVertex3f(x + 1.0 - offset, 0.4, z + 1.0 - offset);
			glVertex3f(x - offset, 0.4, z + 1.0 - offset);
			glVertex3f(x - offset, 0.4, z - offset);
			glEnd();
			glPopMatrix();
		}

		// draw field chosen to aim at in A* search
		if (_AStarAimField > 0)
		{
			int idx = _AStarAimField;
			float x = float(idx % FIELD_SIZE);
			float z = floor( float(idx)/float(FIELD_SIZE) );
			float offset = float(FIELD_SIZE)/2.0;

			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_LINES);
			glVertex3f(x - offset, 0.4, z - offset);
			glVertex3f(x + 1.0 - offset, 0.4, z + 1.0 - offset);
			glVertex3f(x + 1.0 - offset, 0.4, z - offset);
			glVertex3f(x - offset, 0.4, z + 1.0 - offset);
			glEnd();
			glPopMatrix();
		}

		// draw field chosen to be best in A* search
		if (_AStarBestField > 0)
		{
			int idx = _AStarBestField;
			float x = float(idx % FIELD_SIZE);
			float z = floor( float(idx)/float(FIELD_SIZE) );
			float offset = float(FIELD_SIZE)/2.0;

			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_QUADS);
			glVertex3f(x - offset, 0.4, z - offset);
			glVertex3f(x + 1.0 - offset, 0.4, z - offset);
			glVertex3f(x + 1.0 - offset, 0.4, z + 1.0 - offset);
			glVertex3f(x - offset, 0.4, z + 1.0 - offset);
			glEnd();
			glPopMatrix();
		}
	}
}

void Enemy::activateMaterial() const
{
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, _materialAmbientEnemy );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, _materialDiffuseEnemy );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, _materialSpecularEnemy );
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, _materialEmissionEnemy );
    glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, _shininessEnemy );
}

GRAPHICSLAB_NAMESPACE_END
