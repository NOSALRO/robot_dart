#ifndef ROBOT_DART_DESCRIPTOR_BASE_DESCRIPTOR_HPP
#define ROBOT_DART_DESCRIPTOR_BASE_DESCRIPTOR_HPP

// for size_t
#include <cstddef>

namespace robot_dart {
    class RobotDARTSimu;

    namespace descriptor {

        struct BaseDescriptor {
        public:
            BaseDescriptor(size_t desc_dump = 1);
            virtual ~BaseDescriptor() {}

            virtual void operator()() = 0;

            size_t desc_dump() const;
            void set_desc_dump(size_t desc_dump);

            void set_simu(RobotDARTSimu* simu) { _simu = simu; }
            const RobotDARTSimu* simu() const { return _simu; }

        protected:
            RobotDARTSimu* _simu;
            size_t _desc_period;
        };
    } // namespace descriptor
} // namespace robot_dart

#endif
