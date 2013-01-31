/**
 * sound.cpp
 *
 * Author   :   Fabian Gehri
 * Date     :   11.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "sound.hpp"
#include "config.hpp"
#include <stdlib.h>             // I needed this to get atexit() working..
#include <stdio.h>

// Includes
//#include "gl_includes.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

// static variables need to be declared here as well
const std::string Sound::FOLDER = "..\\..\\data\\sound\\";
ALuint Sound::_buffers[NR_SOUNDS];
Sound::ALuintVector Sound::_sourcesDynamic;
Sound::ALuintVector Sound::_sourcesStatic;
const ALfloat Sound::_velocity[3] = { 0.0f, 0.0f, 0.0f };

void Sound::initialize()
{

	// initialize ALUT and clear error bit
	alutInit(NULL, NULL);
	alutGetError();

	// load sounds
	loadSounds();

	// setup exit procedure
	atexit(killALData);

}

void Sound::killALData()
{

	// delete buffers
	alDeleteBuffers(NR_SOUNDS, &_buffers[0]);

	// delete sources
	for (ALuintVector::iterator iter = _sourcesDynamic.begin(); iter != _sourcesDynamic.end(); ++iter)
	{
		alDeleteSources(1, &(*iter));
		checkALError();
	}
	_sourcesDynamic.clear();
	for (ALuintVector::iterator iter = _sourcesStatic.begin(); iter != _sourcesStatic.end(); ++iter)
	{
		alDeleteSources(1, &(*iter));
		checkALError();
	}
	_sourcesStatic.clear();

	// shutdown ALUT library
	alutExit();

}

void Sound::cleanupSources()
{

	// delete all sources that have stopped playing
	for (ALuintVector::iterator iter = _sourcesDynamic.begin(); iter != _sourcesDynamic.end(); )
	{
		ALint state;
		alGetSourcei(*iter, AL_SOURCE_STATE, &state);
		checkALError();
		if (state == AL_STOPPED)
		{
			alDeleteSources(1, &(*iter));
			checkALError();
			iter = _sourcesDynamic.erase(iter);
		}
		else
			++iter;
	}

}

void Sound::loadSounds()
{

	// create buffers with all sounds
	std::string fileName = FOLDER + "engine_idle.wav";
	_buffers[ENGINE_IDLE] = alutCreateBufferFromFile(fileName.c_str());
	fileName = FOLDER + "engine_running.wav";
	_buffers[ENGINE_RUNNING] = alutCreateBufferFromFile(fileName.c_str());
	fileName = FOLDER + "crash.wav";
	_buffers[CRASH] = alutCreateBufferFromFile(fileName.c_str());
	fileName = FOLDER + "gun.wav";
	_buffers[GUN] = alutCreateBufferFromFile(fileName.c_str());
	fileName = FOLDER + "projectile.wav";
	_buffers[PROJECTILE] = alutCreateBufferFromFile(fileName.c_str());
	fileName = FOLDER + "grenade_launcher.wav";
	_buffers[GRENADE_LAUNCHER] = alutCreateBufferFromFile(fileName.c_str());
	fileName = FOLDER + "grenade.wav";
	_buffers[GRENADE] = alutCreateBufferFromFile(fileName.c_str());

}

void Sound::setListener(float* position, float* orientation)
{
	/*if (DEBUG_SOUND)
		printf("position = (%f, %f, %f) \n orientation = (%f, %f, %f, %f, %f, %f)\n", position[0], position[1], position[2], orientation[0], orientation[1], orientation[2], orientation[3], orientation[4], orientation[5]);*/

	alListenerfv(AL_POSITION, position);
	checkALError();
	alListenerfv(AL_VELOCITY, _velocity);
    checkALError();
	alListenerfv(AL_ORIENTATION, orientation);
	checkALError();


}

ALuint Sound::addEngineSource(Point position)
{

	// create source
	ALuint source;
	alGenSources (1, &source);
	checkALError();


	/*if (DEBUG_SOUND)
		printf("position = (%f, %f, %f)\n", position.x / position.w, position.y / position.w, position.z / position.w);*/


	// setup source properties
	alSourcei (source, AL_BUFFER, _buffers[ENGINE_IDLE]);
	checkALError();
	alSourcef (source, AL_PITCH, 1.0);
	checkALError();
	alSourcef (source, AL_GAIN, 0.5);
	checkALError();
	alSource3f(source, AL_POSITION, position.x / position.w, position.y / position.w, position.z / position.w);
	checkALError();
	alSourcefv(source, AL_VELOCITY, _velocity);
	checkALError();
	alSourcei (source, AL_LOOPING, AL_TRUE);
	checkALError();

	// save the static source
	_sourcesStatic.push_back(source);

	// play the source
	alSourcePlay(source);
	checkALError();

	return source;

}

