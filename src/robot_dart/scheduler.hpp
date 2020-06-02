#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
namespace robot_dart {
    class Scheduler {
    public:
        Scheduler(double dt, bool sync = false) : _dt(dt), _sync(sync) {}

        bool operator()(int frequency)
        {
            if (_max_frequency == -1 && _sync)
                _start = clock_t::now();
            _max_frequency = std::max(_max_frequency, frequency);
            double period = std::floor((1. / frequency) / _dt);
            assert(period >= 1.); // if not we need a smaller dt!
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
        double time() const { return _time; }
        double next_time() const { return _time + _dt; }
        double dt() const { return _dt; }

    protected:
        void _do_sync()
        {
            if (_sync) {
                auto expected = std::chrono::microseconds(int(_time * 1e6));
                auto end = clock_t::now();
                std::chrono::duration<double> real = end - _start;
                auto adjust = expected - real;
                if (adjust.count() > 0)
                    std::this_thread::sleep_for(adjust);
            }
        }
        double _time = 0;
        double _dt;
        int _step = 0;
        bool _sync;
        int _max_frequency = -1;
        using clock_t = std::chrono::high_resolution_clock;
        clock_t::time_point _start;
    };
} // namespace robot_dart