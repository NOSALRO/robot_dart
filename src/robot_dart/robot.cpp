#include <robot_dart/robot.hpp>

#include <boost/filesystem.hpp>

#include <dart/dynamics/DegreeOfFreedom.hpp>
#include <dart/dynamics/FreeJoint.hpp>
#include <dart/dynamics/MeshShape.hpp>
#include <dart/dynamics/WeldJoint.hpp>
#include <dart/utils/sdf/SdfParser.hpp>
#include <dart/utils/urdf/urdf.hpp>

#include <robot_dart/control/robot_control.hpp>

namespace robot_dart {
    Robot::Robot() {}

    Robot::Robot(std::string model_file, std::vector<RobotDamage> damages, std::string robot_name, bool absolute_path) : _robot_name(robot_name), _skeleton(_load_model(model_file, absolute_path))
    {
        assert(_skeleton != nullptr);
        _set_damages(damages);
    }

    Robot::Robot(dart::dynamics::SkeletonPtr skeleton, std::vector<RobotDamage> damages, std::string robot_name) : _robot_name(robot_name), _skeleton(skeleton)
    {
        assert(_skeleton != nullptr);
        _skeleton->setName(robot_name);
        _set_damages(damages);
    }

    std::shared_ptr<Robot> Robot::clone() const
    {
        // safely clone the skeleton
        _skeleton->getMutex().lock();
        auto tmp_skel = _skeleton->clone();
        _skeleton->getMutex().unlock();
        auto robot = std::make_shared<Robot>();
        robot->_skeleton = tmp_skel;
        robot->_damages = _damages;
        robot->_robot_name = _robot_name;
        robot->_controllers.clear();
        for (auto& ctrl : _controllers) {
            robot->_controllers.push_back(ctrl->clone());
            robot->_controllers.back()->set_robot(robot);
        }
        return robot;
    }

    dart::dynamics::SkeletonPtr Robot::skeleton()
    {
        return _skeleton;
    }

    std::vector<RobotDamage> Robot::damages() const
    {
        return _damages;
    }

    std::string Robot::name() const
    {
        return _robot_name;
    }

    void Robot::update(double t)
    {
        Eigen::VectorXd commands = Eigen::VectorXd::Zero(_skeleton->getNumDofs());
        for (auto& ctrl : _controllers) {
            if (ctrl->active())
                commands += ctrl->weight() * ctrl->commands(t);
        }

        _skeleton->setCommands(commands);
    }

    void Robot::reinit_controllers()
    {
        for (auto& ctrl : _controllers)
            ctrl->init();
    }

    size_t Robot::num_controllers() const
    {
        return _controllers.size();
    }

    std::vector<std::shared_ptr<control::RobotControl>> Robot::controllers() const
    {
        return _controllers;
    }

    std::vector<std::shared_ptr<control::RobotControl>> Robot::activeControllers() const
    {
        std::vector<std::shared_ptr<control::RobotControl>> ctrls;
        for (auto& ctrl : _controllers) {
            if (ctrl->active())
                ctrls.push_back(ctrl);
        }

        return ctrls;
    }

    void Robot::add_controller(const std::shared_ptr<control::RobotControl>& controller, double weight)
    {
        _controllers.push_back(controller);
        controller->set_robot(this->shared_from_this());
        controller->set_weight(weight);
        controller->init();
    }

    void Robot::remove_controller(const std::shared_ptr<control::RobotControl>& controller)
    {
        auto it = std::find(_controllers.begin(), _controllers.end(), controller);
        if (it != _controllers.end())
            _controllers.erase(it);
    }

    void Robot::remove_controller(size_t index)
    {
        assert(index < _controllers.size());
        _controllers.erase(_controllers.begin() + index);
    }

    void Robot::clear_controllers()
    {
        _controllers.clear();
    }

    std::shared_ptr<control::RobotControl> Robot::controller(size_t index) const
    {
        assert(index < _controllers.size());
        return _controllers[index];
    }

    void Robot::fix_to_world()
    {
        if (fixed())
            return;
        Eigen::Isometry3d tf(dart::math::expMap(_skeleton->getPositions().segment(0, 6)));
        _skeleton->getRootBodyNode()->changeParentJointType<dart::dynamics::WeldJoint>();
        _skeleton->getRootBodyNode()->getParentJoint()->setTransformFromParentBodyNode(tf);

        reinit_controllers();
    }

    // pose: Orientation-Position
    void Robot::free_from_world(const Eigen::Vector6d& pose)
    {
        if (free())
            return;
        Eigen::Isometry3d tf(dart::math::expMap(pose));
        _skeleton->getRootBodyNode()->changeParentJointType<dart::dynamics::FreeJoint>();
        _skeleton->getRootBodyNode()->getParentJoint()->setTransformFromParentBodyNode(tf);

        reinit_controllers();
    }

    bool Robot::fixed() const
    {
        return _skeleton->getRootBodyNode()->getParentJoint()->getType() == dart::dynamics::WeldJoint::getStaticType();
    }

    bool Robot::free() const
    {
        return _skeleton->getRootBodyNode()->getParentJoint()->getType() == dart::dynamics::FreeJoint::getStaticType();
    }

    void Robot::set_actuator_types(const std::vector<dart::dynamics::Joint::ActuatorType>& types)
    {
        assert(types.size() == _skeleton->getNumDofs());
        for (size_t i = 0; i < _skeleton->getNumJoints(); ++i) {
            _skeleton->getDof(i)->getJoint()->setActuatorType(types[i]);
        }
    }

    void Robot::set_actuator_types(dart::dynamics::Joint::ActuatorType type)
    {
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            _skeleton->getDof(i)->getJoint()->setActuatorType(type);
        }
    }

    void Robot::set_position_enforced(const std::vector<bool>& enforced)
    {
        assert(enforced.size() == _skeleton->getNumDofs());
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            _skeleton->getDof(i)->getJoint()->setPositionLimitEnforced(enforced[i]);
        }
    }

    void Robot::set_position_enforced(bool enforced)
    {
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            _skeleton->getDof(i)->getJoint()->setPositionLimitEnforced(enforced);
        }
    }

    // TODO: Warning this only works for the 1st DOF of the joints
    void Robot::set_damping_coeff(const std::vector<double>& damps)
    {
        assert(damps.size() == _skeleton->getNumDofs());
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            _skeleton->getDof(i)->getJoint()->setDampingCoefficient(0, damps[i]);
        }
    }

    void Robot::set_damping_coeff(double damp)
    {
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            _skeleton->getDof(i)->getJoint()->setDampingCoefficient(0, damp);
        }
    }

    Eigen::Vector3d Robot::body_pos(std::string body_name) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        if (bd)
            return bd->getTransform().translation();

        return Eigen::Vector3d::Zero();
    }

    Eigen::Matrix3d Robot::body_rot(std::string body_name) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        if (bd)
            return bd->getTransform().linear();
        return Eigen::Matrix3d::Identity();
    }

    Eigen::Isometry3d Robot::body_trans(std::string body_name) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        if (bd)
            return bd->getTransform();
        return Eigen::Isometry3d::Identity();
    }

    dart::dynamics::SkeletonPtr Robot::_load_model(std::string model_file, bool absolute_path)
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

    void Robot::_set_damages(const std::vector<RobotDamage>& damages)
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
} // namespace robot_dart