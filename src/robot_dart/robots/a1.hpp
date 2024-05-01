#ifndef ROBOT_DART_ROBOTS_A1_HPP
#define ROBOT_DART_ROBOTS_A1_HPP

#include "robot_dart/robot.hpp"
#include "robot_dart/sensor/imu.hpp"

namespace robot_dart {
    namespace robots {
        class A1 : public Robot {
        public:
            A1(size_t frequency = 1000, const std::string& urdf = "unitree_a1/a1.urdf", const std::vector<std::pair<std::string, std::string>>& packages = {{"a1_description", "unitree_a1/a1_description"}});

            void reset() override;

            const sensor::IMU& imu() const { return *_imu; }

        protected:
            std::shared_ptr<sensor::IMU> _imu;
        };
    } // namespace robots
} // namespace robot_dart
#endif
