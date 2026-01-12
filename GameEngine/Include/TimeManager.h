#pragma once
#include "SDL.h"
#include "Singleton.h"

class TimeManager:public Singleton<TimeManager>
{
    friend class Singleton<TimeManager>;

public:
    //The various clock actions
    void start();
    //void stop();
    //void pause();
    //void unpause();

    //Gets the timer's time
    Uint32 getTicks();
    double TickFrameSeconds();

    //Checks the status of the timer
    //bool isStarted();
    //bool isPaused();

    Uint32 TickFrame();
    Uint32 DeltaMs() const;

    

private:
    //Initializes variables
    TimeManager();

    //The clock time when the timer started
    Uint32 mStartTicks;

    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    Uint64 mLastCounter;
    Uint32 mDeltaMs;
    double mDeltaSeconds;
    Uint32 mLastTicksMs;


    //The timer status
    bool mPaused;
    bool mStarted;
};