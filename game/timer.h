/**
 * @file
 * @author (c)2011, by Raymond Løberg
 *
 * Header for the timer module
 */
#ifndef TIMER_H
#define TIMER_H
// Global includes
#include <time.h>

#ifdef FPS_DEF
#define TIMER_SPEED FPS_DEF
#else
#define TIMER_SPEED 100
#endif

/// Timer module. Handles timer related stuff
class cmTimer {
    private:
        double delta_var;
    public:
        int setup(); // Begins a timer
        int get_timer(); // Returns the current timer position
        void reset_timer(); // Sets the timer back to zero
        double delta(); // Returns the time taken since the last call to delta()
        
        void sleep(double seconds);
        
        // Other functions
        double get_ptime(); // Returns the program elapsed time in seconds (NOTE: Cross platform?)
        double fps();	// Returns the frames processed per second
        
        double cfps;
        int last_timer; // Set after each call to delta()
};

extern class cmTimer mTimer;

#endif 
