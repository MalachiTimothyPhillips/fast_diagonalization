/*
 * Timer class declaration
 */

// Headers
#include <chrono>
#include <occa.hpp>

// Class definition
#ifndef TIMER_HPP
#define TIMER_HPP

class Timer
{
    private:
        occa::device device;
        std::map<const char*, std::chrono::high_resolution_clock::time_point> t_start;
        std::map<const char*, std::chrono::high_resolution_clock::time_point> t_stop;
        std::map<const char*, double> t_total;
        double t_sync;

    public:
        // Constructor
        Timer();

        // Utility functions
        void set_device(occa::device);
        void start(const char*);
        void stop(const char*);
        void reset(const char*);
        double total(const char*);

};

#endif