void Sound::deleteStaticSource(ALuint source)
{

	alDeleteSources(1, &source);
	checkALError();

	for (unsigned int i = 0; i < _sourcesStatic.size(); i++)
		if (_sourcesStatic[i] == source)
		{
			_sourcesStatic.erase(_sourcesStatic.begin() + i);
			break;
		}

}

void Sound::moveStaticSource(ALuint source, Point newPosition)
{

	alSource3f(source, AL_POSITION, newPosition.x / newPosition.w, newPosition.y / newPosition.w, newPosition.z / newPosition.w);
	checkALError();

}

void Sound::setEngineRunning(ALuint source, bool running)
{

	alSourceStop(source);
	checkALError();

	if (running)
	{
		alSourcei (source, AL_BUFFER, _buffers[ENGINE_IDLE]);
		checkALError();
		alSourcef (source, AL_GAIN, 1.0);
		checkALError();
	}
	else
	{
		alSourcei (source, AL_BUFFER, _buffers[ENGINE_IDLE]);
		checkALError();
		alSourcef (source, AL_GAIN, 0.5);
		checkALError();
	}

	alSourcePlay(source);
	checkALError();

}

void Sound::playSound(int bufferNr, Point sourcePos)
{
	playSound(bufferNr, sourcePos, 1.0f);
}

void Sound::playSound(int bufferNr, Point sourcePos, float gain)
{

	if (0 <= bufferNr && bufferNr < NR_SOUNDS)
	{
		// before creating a new source we delete all sources that we don't need anymore (i.e. that stopped playing)
		cleanupSources();

		// create source
		ALuint source;
		alGenSources (1, &source);
		checkALError();

		if (DEBUG_SOUND)
			printf("sourcePos = (%f, %f, %f)\n", sourcePos.x / sourcePos.w, sourcePos.y / sourcePos.w, sourcePos.z / sourcePos.w);

		// setup source properties
		alSourcei (source, AL_BUFFER, _buffers[bufferNr]);
		checkALError();
		alSourcef (source, AL_PITCH, 1.0);
		checkALError();
		alSourcef (source, AL_GAIN, gain);
		checkALError();
		alSource3f(source, AL_POSITION, sourcePos.x / sourcePos.w, sourcePos.y / sourcePos.w, sourcePos.z / sourcePos.w);
		checkALError();
		alSourcefv(source, AL_VELOCITY, _velocity);
		checkALError();
		alSourcei (source, AL_LOOPING, AL_FALSE);
		checkALError();

		// save the dynamic source
		_sourcesDynamic.push_back(source);

		// play the source
		alSourcePlay(source);
		checkALError();
	}

}

void Sound::playGun(Point position)
{

	playSound(GUN, position, 0.15f);

}

void Sound::playProjectile(Point position)
{

	playSound(PROJECTILE, position, 0.5f);

}

void Sound::playGrenadeLauncher(Point position)
{

	playSound(GRENADE_LAUNCHER, position, 0.2f);

}

void Sound::playGrenade(Point position)
{

	playSound(GRENADE, position);

}

void Sound::playCollision(Point position)
{

	playSound(CRASH, position);

}

void Sound::checkALError()
{

	ALenum error = alGetError();
	if (DEBUG_SOUND && error != AL_NO_ERROR)
	{
		switch(error)
		{
			case AL_INVALID_NAME:
				printf("AL_INVALID_NAME\n");
				break;
			case AL_INVALID_ENUM:
				printf("AL_INVALID_ENUM\n");
				break;
			case AL_INVALID_VALUE:
				printf("AL_INVALID_VALUE\n");
				break;
			case AL_INVALID_OPERATION:
				printf("AL_INVALID_OPERATION\n");
				break;
			case AL_OUT_OF_MEMORY:
				printf("AL_OUT_OF_MEMORY\n");
				break;
		}
	}

}

GRAPHICSLAB_NAMESPACE_END


