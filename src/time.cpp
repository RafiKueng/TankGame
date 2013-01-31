/**
 * time.cpp
 * 
 * Author   :   Fatih Erol
 * Date     :   18.02.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "time.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

Time::Time()
{
    start();
}

void Time::start()
{
	GETTIME( _time );
}

float Time::getMilliseconds() const
{
	TIMETYPE now;
    GETTIME( now );
    
#ifdef WIN32
	ULARGE_INTEGER timeInteger, nowInteger, diffInteger;
	timeInteger.HighPart = _time.dwHighDateTime;
	timeInteger.LowPart = _time.dwLowDateTime;
	nowInteger.HighPart = now.dwHighDateTime;
	nowInteger.LowPart = now.dwLowDateTime;
	// Calculate difference in 100 nanoseconds' multiple
	diffInteger.QuadPart = nowInteger.QuadPart - timeInteger.QuadPart;
    
	// One millisecond has 10000 times 100 nanoseconds
	static const float hundredNanosPerMillisecond = 10000.0f;
	return ( diffInteger.QuadPart*1.0f/hundredNanosPerMillisecond );
#else
	return ( 1000.0f*( now.tv_sec - _time.tv_sec ) + ( now.tv_usec - _time.tv_usec )/1000.0f );
#endif
}

GRAPHICSLAB_NAMESPACE_END

