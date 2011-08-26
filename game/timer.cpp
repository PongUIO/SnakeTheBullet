/**
 * @file
 * @author (c)2011, by Raymond Løberg
 *
 * Timer module, handles portable timing.
 */
// Global includes
#include <math.h>

// Other includes
#include "timer.h"

class cmTimer mTimer;

#if defined(unix) || defined(__unix__) || defined(__unix)
	#include <unistd.h>
#endif

#if (_POSIX_VERSION) >= 200112L // Which supports gettimeofday()
	#include <sys/time.h>
	
	#define USEC 1000000.0
	
	struct timeval curTime;
	struct timeval prevTime;
	struct timeval startTime;
	struct timeval nowTime;
	double __timer_delta;
	double __frame_time;
	double __fps_timer;
	double proctime;
	
	double timeDiff(struct timeval &a, struct timeval &b) {
		double t1 = double(a.tv_sec)+double(a.tv_usec)/USEC;
		double t2 = double(b.tv_sec)+double(b.tv_usec)/USEC;
		
		return fabs(t2-t1);
	}
	
	void setTimer() {
		gettimeofday(&prevTime, NULL);
		gettimeofday(&startTime, NULL);
	}
	
	void timer_init() {
		setTimer();
	}

	bool newFrame() {
		gettimeofday(&curTime, NULL);
		if(curTime.tv_usec > prevTime.tv_usec) {
			__timer_delta = double(curTime.tv_usec-prevTime.tv_usec)/double(1.0e6); // Microsec timer
		}else {
			__timer_delta = double((1.0e6+curTime.tv_usec)-prevTime.tv_usec)/double(1.0e6); // Microsec timer
		}
		
		if(__timer_delta > 1.0/double(TIMER_SPEED)) return true;
		else return false;
	}
	
	double getDelta() {
		__frame_time = __timer_delta;
		newFrame();
		prevTime.tv_usec = curTime.tv_usec;
		return __timer_delta;
	}
	
	double getProcTime() {
		gettimeofday(&nowTime, NULL);
		return timeDiff(nowTime,startTime);
	}
	
	double getFps(double delta) {
	    
	    return 1.0/delta;
	}
#else
	/// This is a raw timer system.
	/// It pretends to do something,
	/// although it really doesn't.
	/// Overload these to create timer systems for other platforms
	
    void timer_init() {
    }
    
    void setTimer() {
    }
    
    bool newFrame() {
        return true;
    }
    
    double getDelta() {
        return(0.01);
    }
    
    double getProcTime() {
    	return 0.0;
    }
    
    double getFps() {
        return 0.0;
    }
#endif


// Class functions
/**
 * Initializes the timer
 */
int cmTimer::setup()
{
    timer_init();
    return 1;
}

/**
 * Returns the timer value
 */
int cmTimer::get_timer(void)
{
    if(newFrame()) {
        delta_var = getDelta();
        return 1;
    }else
        return 0;
}

/**
 * Resets the timer to zero, and sets last_timer for the delta function
 */
void cmTimer::reset_timer(void)
{
    getDelta();
}

/**
 * Computes the delta since the last call to the function
 * @returns The delta time in seconds
 */
double cmTimer::delta(void)
{
    return delta_var;
}

/**
 * Sleeps for a specific number of seconds
 */
void cmTimer::sleep(double seconds)
{
	double waitTo = get_ptime()+seconds;
	while(get_ptime() < waitTo)
		;
}


/**
 * Returns the number of seconds the program has been running
 */
double cmTimer::get_ptime(void)
{
	return(getProcTime());
}

/**
 * Returns the number of frames per second
 */
double cmTimer::fps()
{
	return getFps(delta_var);
}
