/**
 * parameters.hpp
 *
 * Author   :   Fabian Gehri
 * Date     :   10.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_PARAMETERS_HPP
#define GRAPHICSLAB_PARAMETERS_HPP

// game parameters
#define MAX_HEALTH 100
#define NR_LEVELS 10
#define MAX_NR_ENEMIES 10
//#define NR_ENEMIES 5
//#define KILLS_TO_WIN 10
//#define DEATHS_TO_LOSE 10

// tank parameters
//#define MAX_SPEED 20.0 // max speed is not needed, it is determined by: acceleration / (1 - friction coeff)
//#define MAX_SPEED_REVERSE -5.0
#define ACCELERATION 1.0
#define DECELERATION 0.5
#define FRICTION_DECELERATION_COEFF 0.9
#define TURN_ACCELERATION 15.0
#define FRICTION_TURN_DECELERATION_COEFF 0.8
#define COLLISION_DECELERATION_COEFF 0.3
#define FIRING_DELAY_GRENADE    1.0
#define FIRING_DELAY_MG         0.5
#define MG_DAMAGE               2
#define GRENADE_DAMAGE          20
#define POWERUP_HEALTH_BONUS	30

// scene parameters
#define MAX_INSERT_TRIALS  1000     // maximal number of random trials to find a free
                                    // field for an enemy

// landscape parameters
//-----------------------
#define FIELD_SIZE 128
#define N_POWERUPS 20

#define HOUSETYPE_HI 80		//where the tall buildings stand (threshold value <100)
#define HOUSETYPE_MED 40	//medium building
#define HOUSETYPE_LOW 20	//small building, everything outside will be place!


// AI parameters
#define AI_VIEW_DISTANCE 10
#define AI_MAX_STEPS 100
#define AI_COLL_WIDTH 2.0f
#define AI_SPEED_FACTOR 0.5f
#define AI_GRENADE_DIST 6.0f
//#define AI_COST_TO_GOAL_EXP 1.0f

#endif  //  GRAPHICSLAB_PARAMETERS_HPP


