#ifndef ROBOT_DART_SIMU_GUI_DATA_HPP
#define ROBOT_DART_SIMU_GUI_DATA_HPP

#include "robot_dart_simu.hpp"

#include <unordered_map>

#include <dart/dynamics/ShapeNode.hpp>

namespace robot_dart {
    namespace simu {
        struct GUIData {
        private:
            struct RobotData {
                bool casting_shadows;
                bool is_ghost;
            };

            std::unordered_map<dart::dynamics::ShapeNode*, RobotData> robot_data;

        public:
            void update_robot(const std::shared_ptr<robots::Robot>& robot)
            {
                auto skel = robot->skeleton();
                bool cast = robot->cast_shadows();
                bool ghost = robot->ghost();
                for (size_t i = 0; i < skel->getNumShapeNodes(); ++i) {
                    auto shape = skel->getShapeNode(i);
                    robot_data[shape] = {cast, ghost};
                }
            }

            void remove_robot(const std::shared_ptr<robots::Robot>& robot)
            {
                auto skel = robot->skeleton();
                for (size_t i = 0; i < skel->getNumShapeNodes(); ++i) {
                    auto shape = skel->getShapeNode(i);
                    auto shape_iter = robot_data.find(shape);
                    if (shape_iter != robot_data.end())
                        robot_data.erase(shape_iter);
                }
            }

            bool cast_shadows(dart::dynamics::ShapeNode* shape) const
            {
                auto shape_iter = robot_data.find(shape);
                if (shape_iter != robot_data.end())
                    return robot_data.at(shape).casting_shadows;
                // if not in the array, cast shadow by default
                return true;
            }

            bool ghost(dart::dynamics::ShapeNode* shape) const
            {
                auto shape_iter = robot_data.find(shape);
                if (shape_iter != robot_data.end())
                    return robot_data.at(shape).is_ghost;
                // if not in the array, the robot is not ghost by default
                return false;
            }
        };
    } // namespace simu
} // namespace robot_dart

#endif