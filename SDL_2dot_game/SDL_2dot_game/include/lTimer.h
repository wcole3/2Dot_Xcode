//
//  lTimer.h
//  Simple timer wrapper class
//
//  Created by William Cole on 12/2/18.
//  Copyright © 2018 William Cole. All rights reserved.
//

#ifndef lTimer_h
#define lTimer_h
using namespace std;

//class mimicing a stopwatch
class lTimer{
public:
    lTimer();
    /**
        Resets the timer and starts it running
     */
    void start();
    /**
        Paused the timer
     */
    void pause();
    /**
        Unpaused the timer
     */
    void unpause();
    /**
        Stop the timer and reset it
     */
    void stop();
    /**
        Get the current time
     
        @return the time since start in milliseconds
     */
    Uint32 getTime();
    /**
        @return true if the timer is running
     */
    bool isStarted(){return started;};
    /**
        @return true if the timer is paused
     */
    bool isPaused(){return paused;};
private:
    bool started;
    bool paused;
    Uint32 startTime;
    Uint32 pauseTime;
};

lTimer::lTimer(){
    started=false;
    paused=false;
    startTime=0;
    pauseTime=0;
}

void lTimer::start(){
    //need to start the timer running
    
    
        //if the timer hasnt started we start it
        startTime=SDL_GetTicks();
        started=true;
        paused=false;
        pauseTime=0;
    
    
}

void lTimer::pause(){
    if(isStarted() && !isPaused()){
        //need to pause the timer
        pauseTime=SDL_GetTicks()-startTime;
        paused=true;
        startTime=0;
    }
}

void lTimer::unpause(){
    if(isStarted() && isPaused()){
        paused=false;
        startTime=SDL_GetTicks()-pauseTime;
        pauseTime=0;
    }
}

void lTimer::stop(){
    started=false;
    paused=false;
    pauseTime=0;
    startTime=0;
    
}

Uint32 lTimer::getTime(){
    Uint32 time=0;
    if(isStarted()){
        if(isPaused()){
            time=pauseTime;
        }else{
            time=SDL_GetTicks()-startTime;
        }
    }
    return time;
}

#endif /* lTimer_h */
