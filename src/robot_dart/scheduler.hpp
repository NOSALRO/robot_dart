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
        /// and increase the counter
        void step();

        void reset(double dt, bool sync = false);
        void set_sync(bool enable) { _sync = enable; }
        double current_time() const { return _current_time; }
        double next_time() const { return _current_time + _dt; }
        double dt() const { return _dt; }

    protected:
        double _current_time = 0.;
        double _dt;
        int _current_step = 0;
        bool _sync;
        int _max_frequency = -1;
        clock_t::time_point _start;
    };
} // namespace robot_dart

#endif
