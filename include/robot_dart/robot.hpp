#ifndef ROBOT_DART_ROBOT_HPP
#define ROBOT_DART_ROBOT_HPP

#include <boost/filesystem.hpp>

#include <dart/dart.hpp>
#include <dart/utils/sdf/SdfParser.hpp>
#include <dart/utils/urdf/urdf.hpp>

#include <Eigen/Core>
#include <fstream>
#include <streambuf>
#include <string>

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

        Robot(std::string model_file, std::vector<RobotDamage> damages = {}, std::string robot_name = "robot", bool absolute_path = false) : _robot_name(robot_name), _skeleton(_load_model(model_file, absolute_path)), _fixed_to_world(false)
        {
            assert(_skeleton != nullptr);
            _set_damages(damages);
        }

        Robot(dart::dynamics::SkeletonPtr skeleton, std::vector<RobotDamage> damages = {}, std::string robot_name = "robot") : _robot_name(robot_name), _skeleton(skeleton), _fixed_to_world(false)
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
            robot->_fixed_to_world = _fixed_to_world;
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
            Eigen::Isometry3d tf(dart::math::expMap(_skeleton->getPositions().segment(0, 6)));
            _skeleton->getRootBodyNode()->changeParentJointType<dart::dynamics::WeldJoint>();
            _skeleton->getRootBodyNode()->getParentJoint()->setTransformFromParentBodyNode(tf);
            _fixed_to_world = true;
        }

        // pose: Orientation-Position
        void free_from_world(const Eigen::Vector6d& pose = Eigen::Vector6d::Zero())
        {
            Eigen::Isometry3d tf(dart::math::expMap(pose));
            _skeleton->getRootBodyNode()->changeParentJointType<dart::dynamics::FreeJoint>();
            _skeleton->getRootBodyNode()->getParentJoint()->setTransformFromParentBodyNode(tf);
            _fixed_to_world = false;
        }

        bool fixed_to_world() const
        {
            return _skeleton->getRootBodyNode()->getParentJoint()->getNumDofs() == 0;
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

        // TODO: Warning this only works for the 1st DOF of the joints
        void set_damping_coeff(const std::vector<double>& damps)
        {
            assert(damps.size() == _skeleton->getNumJoints());
            for (size_t i = 0; i < _skeleton->getNumJoints(); ++i) {
                _skeleton->getJoint(i)->setDampingCoefficient(0, damps[i]);
            }
        }

        void set_damping_coeff(double damp)
        {
            for (size_t i = 0; i < _skeleton->getNumJoints(); ++i) {
                _skeleton->getJoint(i)->setDampingCoefficient(0, damp);
            }
        }

        void set_damping_coeff(const std::vector<size_t>& indices, const std::vector<double>& damps)
        {
            assert(indices.size() == damps.size());
            size_t jnt_num = _skeleton->getNumJoints();
            for (size_t i = 0; i < indices.size(); ++i) {
                assert(indices[i] > 0 && indices[i] < jnt_num);
                _skeleton->getJoint(indices[i])->setDampingCoefficient(0, damps[i]);
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
        dart::dynamics::SkeletonPtr _load_model(std::string model_file, bool absolute_path)
        {
            if (!absolute_path)
                model_file = boost::filesystem::current_path().string() + "/" + model_file;

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

            // Fix for mesh materials
            for (size_t i = 0; i < tmp_skel->getNumBodyNodes(); ++i) {
                dart::dynamics::BodyNode* bn = tmp_skel->getBodyNode(i);
                for (size_t j = 0; j < bn->getNumShapeNodes(); ++j) {
                    dart::dynamics::ShapeNode* sn = bn->getShapeNode(j);
                    if (sn->getVisualAspect()) {
                        dart::dynamics::MeshShape* ms = dynamic_cast<dart::dynamics::MeshShape*>(sn->getShape().get());
                        if (ms)
                            ms->setColorMode(dart::dynamics::MeshShape::SHAPE_COLOR);
                    }
                }
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
        bool _fixed_to_world;
    };
}

#endif
