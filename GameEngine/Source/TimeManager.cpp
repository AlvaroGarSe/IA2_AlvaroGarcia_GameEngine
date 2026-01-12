#include "TimeManager.h"
TimeManager::TimeManager()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;

    mLastCounter = NULL;
    mLastTicksMs = 0;
    mDeltaSeconds = 0.0;
	mDeltaMs = 0;
}
Uint32 TimeManager::TickFrame()
{
    Uint32 now = SDL_GetTicks();
    Uint32 delta = now - mLastCounter;
    mLastTicksMs = now;
    mDeltaMs = delta;
    return delta;
}
Uint32 TimeManager::DeltaMs() const
{
	return mDeltaMs;
}
void TimeManager::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;

    mLastTicksMs = SDL_GetTicks();
    mLastCounter = SDL_GetPerformanceCounter();

	mLastCounter = SDL_GetPerformanceCounter();
    mDeltaSeconds = 0.0;
	mDeltaMs = 0;
}
Uint32 TimeManager::getTicks()
{
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if (mStarted)
    {
        //If the timer is paused
        if (mPaused)
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

double TimeManager::TickFrameSeconds()
{
    const Uint64 now = SDL_GetPerformanceCounter();
    const Uint64 freq = SDL_GetPerformanceFrequency();

    Uint64 counterDelta = now - mLastCounter;
    mLastCounter = now;

    mDeltaSeconds = (freq > 0) ? (double)counterDelta / (double)freq : 0.0;
    mDeltaMs = (Uint32)(mDeltaSeconds * 1000.0);

    return mDeltaSeconds;
}