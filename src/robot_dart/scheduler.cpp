#include <robot_dart/scheduler.hpp>

namespace robot_dart {
    bool Scheduler::schedule(int frequency)
    {
        if (_max_frequency == -1) {
            _start_time = clock_t::now();
            _last_iteration_time = _start_time;
        }

        _max_frequency = std::max(_max_frequency, frequency);
        double period = std::round((1. / frequency) / _dt);

        ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(
            period >= 1. && "Time-step is too big for required frequency.");

        if (_current_step % int(period) == 0)
            return true;

        return false;
    }

    void Scheduler::reset(double dt, bool sync, double current_time, double real_time)
    {
        ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(dt > 0. && "Time-step needs to be bigger than zero.");

        _current_time = 0.;
        _real_time = 0.;
        _simu_start_time = current_time;
        _real_start_time = real_time;
        _current_step = 0;
        _max_frequency = -1;
        _average_it_duration = 0.;

        _dt = dt;
        _sync = sync;
    }

    double Scheduler::step()
    {
        _current_time += _dt;
        _current_step += 1;

        auto end = clock_t::now();
        _it_duration = std::chrono::duration<double, std::micro>(end - _last_iteration_time).count();
        _last_iteration_time = end;
        _average_it_duration = _average_it_duration + (_it_duration - _average_it_duration) / _current_step;
        std::chrono::duration<double, std::micro> real = end - _start_time;
        if (_sync) {
            auto expected = std::chrono::microseconds(int(_current_time * 1e6));
            std::chrono::duration<double, std::micro> adjust = expected - real;
            if (adjust.count() > 0)
                std::this_thread::sleep_for(adjust);
        }

        _real_time = real.count() * 1e-6;
        return _real_start_time + _real_time;
    }

} // namespace robot_dart