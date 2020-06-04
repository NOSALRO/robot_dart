#include <robot_dart/scheduler.hpp>

namespace robot_dart {
    bool Scheduler::schedule(int frequency)
    {
        if (_max_frequency == -1 && _sync)
            _start = clock_t::now();

        _max_frequency = std::max(_max_frequency, frequency);
        double period = std::round((1. / frequency) / _dt);

        ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(
            period >= 1. && "Time-step is too big for required frequency.");

        if (_current_step % int(period) == 0)
            return true;

        return false;
    }

    void Scheduler::reset(double dt, bool sync, double current_time)
    {
        ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(dt > 0. && "Time-step needs to be bigger than zero.");

        _current_time = 0.;
        _simu_start_time = current_time;
        _current_step = 0;
        _max_frequency = -1;

        _dt = dt;
        _sync = sync;
    }

    void Scheduler::step()
    {
        _current_time += _dt;
        _current_step += 1;

        if (_sync) {
            auto expected = std::chrono::microseconds(int(_current_time * 1e6));
            auto end = clock_t::now();
            std::chrono::duration<double, std::micro> real = end - _start;
            std::chrono::duration<double, std::micro> adjust = expected - real;
            if (adjust.count() > 0)
                std::this_thread::sleep_for(adjust);
        }
    }

} // namespace robot_dart