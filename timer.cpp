/*
 * Timer definition
 */

// Headers
#include <algorithm>
#include "timer.hpp"

// Functions definition
Timer::Timer()
{

}

void Timer::set_device(occa::device device_)
{
    // Set device
    device = device_;

    // Measure synchronization time
    const int num_tests = 24;
    double sync_time[num_tests];

    for (int i = 0; i < num_tests; i++)
    {
        std::chrono::high_resolution_clock::time_point sync_start = std::chrono::high_resolution_clock::now();
        device.finish();
        std::chrono::high_resolution_clock::time_point sync_stop = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> t_elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(sync_stop - sync_start);

        sync_time[i] = t_elapsed.count();
    }

    std::sort(sync_time, sync_time + num_tests);

    if (num_tests % 2 == 0)
        t_sync = 0.5 * (sync_time[(num_tests - 1) / 2] + sync_time[num_tests / 2]);
    else
        t_sync = sync_time[num_tests / 2];
}

void Timer::start(const char *name)
{
    device.finish();
    t_start[name] = std::chrono::high_resolution_clock::now();
}

void Timer::stop(const char *name)
{
    device.finish();
    t_stop[name] = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> t_elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t_stop[name] - t_start[name]);

    if (t_total.find(name) == t_total.end())
        t_total[name] = 0.0;

    t_total[name] += t_elapsed.count() - t_sync;
}

void Timer::reset(const char *name)
{
    t_total[name] = 0.0;
}

double Timer::total(const char *name)
{
    return t_total[name];
}
