

# File scheduler.hpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**scheduler.hpp**](scheduler_8hpp.md)

[Go to the documentation of this file](scheduler_8hpp.md)

```C++

#ifndef ROBOT_DART_SCHEDULER_HPP
#define ROBOT_DART_SCHEDULER_HPP

#include <robot_dart/utils.hpp>

#include <chrono>
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

        double step();

        void reset(double dt, bool sync = false, double current_time = 0., double real_time = 0.);

        void set_sync(bool enable) { _sync = enable; }
        bool sync() const { return _sync; }

        double current_time() const { return _simu_start_time + _current_time; }
        double next_time() const { return _simu_start_time + _current_time + _dt; }
        double real_time() const { return _real_start_time + _real_time; }
        double dt() const { return _dt; }
        // 0.8x => we are simulating at 80% of real time
        double real_time_factor() const { return _dt / it_duration(); }
        // time for a single iteration (wall-clock)
        double it_duration() const { return _average_it_duration * 1e-6; }
        // time of the last iteration (wall-clock)
        double last_it_duration() const { return _it_duration * 1e-6; }

    protected:
        double _current_time = 0., _simu_start_time = 0., _real_time = 0., _real_start_time = 0., _it_duration = 0.;
        double _average_it_duration = 0.;
        double _dt;
        int _current_step = 0;
        bool _sync;
        int _max_frequency = -1;
        clock_t::time_point _start_time;
        clock_t::time_point _last_iteration_time;
    };
} // namespace robot_dart

#endif

```

