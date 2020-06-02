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

        bool operator()(int frequency)
        {
            if (_max_frequency == -1 && _sync)
                _start = clock_t::now();

            _max_frequency = std::max(_max_frequency, frequency);

            double period = std::floor((1. / frequency) / _dt);
            ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(period >= 1. && "Time-step is too big for required frequency.");

            if (_step % int(period) == 0)
                return true;

            return false;
        }

        void sync()
        {
            _time += _dt;
            _step += 1;
            _do_sync();
        }

        void reset(double dt, bool sync = false)
        {
            ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(dt > 0. && "Time-step needs to be bigger than zero.");

            _time = 0.;
            _step = 0;
            _max_frequency = -1;

            _dt = dt;
            _sync = sync;
        }

        double time() const { return _time; }
        double next_time() const { return _time + _dt; }
        double dt() const { return _dt; }

    protected:
        void _do_sync()
        {
            if (_sync) {
                auto expected = std::chrono::microseconds(int(_time * 1e6));
                auto end = clock_t::now();
                std::chrono::duration<double, std::micro> real = end - _start;
                std::chrono::duration<double, std::micro> adjust = expected - real;
                if (adjust.count() > 0)
                    std::this_thread::sleep_for(adjust);
            }
        }

        double _time = 0.;
        double _dt;
        int _step = 0;
        bool _sync;
        int _max_frequency = -1;
        clock_t::time_point _start;
    };
} // namespace robot_dart

#endif
