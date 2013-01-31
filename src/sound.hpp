/**
 * sound.hpp
 * 
 * Author   :   Fabian Gehri
 * Date     :   11.05.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_SOUND_HPP
#define GRAPHICSLAB_SOUND_HPP

// Includes
#include "defs.hpp"
#include "types.hpp"
#include <vector>
#include <string>

// OpenAL includes
//#include <al.h>
//#include <alc.h>
#include <AL/alut.h>


GRAPHICSLAB_NAMESPACE_BEGIN

// number of different sounds
#define NR_SOUNDS 7

// indices of sounds
#define ENGINE_IDLE 0
#define ENGINE_RUNNING 1
#define CRASH 2
#define GUN 3
#define PROJECTILE 4
#define GRENADE_LAUNCHER 5
#define GRENADE 6

// notice: all velocities (listener and sources) are always set to zero, so there is no doppler effect

/** Sound */
class Sound
{
private:
	// location of sound files
	static const std::string FOLDER;

	// buffers for all sounds
	static ALuint _buffers[NR_SOUNDS];

	// dynamically used sources (used for bullets and other things whose sounds do not loop)
	typedef std::vector<ALuint> ALuintVector;
	static ALuintVector _sourcesDynamic;
	
	// statically used sources (used for tanks because engine sound loops)
	static ALuintVector _sourcesStatic;

	// null velocity
	static const ALfloat _velocity[3];

	// load all sounds into the buffers
	static void loadSounds();

	// deletes all buffers and sources and shuts down ALUT library
	static void killALData();

	// delete all sources that have stopped playing
	static void cleanupSources();

	// private methods for playing sounds
	static void playSound(int bufferNr, Point sourcePos);
	static void playSound(int bufferNr, Point sourcePos, float gain);

	// checks if there was an AL error and if so, prints the error
	static void checkALError();
public:
    // initializes ALUT context and loads sounds
	static void initialize();

	// set listener parameters (orientation: first 3 are "at", last 3 are "up")
	static void setListener(float* position, float* orientation);

	// add new static source for engine sound at specified position and return its index
	static ALuint addEngineSource(Point position);

	// delete static source
	static void deleteStaticSource(ALuint source);

	// moves a static source to the specified position
	static void moveStaticSource(ALuint source, Point newPosition);

	// switches a static source between idle and running engine sound
	static void setEngineRunning(ALuint source, bool running);

	// play the sound of a machine gun at specified position
	static void playGun(Point position);

	// play the sound of a hitting projectile at specified position
	static void playProjectile(Point position);

	// play the sound of a grenade launcher at specified position
	static void playGrenadeLauncher(Point position);

	// play the sound of an exploding grenade at specified position
	static void playGrenade(Point position);

	// play the sound of a collision at specifies position
	static void playCollision(Point position);

}; // class Sound

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_SOUND_HPP


