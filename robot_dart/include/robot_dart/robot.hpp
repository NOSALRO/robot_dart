#ifndef ROBOT_DART_ROBOT_HPP
#define ROBOT_DART_ROBOT_HPP

#include <dart/dart.hpp>
#include <dart/utils/urdf/urdf.hpp>
#include <dart/utils/sdf/SdfParser.hpp>
#include <boost/filesystem.hpp>
#include <Eigen/Core>
#include <string>
#include <fstream>
#include <streambuf>

namespace robot_dart {

    struct RobotDamage {
        RobotDamage() {}
        RobotDamage(const std::string& type, const std::string& data, void* extra = nullptr) : type(type), data(data), extra(extra) {}

        std::string type;
        std::string data;
        void* extra = nullptr;
    };

    class Robot {
    public:
        Robot() {}

        Robot(std::string model_file, std::vector<RobotDamage> damages, std::string robot_name = "robot") : _robot_name(robot_name), _skeleton(_load_model(model_file))
        {
            assert(_skeleton != nullptr);
            _set_damages(damages);
        }

        Robot(dart::dynamics::SkeletonPtr skeleton, std::vector<RobotDamage> damages, std::string robot_name = "robot") : _robot_name(robot_name), _skeleton(skeleton)
        {
            assert(_skeleton != nullptr);
            _skeleton->setName(robot_name);
            _set_damages(damages);
        }

        std::shared_ptr<Robot> clone() const
        {
            // safely clone the skeleton
            _skeleton->getMutex().lock();
            auto tmp_skel = _skeleton->clone();
            _skeleton->getMutex().unlock();
            auto robot = std::make_shared<Robot>();
            robot->_skeleton = tmp_skel;
            robot->_damages = _damages;
            robot->_robot_name = _robot_name;
            return robot;
        }

        dart::dynamics::SkeletonPtr skeleton()
        {
            return _skeleton;
        }

        std::vector<RobotDamage> damages() const
        {
            return _damages;
        }

        std::string name() const
        {
            return _robot_name;
        }

        void fix_to_world()
        {
            _skeleton->getRootBodyNode()->changeParentJointType<dart::dynamics::WeldJoint>();
        }

        void set_actuator_types(const std::vector<dart::dynamics::Joint::ActuatorType>& types)
        {
            assert(types.size() == _skeleton->getNumJoints());
            for (size_t i = 0; i < _skeleton->getNumJoints(); ++i) {
                _skeleton->getJoint(i)->setActuatorType(types[i]);
            }
        }

        void set_actuator_types(dart::dynamics::Joint::ActuatorType type)
        {
            for (size_t i = 0; i < _skeleton->getNumJoints(); ++i) {
                _skeleton->getJoint(i)->setActuatorType(type);
            }
        }

        void set_actuator_types(const std::vector<size_t>& indices, const std::vector<dart::dynamics::Joint::ActuatorType>& types)
        {
            assert(indices.size() == types.size());
            size_t jnt_num = _skeleton->getNumJoints();
            for (size_t i = 0; i < indices.size(); ++i) {
                assert(indices[i] > 0 && indices[i] < jnt_num);
                _skeleton->getJoint(indices[i])->setActuatorType(types[i]);
            }
        }

        void set_position_enforced(const std::vector<bool>& enforced)
        {
            assert(enforced.size() == _skeleton->getNumJoints());
            for (size_t i = 0; i < _skeleton->getNumJoints(); ++i) {
                _skeleton->getJoint(i)->setPositionLimitEnforced(enforced[i]);
            }
        }

        void set_position_enforced(bool enforced)
        {
            for (size_t i = 0; i < _skeleton->getNumJoints(); ++i) {
                _skeleton->getJoint(i)->setPositionLimitEnforced(enforced);
            }
        }

        void set_position_enforced(const std::vector<size_t>& indices, const std::vector<bool>& enforced)
        {
            assert(indices.size() == enforced.size());
            size_t jnt_num = _skeleton->getNumJoints();
            for (size_t i = 0; i < indices.size(); ++i) {
                assert(indices[i] > 0 && indices[i] < jnt_num);
                _skeleton->getJoint(indices[i])->setPositionLimitEnforced(enforced[i]);
            }
        }

        Eigen::Vector3d body_pos(std::string body_name) const
        {
            auto bd = _skeleton->getBodyNode(body_name);
            if (bd)
                return bd->getTransform().translation();

            return Eigen::Vector3d::Zero();
        }

        Eigen::Matrix3d body_rot(std::string body_name) const
        {
            auto bd = _skeleton->getBodyNode(body_name);
            if (bd)
                return bd->getTransform().linear();
            return Eigen::Matrix3d::Identity();
        }

        Eigen::Isometry3d body_trans(std::string body_name) const
        {
            auto bd = _skeleton->getBodyNode(body_name);
            if (bd)
                return bd->getTransform();
            return Eigen::Isometry3d::Identity();
        }

    protected:
        dart::dynamics::SkeletonPtr _load_model(std::string model_file)
        {
            dart::dynamics::SkeletonPtr tmp_skel;
            boost::filesystem::path p(model_file);
            if (p.extension() == ".urdf") {
                dart::utils::DartLoader loader;
                tmp_skel = loader.parseSkeleton(model_file);
            }
            else if (p.extension() == ".sdf")
                tmp_skel = dart::utils::SdfParser::readSkeleton(model_file);
            else
                return nullptr;

            if (tmp_skel == nullptr)
                return nullptr;

            tmp_skel->setName(_robot_name);
            // Set joint limits/actuator types
            for (size_t i = 0; i < tmp_skel->getNumJoints(); ++i) {
                tmp_skel->getJoint(i)->setPositionLimitEnforced(true);
                tmp_skel->getJoint(i)->setActuatorType(dart::dynamics::Joint::FORCE);
            }

            return tmp_skel;
        }

        void _set_damages(const std::vector<RobotDamage>& damages)
        {
            _damages = damages;
            for (auto dmg : _damages) {
                if (dmg.type == "blocked_joint") {
                    auto jnt = _skeleton->getJoint(dmg.data);
                    if (dmg.extra)
                        jnt->setPosition(0, *((double*)dmg.extra));
                    jnt->setActuatorType(dart::dynamics::Joint::LOCKED);
                }
                else if (dmg.type == "free_joint") {
                    _skeleton->getJoint(dmg.data)->setActuatorType(dart::dynamics::Joint::PASSIVE);
                }
            }
        }

        std::string _robot_name;
        dart::dynamics::SkeletonPtr _skeleton;
        std::vector<RobotDamage> _damages;
    };
}

#endif
