#ifndef ROBOT_DART_SCHEDULER_HPP
#define ROBOT_DART_SCHEDULER_HPP

#include <robot_dart/utils.hpp>

#include <chrono>
#include <cmath>
#include <thread>

namespace robot_dart {
    class Scheduler {
    protected:
        using clock_t = std::chrono::high_resolution_clock;

    public:
        Scheduler(double dt, bool sync = false) : _dt(dt), _sync(sync)
        {
            ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(_dt > 0. && "Time-step needs to be bigger than zero.");
        }

        bool operator()(int frequency) { return schedule(frequency); };
        bool schedule(int frequency);

        /// call this at the end of the loop (see examples)
        /// this will synchronize with real time if requested
        /// and increase the counter;
        /// returns the real-time (in seconds)
        double step();

        void reset(double dt, bool sync = false, double current_time = 0.);

        void set_sync(bool enable) { _sync = enable; }
        bool sync() { return _sync; }

        double current_time() const { return _simu_start_time + _current_time; }
        double next_time() const { return _simu_start_time + _current_time + _dt; }
        double dt() const { return _dt; }

    protected:
        double _current_time = 0., _simu_start_time = 0.;
        double _dt;
        int _current_step = 0;
        bool _sync;
        int _max_frequency = -1;
        clock_t::time_point _start;
    };
} // namespace robot_dart

#endif
