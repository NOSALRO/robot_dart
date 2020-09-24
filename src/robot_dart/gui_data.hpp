#ifndef ROBOT_DART_SIMU_GUI_DATA_HPP
#define ROBOT_DART_SIMU_GUI_DATA_HPP

#include "robot_dart_simu.hpp"

#include <unordered_map>
#include <vector>

#include <Eigen/Geometry>

#include <dart/dynamics/BodyNode.hpp>
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
            std::unordered_map<Robot*, std::vector<std::pair<dart::dynamics::BodyNode*, double>>> robot_axes;
            std::vector<std::shared_ptr<simu::TextData>> text_drawings;

        public:
            std::shared_ptr<simu::TextData> add_text(const std::string& text, const Eigen::Affine2d& tf = Eigen::Affine2d::Identity(), Eigen::Vector4d color = Eigen::Vector4d(1, 1, 1, 1))
            {
                text_drawings.emplace_back(new TextData{text, tf, color});

                return text_drawings.back();
            }

            void remove_text(const std::shared_ptr<simu::TextData>& data)
            {
                for (size_t i = 0; i < text_drawings.size(); i++) {
                    if (text_drawings[i] == data) {
                        text_drawings.erase(text_drawings.begin() + i);
                        return;
                    }
                }
            }

            void remove_text(size_t index)
            {
                if (index >= text_drawings.size())
                    return;
                text_drawings.erase(text_drawings.begin() + index);
            }

            void update_robot(const std::shared_ptr<Robot>& robot)
            {
                auto robot_ptr = &*robot;
                auto skel = robot->skeleton();
                bool cast = robot->cast_shadows();
                bool ghost = robot->ghost();

                for (size_t i = 0; i < skel->getNumBodyNodes(); ++i) {
                    auto bd = skel->getBodyNode(i);
                    auto& shapes = bd->getShapeNodesWith<dart::dynamics::VisualAspect>();
                    for (size_t j = 0; j < shapes.size(); j++) {
                        robot_data[shapes[j]] = {cast, ghost};
                    }
                }

                auto& axes = robot->drawing_axes();
                if (axes.size() > 0)
                    robot_axes[robot_ptr] = axes;
            }

            void remove_robot(const std::shared_ptr<Robot>& robot)
            {
                auto robot_ptr = &*robot;
                auto skel = robot->skeleton();
                for (size_t i = 0; i < skel->getNumShapeNodes(); ++i) {
                    auto shape = skel->getShapeNode(i);
                    auto shape_iter = robot_data.find(shape);
                    if (shape_iter != robot_data.end())
                        robot_data.erase(shape_iter);
                }

                auto iter = robot_axes.find(robot_ptr);
                if (iter != robot_axes.end())
                    robot_axes.erase(iter);
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

            std::vector<std::pair<dart::dynamics::BodyNode*, double>> drawing_axes() const
            {
                std::vector<std::pair<dart::dynamics::BodyNode*, double>> axes;
                for (std::pair<Robot*, std::vector<std::pair<dart::dynamics::BodyNode*, double>>> elem : robot_axes) {
                    axes.insert(axes.end(), elem.second.begin(), elem.second.end());
                }

                return axes;
            }

            const std::vector<std::shared_ptr<simu::TextData>>& drawing_texts() const { return text_drawings; }
        };
    } // namespace simu
} // namespace robot_dart

#endif