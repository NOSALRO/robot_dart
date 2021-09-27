
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <unistd.h>

#include <dart/config.hpp>
#include <dart/dynamics/BoxShape.hpp>
#include <dart/dynamics/DegreeOfFreedom.hpp>
#include <dart/dynamics/EllipsoidShape.hpp>
#include <dart/dynamics/FreeJoint.hpp>
#include <dart/dynamics/MeshShape.hpp>
#include <dart/dynamics/WeldJoint.hpp>
#if DART_VERSION_AT_LEAST(7, 0, 0)
#include <dart/io/SkelParser.hpp>
#include <dart/io/sdf/SdfParser.hpp>
#include <dart/io/urdf/urdf.hpp>
#else
#include <dart/utils/SkelParser.hpp>
#include <dart/utils/sdf/SdfParser.hpp>
#include <dart/utils/urdf/urdf.hpp>

#include <robot_dart/robot.hpp>
#include <robot_dart/utils.hpp>

// namespace alias for compatibility
namespace dart {
    namespace io = utils;
}
#endif

#include <robot_dart/control/robot_control.hpp>

namespace robot_dart {
    namespace detail {
        template <int content>
        Eigen::VectorXd dof_data(dart::dynamics::SkeletonPtr skeleton, const std::vector<std::string>& dof_names, const std::unordered_map<std::string, size_t>& dof_map)
        {
            // Return all values
            if (dof_names.empty()) {
                if (content == 0)
                    return skeleton->getPositions();
                else if (content == 1)
                    return skeleton->getVelocities();
                else if (content == 2)
                    return skeleton->getAccelerations();
                else if (content == 3)
                    return skeleton->getForces();
                else if (content == 4)
                    return skeleton->getCommands();
                else if (content == 5)
                    return skeleton->getPositionLowerLimits();
                else if (content == 6)
                    return skeleton->getPositionUpperLimits();
                else if (content == 7)
                    return skeleton->getVelocityLowerLimits();
                else if (content == 8)
                    return skeleton->getVelocityUpperLimits();
                else if (content == 9)
                    return skeleton->getAccelerationLowerLimits();
                else if (content == 10)
                    return skeleton->getAccelerationUpperLimits();
                else if (content == 11)
                    return skeleton->getForceLowerLimits();
                else if (content == 12)
                    return skeleton->getForceUpperLimits();
                else if (content == 13)
                    return skeleton->getCoriolisForces();
                else if (content == 14)
                    return skeleton->getGravityForces();
                else if (content == 15)
                    return skeleton->getCoriolisAndGravityForces();
                ROBOT_DART_EXCEPTION_ASSERT(false, "Unknown type of data!");
            }

            Eigen::VectorXd data(dof_names.size());
            Eigen::VectorXd tmp;

            if (content == 13)
                tmp = skeleton->getCoriolisForces();
            else if (content == 14)
                tmp = skeleton->getGravityForces();
            else if (content == 15)
                tmp = skeleton->getCoriolisAndGravityForces();

            for (size_t i = 0; i < dof_names.size(); i++) {
                auto it = dof_map.find(dof_names[i]);
                ROBOT_DART_ASSERT(it != dof_map.end(), "dof_data: " + dof_names[i] + " is not in dof_map", Eigen::VectorXd());
                auto dof = skeleton->getDof(it->second);
                if (content == 0)
                    data(i) = dof->getPosition();
                else if (content == 1)
                    data(i) = dof->getVelocity();
                else if (content == 2)
                    data(i) = dof->getAcceleration();
                else if (content == 3)
                    data(i) = dof->getForce();
                else if (content == 4)
                    data(i) = dof->getCommand();
                else if (content == 5)
                    data(i) = dof->getPositionLowerLimit();
                else if (content == 6)
                    data(i) = dof->getPositionUpperLimit();
                else if (content == 7)
                    data(i) = dof->getVelocityLowerLimit();
                else if (content == 8)
                    data(i) = dof->getVelocityUpperLimit();
                else if (content == 9)
                    data(i) = dof->getAccelerationLowerLimit();
                else if (content == 10)
                    data(i) = dof->getAccelerationUpperLimit();
                else if (content == 11)
                    data(i) = dof->getForceLowerLimit();
                else if (content == 12)
                    data(i) = dof->getForceUpperLimit();
                else if (content == 13 || content == 14 || content == 15)
                    data(i) = tmp(it->second);
                else
                    ROBOT_DART_EXCEPTION_ASSERT(false, "Unknown type of data!");
            }
            return data;
        }

        template <int content>
        void set_dof_data(const Eigen::VectorXd& data, dart::dynamics::SkeletonPtr skeleton, const std::vector<std::string>& dof_names, const std::unordered_map<std::string, size_t>& dof_map)
        {
            // Set all values
            if (dof_names.empty()) {
                ROBOT_DART_ASSERT(static_cast<size_t>(data.size()) == skeleton->getNumDofs(), "set_dof_data: size of data is not the same as the DoFs", );
                if (content == 0)
                    return skeleton->setPositions(data);
                else if (content == 1)
                    return skeleton->setVelocities(data);
                else if (content == 2)
                    return skeleton->setAccelerations(data);
                else if (content == 3)
                    return skeleton->setForces(data);
                else if (content == 4)
                    return skeleton->setCommands(data);
                else if (content == 5)
                    return skeleton->setPositionLowerLimits(data);
                else if (content == 6)
                    return skeleton->setPositionUpperLimits(data);
                else if (content == 7)
                    return skeleton->setVelocityLowerLimits(data);
                else if (content == 8)
                    return skeleton->setVelocityUpperLimits(data);
                else if (content == 9)
                    return skeleton->setAccelerationLowerLimits(data);
                else if (content == 10)
                    return skeleton->setAccelerationUpperLimits(data);
                else if (content == 11)
                    return skeleton->setForceLowerLimits(data);
                else if (content == 12)
                    return skeleton->setForceUpperLimits(data);
                ROBOT_DART_EXCEPTION_ASSERT(false, "Unknown type of data!");
            }

            ROBOT_DART_ASSERT(static_cast<size_t>(data.size()) == dof_names.size(), "set_dof_data: size of data is not the same as the dof_names size", );
            for (size_t i = 0; i < dof_names.size(); i++) {
                auto it = dof_map.find(dof_names[i]);
                ROBOT_DART_ASSERT(it != dof_map.end(), "dof_data: " + dof_names[i] + " is not in dof_map", );
                auto dof = skeleton->getDof(it->second);
                if (content == 0)
                    dof->setPosition(data(i));
                else if (content == 1)
                    dof->setVelocity(data(i));
                else if (content == 2)
                    dof->setAcceleration(data(i));
                else if (content == 3)
                    dof->setForce(data(i));
                else if (content == 4)
                    dof->setCommand(data(i));
                else if (content == 5)
                    dof->setPositionLowerLimit(data(i));
                else if (content == 6)
                    dof->setPositionUpperLimit(data(i));
                else if (content == 7)
                    dof->setVelocityLowerLimit(data(i));
                else if (content == 8)
                    dof->setVelocityUpperLimit(data(i));
                else if (content == 9)
                    dof->setAccelerationLowerLimit(data(i));
                else if (content == 10)
                    dof->setAccelerationUpperLimit(data(i));
                else if (content == 11)
                    dof->setForceLowerLimit(data(i));
                else if (content == 12)
                    dof->setForceUpperLimit(data(i));
                else
                    ROBOT_DART_EXCEPTION_ASSERT(false, "Unknown type of data!");
            }
        }

        template <int content>
        void add_dof_data(const Eigen::VectorXd& data, dart::dynamics::SkeletonPtr skeleton, const std::vector<std::string>& dof_names, const std::unordered_map<std::string, size_t>& dof_map)
        {
            // Set all values
            if (dof_names.empty()) {
                ROBOT_DART_ASSERT(static_cast<size_t>(data.size()) == skeleton->getNumDofs(), "set_dof_data: size of data is not the same as the DoFs", );
                if (content == 0)
                    return skeleton->setPositions(skeleton->getPositions() + data);
                else if (content == 1)
                    return skeleton->setVelocities(skeleton->getVelocities() + data);
                else if (content == 2)
                    return skeleton->setAccelerations(skeleton->getAccelerations() + data);
                else if (content == 3)
                    return skeleton->setForces(skeleton->getForces() + data);
                else if (content == 4)
                    return skeleton->setCommands(skeleton->getCommands() + data);
                else if (content == 5)
                    return skeleton->setPositionLowerLimits(skeleton->getPositionLowerLimits() + data);
                else if (content == 6)
                    return skeleton->setPositionUpperLimits(skeleton->getPositionUpperLimits() + data);
                else if (content == 7)
                    return skeleton->setVelocityLowerLimits(skeleton->getVelocityLowerLimits() + data);
                else if (content == 8)
                    return skeleton->setVelocityUpperLimits(skeleton->getVelocityUpperLimits() + data);
                else if (content == 9)
                    return skeleton->setAccelerationLowerLimits(skeleton->getAccelerationLowerLimits() + data);
                else if (content == 10)
                    return skeleton->setAccelerationUpperLimits(skeleton->getAccelerationUpperLimits() + data);
                else if (content == 11)
                    return skeleton->setForceLowerLimits(skeleton->getForceLowerLimits() + data);
                else if (content == 12)
                    return skeleton->setForceUpperLimits(skeleton->getForceUpperLimits() + data);
                ROBOT_DART_EXCEPTION_ASSERT(false, "Unknown type of data!");
            }

            ROBOT_DART_ASSERT(static_cast<size_t>(data.size()) == dof_names.size(), "add_dof_data: size of data is not the same as the dof_names size", );
            for (size_t i = 0; i < dof_names.size(); i++) {
                auto it = dof_map.find(dof_names[i]);
                ROBOT_DART_ASSERT(it != dof_map.end(), "dof_data: " + dof_names[i] + " is not in dof_map", );
                auto dof = skeleton->getDof(it->second);
                if (content == 0)
                    dof->setPosition(dof->getPosition() + data(i));
                else if (content == 1)
                    dof->setVelocity(dof->getVelocity() + data(i));
                else if (content == 2)
                    dof->setAcceleration(dof->getAcceleration() + data(i));
                else if (content == 3)
                    dof->setForce(dof->getForce() + data(i));
                else if (content == 4)
                    dof->setCommand(dof->getCommand() + data(i));
                else if (content == 5)
                    dof->setPositionLowerLimit(dof->getPositionLowerLimit() + data(i));
                else if (content == 6)
                    dof->setPositionUpperLimit(dof->getPositionUpperLimit() + data(i));
                else if (content == 7)
                    dof->setVelocityLowerLimit(dof->getVelocityLowerLimit() + data(i));
                else if (content == 8)
                    dof->setVelocityUpperLimit(dof->getVelocityUpperLimit() + data(i));
                else if (content == 9)
                    dof->setAccelerationLowerLimit(dof->getAccelerationLowerLimit() + data(i));
                else if (content == 10)
                    dof->setAccelerationUpperLimit(dof->getAccelerationUpperLimit() + data(i));
                else if (content == 11)
                    dof->setForceLowerLimit(dof->getForceLowerLimit() + data(i));
                else if (content == 12)
                    dof->setForceUpperLimit(dof->getForceUpperLimit() + data(i));
                else
                    ROBOT_DART_EXCEPTION_ASSERT(false, "Unknown type of data!");
            }
        }
    } // namespace detail

    Robot::Robot(const std::string& model_file, const std::vector<std::pair<std::string, std::string>>& packages, const std::string& robot_name, bool is_urdf_string, bool cast_shadows, std::vector<RobotDamage> damages)
        : _robot_name(robot_name), _skeleton(_load_model(model_file, packages, is_urdf_string)), _cast_shadows(cast_shadows), _is_ghost(false)
    {
        ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(_skeleton != nullptr);
        _set_damages(damages);
    }

    Robot::Robot(const std::string& model_file, const std::string& robot_name, bool is_urdf_string, bool cast_shadows, std::vector<RobotDamage> damages)
        : Robot(model_file, std::vector<std::pair<std::string, std::string>>(), robot_name, is_urdf_string, cast_shadows, damages)
    {
    }

    Robot::Robot(dart::dynamics::SkeletonPtr skeleton, const std::string& robot_name, bool cast_shadows, std::vector<RobotDamage> damages)
        : _robot_name(robot_name), _skeleton(skeleton), _cast_shadows(cast_shadows), _is_ghost(false)
    {
        ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(_skeleton != nullptr);
        _skeleton->setName(robot_name);
        _set_damages(damages);
        reset();
    }

    std::shared_ptr<Robot> Robot::clone() const
    {
        // safely clone the skeleton
        _skeleton->getMutex().lock();
#if DART_VERSION_AT_LEAST(6, 7, 2)
        auto tmp_skel = _skeleton->cloneSkeleton();
#else
        auto tmp_skel = _skeleton->clone();
#endif
        _skeleton->getMutex().unlock();
        auto robot = std::make_shared<Robot>(tmp_skel, _robot_name);
        robot->_damages = _damages;
        robot->_model_filename = _model_filename;
        robot->_controllers.clear();
        for (auto& ctrl : _controllers) {
            robot->add_controller(ctrl->clone(), ctrl->weight());
        }
        return robot;
    }

    std::shared_ptr<Robot> Robot::clone_ghost(const std::string& ghost_name, const Eigen::Vector4d& ghost_color) const
    {
        // safely clone the skeleton
        _skeleton->getMutex().lock();
#if DART_VERSION_AT_LEAST(6, 7, 2)
        auto tmp_skel = _skeleton->cloneSkeleton();
#else
        auto tmp_skel = _skeleton->clone();
#endif
        _skeleton->getMutex().unlock();
        auto robot = std::make_shared<Robot>(tmp_skel, ghost_name + "_" + _robot_name);
        robot->_damages = _damages;
        robot->_model_filename = _model_filename;

        // ghost robots have no controllers
        robot->_controllers.clear();
        // ghost robots do not do physics updates
        robot->skeleton()->setMobile(false);
        for (auto& bd : robot->skeleton()->getBodyNodes()) {
            // ghost robots do not have collisions
            auto& collision_shapes = bd->getShapeNodesWith<dart::dynamics::CollisionAspect>();
            for (auto& shape : collision_shapes) {
                shape->removeAspect<dart::dynamics::CollisionAspect>();
            }

            // ghost robots have a different color (same for all bodies)
            auto& visual_shapes = bd->getShapeNodesWith<dart::dynamics::VisualAspect>();
            for (auto& shape : visual_shapes) {
                shape->getVisualAspect()->setRGBA(ghost_color);
            }
        }

        // ghost robots, by default, use the color from the VisualAspect
        robot->set_color_mode("aspect");

        // ghost robots do not cast shadows
        robot->set_cast_shadows(false);
        // set the ghost flag
        robot->set_ghost(true);

        return robot;
    }

    dart::dynamics::SkeletonPtr Robot::skeleton() { return _skeleton; }

    dart::dynamics::BodyNode* Robot::body_node(const std::string& body_name) { return _skeleton->getBodyNode(body_name); }

    dart::dynamics::BodyNode* Robot::body_node(size_t body_index)
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", nullptr);
        return _skeleton->getBodyNode(body_index);
    }

    dart::dynamics::Joint* Robot::joint(const std::string& joint_name) { return _skeleton->getJoint(joint_name); }

    dart::dynamics::Joint* Robot::joint(size_t joint_index)
    {
        ROBOT_DART_ASSERT(joint_index < _skeleton->getNumJoints(), "Joint index out of bounds", nullptr);
        return _skeleton->getJoint(joint_index);
    }

    dart::dynamics::DegreeOfFreedom* Robot::dof(const std::string& dof_name) { return _skeleton->getDof(dof_name); }

    dart::dynamics::DegreeOfFreedom* Robot::dof(size_t dof_index)
    {
        ROBOT_DART_ASSERT(dof_index < _skeleton->getNumDofs(), "Dof index out of bounds", nullptr);
        return _skeleton->getDof(dof_index);
    }

    std::vector<RobotDamage> Robot::damages() const { return _damages; }

    const std::string& Robot::name() const { return _robot_name; }

    void Robot::update(double t)
    {
        _skeleton->setCommands(Eigen::VectorXd::Zero(_skeleton->getNumDofs()));

        for (auto& ctrl : _controllers) {
            if (ctrl->active())
                detail::add_dof_data<4>(ctrl->weight() * ctrl->calculate(t), _skeleton,
                    ctrl->controllable_dofs(), _dof_map);
        }
    }

    void Robot::reinit_controllers()
    {
        for (auto& ctrl : _controllers)
            ctrl->init();
    }

    size_t Robot::num_controllers() const { return _controllers.size(); }

    std::vector<std::shared_ptr<control::RobotControl>> Robot::controllers() const
    {
        return _controllers;
    }

    std::vector<std::shared_ptr<control::RobotControl>> Robot::active_controllers() const
    {
        std::vector<std::shared_ptr<control::RobotControl>> ctrls;
        for (auto& ctrl : _controllers) {
            if (ctrl->active())
                ctrls.push_back(ctrl);
        }

        return ctrls;
    }

    std::shared_ptr<control::RobotControl> Robot::controller(size_t index) const
    {
        ROBOT_DART_ASSERT(index < _controllers.size(), "Controller index out of bounds", nullptr);
        return _controllers[index];
    }

    void Robot::add_controller(
        const std::shared_ptr<control::RobotControl>& controller, double weight)
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
        ROBOT_DART_ASSERT(index < _controllers.size(), "Controller index out of bounds", );
        _controllers.erase(_controllers.begin() + index);
    }

    void Robot::clear_controllers() { _controllers.clear(); }

    void Robot::fix_to_world()
    {
        auto parent_jt = _skeleton->getRootBodyNode()->getParentJoint();
        ROBOT_DART_ASSERT(parent_jt != nullptr, "RootBodyNode does not have a parent joint!", );

        if (fixed())
            return;

        Eigen::Isometry3d tf(dart::math::expAngular(_skeleton->getPositions().head(3)));
        tf.translation() = _skeleton->getPositions().segment(3, 3);
        dart::dynamics::WeldJoint::Properties properties;
        properties.mName = parent_jt->getName();
        _skeleton->getRootBodyNode()->changeParentJointType<dart::dynamics::WeldJoint>(properties);
        _skeleton->getRootBodyNode()->getParentJoint()->setTransformFromParentBodyNode(tf);

        reinit_controllers();
        update_joint_dof_maps();
    }

    // pose: Orientation-Position
    void Robot::free_from_world(const Eigen::Vector6d& pose)
    {
        auto parent_jt = _skeleton->getRootBodyNode()->getParentJoint();
        ROBOT_DART_ASSERT(parent_jt != nullptr, "RootBodyNode does not have a parent joint!", );

        Eigen::Isometry3d tf(dart::math::expAngular(pose.head(3)));
        tf.translation() = pose.segment(3, 3);

        // if already free, we only change the transformation
        if (free()) {
            parent_jt->setTransformFromParentBodyNode(tf);
            return;
        }

        dart::dynamics::FreeJoint::Properties properties;
        properties.mName = parent_jt->getName();
        _skeleton->getRootBodyNode()->changeParentJointType<dart::dynamics::FreeJoint>(properties);
        _skeleton->getRootBodyNode()->getParentJoint()->setTransformFromParentBodyNode(tf);

        reinit_controllers();
        update_joint_dof_maps();
    }

    bool Robot::fixed() const
    {
        auto parent_jt = _skeleton->getRootBodyNode()->getParentJoint();
        ROBOT_DART_ASSERT(parent_jt != nullptr, "RootBodyNode does not have a parent joint!", false);
        return parent_jt->getType() == dart::dynamics::WeldJoint::getStaticType();
    }

    bool Robot::free() const
    {
        auto parent_jt = _skeleton->getRootBodyNode()->getParentJoint();
        ROBOT_DART_ASSERT(parent_jt != nullptr, "RootBodyNode does not have a parent joint!", false);
        return parent_jt->getType() == dart::dynamics::FreeJoint::getStaticType();
    }

    void Robot::reset()
    {
        _skeleton->resetPositions();
        _skeleton->resetVelocities();
        _skeleton->resetAccelerations();

        clear_internal_forces();
        reset_commands();
        clear_external_forces();
    }

    void Robot::clear_external_forces() { _skeleton->clearExternalForces(); }

    void Robot::clear_internal_forces()
    {
        _skeleton->clearInternalForces();
        _skeleton->clearConstraintImpulses();
    }

    void Robot::reset_commands() { _skeleton->resetCommands(); }

    void Robot::set_actuator_types(const std::string& type, const std::vector<std::string>& joint_names, bool override_mimic, bool override_base)
    {
        // Set all dofs to same actuator type
        if (joint_names.empty()) {
            if (type == "torque") {
                return _set_actuator_types(dart::dynamics::Joint::FORCE, override_mimic, override_base);
            }
            else if (type == "servo") {
                return _set_actuator_types(dart::dynamics::Joint::SERVO, override_mimic, override_base);
            }
            else if (type == "velocity") {
                return _set_actuator_types(dart::dynamics::Joint::VELOCITY, override_mimic, override_base);
            }
            else if (type == "passive") {
                return _set_actuator_types(dart::dynamics::Joint::PASSIVE, override_mimic, override_base);
            }
            else if (type == "locked") {
                return _set_actuator_types(dart::dynamics::Joint::LOCKED, override_mimic, override_base);
            }
            else if (type == "mimic") {
                ROBOT_DART_WARNING(true, "Use this only if you know what you are doing. Use set_mimic otherwise.");
                return _set_actuator_types(dart::dynamics::Joint::MIMIC, override_mimic, override_base);
            }
            ROBOT_DART_EXCEPTION_ASSERT(false, "Unknown type of actuator type. Valid values: torque, servo, velocity, passive, locked, mimic");
        }

        for (size_t i = 0; i < joint_names.size(); i++) {
            auto it = _joint_map.find(joint_names[i]);
            ROBOT_DART_ASSERT(it != _joint_map.end(), "set_actuator_type: " + joint_names[i] + " is not in joint_map", );
            if (type == "torque") {
                _set_actuator_type(it->second, dart::dynamics::Joint::FORCE, override_mimic, override_base);
            }
            else if (type == "servo") {
                _set_actuator_type(it->second, dart::dynamics::Joint::SERVO, override_mimic, override_base);
            }
            else if (type == "velocity") {
                _set_actuator_type(it->second, dart::dynamics::Joint::VELOCITY, override_mimic, override_base);
            }
            else if (type == "passive") {
                _set_actuator_type(it->second, dart::dynamics::Joint::PASSIVE, override_mimic, override_base);
            }
            else if (type == "locked") {
                _set_actuator_type(it->second, dart::dynamics::Joint::LOCKED, override_mimic, override_base);
            }
            else if (type == "mimic") {
                ROBOT_DART_WARNING(true, "Use this only if you know what you are doing. Use set_mimic otherwise.");
                _set_actuator_type(it->second, dart::dynamics::Joint::MIMIC, override_mimic, override_base);
            }
            else
                ROBOT_DART_EXCEPTION_ASSERT(false, "Unknown type of actuator type. Valid values: torque, servo, velocity, passive, locked, mimic");
        }
    }

    void Robot::set_actuator_type(const std::string& type, const std::string& joint_name, bool override_mimic, bool override_base)
    {
        set_actuator_types(type, {joint_name}, override_mimic, override_base);
    }

    void Robot::set_mimic(const std::string& joint_name, const std::string& mimic_joint_name, double multiplier, double offset)
    {
        dart::dynamics::Joint* jnt = _skeleton->getJoint(joint_name);
        const dart::dynamics::Joint* mimic_jnt = _skeleton->getJoint(mimic_joint_name);

        ROBOT_DART_ASSERT((jnt && mimic_jnt), "set_mimic: joint names do not exist", );

        jnt->setActuatorType(dart::dynamics::Joint::MIMIC);
        jnt->setMimicJoint(mimic_jnt, multiplier, offset);
    }

    std::string Robot::actuator_type(const std::string& joint_name) const
    {
        auto it = _joint_map.find(joint_name);
        ROBOT_DART_ASSERT(it != _joint_map.end(), "actuator_type: " + joint_name + " is not in joint_map", "invalid");

        auto type = _actuator_type(it->second);
        if (type == dart::dynamics::Joint::FORCE)
            return "torque";
        else if (type == dart::dynamics::Joint::SERVO)
            return "servo";
        else if (type == dart::dynamics::Joint::VELOCITY)
            return "velocity";
        else if (type == dart::dynamics::Joint::PASSIVE)
            return "passive";
        else if (type == dart::dynamics::Joint::LOCKED)
            return "locked";
        else if (type == dart::dynamics::Joint::MIMIC)
            return "mimic";

        ROBOT_DART_ASSERT(false, "actuator_type: we should not reach here", "invalid");
    }

    std::vector<std::string> Robot::actuator_types(const std::vector<std::string>& joint_names) const
    {
        std::vector<std::string> str_types;
        // Get all dofs
        if (joint_names.empty()) {
            auto types = _actuator_types();

            for (size_t i = 0; i < types.size(); i++) {
                auto type = types[i];
                if (type == dart::dynamics::Joint::FORCE)
                    str_types.push_back("torque");
                else if (type == dart::dynamics::Joint::SERVO)
                    str_types.push_back("servo");
                else if (type == dart::dynamics::Joint::VELOCITY)
                    str_types.push_back("velocity");
                else if (type == dart::dynamics::Joint::PASSIVE)
                    str_types.push_back("passive");
                else if (type == dart::dynamics::Joint::LOCKED)
                    str_types.push_back("locked");
                else if (type == dart::dynamics::Joint::MIMIC)
                    str_types.push_back("mimic");
            }

            ROBOT_DART_ASSERT(str_types.size() == types.size(), "actuator_types: sizes of retrieved modes do not match", {});

            return str_types;
        }

        for (size_t i = 0; i < joint_names.size(); i++) {
            str_types.push_back(actuator_type(joint_names[i]));
        }

        ROBOT_DART_ASSERT(str_types.size() == joint_names.size(), "actuator_types: sizes of retrieved modes do not match", {});

        return str_types;
    }

    void Robot::set_position_enforced(const std::vector<bool>& enforced, const std::vector<std::string>& dof_names)
    {
        size_t n_dofs = dof_names.size();
        if (n_dofs == 0) {
            ROBOT_DART_ASSERT(enforced.size() == _skeleton->getNumDofs(),
                "Position enforced vector size is not the same as the DOFs of the robot", );
            for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
#if DART_VERSION_AT_LEAST(6, 10, 0)
                _skeleton->getDof(i)->getJoint()->setLimitEnforcement(enforced[i]);
#else
                _skeleton->getDof(i)->getJoint()->setPositionLimitEnforced(enforced[i]);
#endif
            }
        }
        else {
            ROBOT_DART_ASSERT(enforced.size() == dof_names.size(),
                "Position enforced vector size is not the same as the dof_names size", );
            for (size_t i = 0; i < dof_names.size(); i++) {
                auto it = _dof_map.find(dof_names[i]);
                ROBOT_DART_ASSERT(it != _dof_map.end(), "set_position_enforced: " + dof_names[i] + " is not in dof_map", );
#if DART_VERSION_AT_LEAST(6, 10, 0)
                _skeleton->getDof(it->second)->getJoint()->setLimitEnforcement(enforced[i]);
#else
                _skeleton->getDof(it->second)->getJoint()->setPositionLimitEnforced(enforced[i]);
#endif
            }
        }
    }

    void Robot::set_position_enforced(bool enforced, const std::vector<std::string>& dof_names)
    {
        size_t n_dofs = dof_names.size();
        if (n_dofs == 0)
            n_dofs = _skeleton->getNumDofs();
        std::vector<bool> enforced_all(n_dofs, enforced);

        set_position_enforced(enforced_all, dof_names);
    }

    std::vector<bool> Robot::position_enforced(const std::vector<std::string>& dof_names) const
    {
        std::vector<bool> pos;
        if (dof_names.size() == 0) {
            for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
#if DART_VERSION_AT_LEAST(6, 10, 0)
                pos.push_back(_skeleton->getDof(i)->getJoint()->areLimitsEnforced());
#else
                pos.push_back(_skeleton->getDof(i)->getJoint()->isPositionLimitEnforced());
#endif
            }
        }
        else {
            for (size_t i = 0; i < dof_names.size(); i++) {
                auto it = _dof_map.find(dof_names[i]);
                ROBOT_DART_ASSERT(it != _dof_map.end(), "position_enforced: " + dof_names[i] + " is not in dof_map", std::vector<bool>());
#if DART_VERSION_AT_LEAST(6, 10, 0)
                pos.push_back(_skeleton->getDof(it->second)->getJoint()->areLimitsEnforced());
#else
                pos.push_back(_skeleton->getDof(it->second)->getJoint()->isPositionLimitEnforced());
#endif
            }
        }

        return pos;
    }

    void Robot::set_damping_coeffs(const std::vector<double>& damps, const std::vector<std::string>& dof_names)
    {
        size_t n_dofs = dof_names.size();
        if (n_dofs == 0) {
            ROBOT_DART_ASSERT(damps.size() == _skeleton->getNumDofs(),
                "Damping coefficient vector size is not the same as the DOFs of the robot", );
            for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
                _skeleton->getDof(i)->setDampingCoefficient(damps[i]);
            }
        }
        else {
            ROBOT_DART_ASSERT(damps.size() == dof_names.size(),
                "Damping coefficient vector size is not the same as the dof_names size", );
            for (size_t i = 0; i < dof_names.size(); i++) {
                auto it = _dof_map.find(dof_names[i]);
                ROBOT_DART_ASSERT(it != _dof_map.end(), "set_damping_coeffs: " + dof_names[i] + " is not in dof_map", );
                _skeleton->getDof(it->second)->setDampingCoefficient(damps[i]);
            }
        }
    }

    void Robot::set_damping_coeffs(double damp, const std::vector<std::string>& dof_names)
    {
        size_t n_dofs = dof_names.size();
        if (n_dofs == 0)
            n_dofs = _skeleton->getNumDofs();
        std::vector<double> damps_all(n_dofs, damp);

        set_damping_coeffs(damps_all, dof_names);
    }

    std::vector<double> Robot::damping_coeffs(const std::vector<std::string>& dof_names) const
    {
        std::vector<double> dampings;
        if (dof_names.size() == 0) {
            for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
                dampings.push_back(_skeleton->getDof(i)->getDampingCoefficient());
            }
        }
        else {
            for (size_t i = 0; i < dof_names.size(); i++) {
                auto it = _dof_map.find(dof_names[i]);
                ROBOT_DART_ASSERT(it != _dof_map.end(), "damping_coeffs: " + dof_names[i] + " is not in dof_map", std::vector<double>());
                dampings.push_back(_skeleton->getDof(it->second)->getDampingCoefficient());
            }
        }

        return dampings;
    }

    void Robot::set_coulomb_coeffs(const std::vector<double>& cfrictions, const std::vector<std::string>& dof_names)
    {
        size_t n_dofs = dof_names.size();
        if (n_dofs == 0) {
            ROBOT_DART_ASSERT(cfrictions.size() == _skeleton->getNumDofs(),
                "Coulomb friction coefficient vector size is not the same as the DOFs of the robot", );
            for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
                _skeleton->getDof(i)->setCoulombFriction(cfrictions[i]);
            }
        }
        else {
            ROBOT_DART_ASSERT(cfrictions.size() == dof_names.size(),
                "Coulomb friction coefficient vector size is not the same as the dof_names size", );
            for (size_t i = 0; i < dof_names.size(); i++) {
                auto it = _dof_map.find(dof_names[i]);
                ROBOT_DART_ASSERT(it != _dof_map.end(), "set_coulomb_coeffs: " + dof_names[i] + " is not in dof_map", );
                _skeleton->getDof(it->second)->setCoulombFriction(cfrictions[i]);
            }
        }
    }

    void Robot::set_coulomb_coeffs(double cfriction, const std::vector<std::string>& dof_names)
    {
        size_t n_dofs = dof_names.size();
        if (n_dofs == 0)
            n_dofs = _skeleton->getNumDofs();
        std::vector<double> cfrictions(n_dofs, cfriction);

        set_coulomb_coeffs(cfrictions, dof_names);
    }

    std::vector<double> Robot::coulomb_coeffs(const std::vector<std::string>& dof_names) const
    {
        std::vector<double> cfrictions;
        if (dof_names.size() == 0) {
            for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
                cfrictions.push_back(_skeleton->getDof(i)->getCoulombFriction());
            }
        }
        else {
            for (size_t i = 0; i < dof_names.size(); i++) {
                auto it = _dof_map.find(dof_names[i]);
                ROBOT_DART_ASSERT(it != _dof_map.end(), "coulomb_coeffs: " + dof_names[i] + " is not in dof_map", std::vector<double>());
                cfrictions.push_back(_skeleton->getDof(it->second)->getCoulombFriction());
            }
        }

        return cfrictions;
    }

    void Robot::set_spring_stiffnesses(const std::vector<double>& stiffnesses, const std::vector<std::string>& dof_names)
    {
        size_t n_dofs = dof_names.size();
        if (n_dofs == 0) {
            ROBOT_DART_ASSERT(stiffnesses.size() == _skeleton->getNumDofs(),
                "Spring stiffnesses vector size is not the same as the DOFs of the robot", );
            for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
                _skeleton->getDof(i)->setSpringStiffness(stiffnesses[i]);
            }
        }
        else {
            ROBOT_DART_ASSERT(stiffnesses.size() == dof_names.size(),
                "Spring stiffnesses vector size is not the same as the dof_names size", );
            for (size_t i = 0; i < dof_names.size(); i++) {
                auto it = _dof_map.find(dof_names[i]);
                ROBOT_DART_ASSERT(it != _dof_map.end(), "set_spring_stiffnesses: " + dof_names[i] + " is not in dof_map", );
                _skeleton->getDof(it->second)->setSpringStiffness(stiffnesses[i]);
            }
        }
    }

    void Robot::set_spring_stiffnesses(double stiffness, const std::vector<std::string>& dof_names)
    {
        size_t n_dofs = dof_names.size();
        if (n_dofs == 0)
            n_dofs = _skeleton->getNumDofs();
        std::vector<double> stiffnesses(n_dofs, stiffness);

        set_spring_stiffnesses(stiffnesses, dof_names);
    }

    std::vector<double> Robot::spring_stiffnesses(const std::vector<std::string>& dof_names) const
    {
        std::vector<double> stiffnesses;
        if (dof_names.size() == 0) {
            for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
                stiffnesses.push_back(_skeleton->getDof(i)->getSpringStiffness());
            }
        }
        else {
            for (size_t i = 0; i < dof_names.size(); i++) {
                auto it = _dof_map.find(dof_names[i]);
                ROBOT_DART_ASSERT(it != _dof_map.end(), "spring_stiffnesses: " + dof_names[i] + " is not in dof_map", std::vector<double>());
                stiffnesses.push_back(_skeleton->getDof(it->second)->getSpringStiffness());
            }
        }

        return stiffnesses;
    }

#if DART_VERSION_AT_LEAST(6, 10, 0)
    auto body_node_set_friction_dir = [](dart::dynamics::BodyNode* body, const Eigen::Vector3d& direction) {
        auto& dyn_shapes = body->getShapeNodesWith<dart::dynamics::DynamicsAspect>();
        for (auto& shape : dyn_shapes) {
            const auto& dyn = shape->getDynamicsAspect();
            dyn->setFirstFrictionDirection(direction);
            dyn->setFirstFrictionDirectionFrame(body);
        }
    };
#endif

    void Robot::set_friction_dir(const std::string& body_name, const Eigen::Vector3d& direction)
    {
#if DART_VERSION_AT_LEAST(6, 10, 0)
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", );

        body_node_set_friction_dir(bd, direction);
#else
        ROBOT_DART_WARNING(true, "DART supports the frictional direction from v.6.10 onwards!");
#endif
    }

    void Robot::set_friction_dir(size_t body_index, const Eigen::Vector3d& direction)
    {
#if DART_VERSION_AT_LEAST(6, 10, 0)
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", );

        body_node_set_friction_dir(_skeleton->getBodyNode(body_index), direction);
#else
        ROBOT_DART_WARNING(true, "DART supports the frictional direction from v.6.10 onwards!");
#endif
    }

#if DART_VERSION_AT_LEAST(6, 10, 0)
    auto body_node_get_friction_dir = [](dart::dynamics::BodyNode* body) {
        auto& dyn_shapes = body->getShapeNodesWith<dart::dynamics::DynamicsAspect>();
        for (auto& shape : dyn_shapes) {
            const auto& dyn = shape->getDynamicsAspect();
            return dyn->getFirstFrictionDirection(); // assume all shape nodes have the same friction direction
        }

        return Eigen::Vector3d(Eigen::Vector3d::Zero());
    };
#endif

    Eigen::Vector3d Robot::friction_dir(const std::string& body_name)
    {
#if DART_VERSION_AT_LEAST(6, 10, 0)
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", Eigen::Vector3d::Zero());

        return body_node_get_friction_dir(bd);
#else
        ROBOT_DART_WARNING(true, "DART supports the frictional direction from v.6.10 onwards!");
        return Eigen::Vector3d::Zero();
#endif
    }

    Eigen::Vector3d Robot::friction_dir(size_t body_index)
    {
#if DART_VERSION_AT_LEAST(6, 10, 0)
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", Eigen::Vector3d::Zero());

        return body_node_get_friction_dir(_skeleton->getBodyNode(body_index));
#else
        ROBOT_DART_WARNING(true, "DART supports the frictional direction from v.6.10 onwards!");
        return Eigen::Vector3d::Zero();
#endif
    }

    auto body_node_set_friction_coeff = [](dart::dynamics::BodyNode* body, double value) {
#if DART_VERSION_AT_LEAST(6, 10, 0)
        auto& dyn_shapes = body->getShapeNodesWith<dart::dynamics::DynamicsAspect>();
        for (auto& shape : dyn_shapes) {
            shape->getDynamicsAspect()->setFrictionCoeff(value);
        }
#else
        body->setFrictionCoeff(value);
#endif
    };

    void Robot::set_friction_coeff(const std::string& body_name, double value)
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", );

        body_node_set_friction_coeff(bd, value);
    }

    void Robot::set_friction_coeff(size_t body_index, double value)
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", );

        body_node_set_friction_coeff(_skeleton->getBodyNode(body_index), value);
    }

    void Robot::set_friction_coeffs(double value)
    {
        for (auto bd : _skeleton->getBodyNodes())
            body_node_set_friction_coeff(bd, value);
    }

    auto body_node_get_friction_coeff = [](dart::dynamics::BodyNode* body) {
#if DART_VERSION_AT_LEAST(6, 10, 0)
        auto& dyn_shapes = body->getShapeNodesWith<dart::dynamics::DynamicsAspect>();
        for (auto& shape : dyn_shapes) {
            return shape->getDynamicsAspect()->getFrictionCoeff(); // assume all shape nodes have the same friction
        }

        return 0.;
#else
        return body->getFrictionCoeff();
#endif
    };

    double Robot::friction_coeff(const std::string& body_name)
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", 0.);

        return body_node_get_friction_coeff(bd);
    }

    double Robot::friction_coeff(size_t body_index)
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", 0.);
        return body_node_get_friction_coeff(_skeleton->getBodyNode(body_index));
    }

#if DART_VERSION_AT_LEAST(6, 10, 0)
    auto body_node_set_secondary_friction_coeff = [](dart::dynamics::BodyNode* body, double value) {
        auto& dyn_shapes = body->getShapeNodesWith<dart::dynamics::DynamicsAspect>();
        for (auto& shape : dyn_shapes) {
            shape->getDynamicsAspect()->setSecondaryFrictionCoeff(value);
        }
    };
#endif

    void Robot::set_secondary_friction_coeff(const std::string& body_name, double value)
    {
#if DART_VERSION_AT_LEAST(6, 10, 0)
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", );

        body_node_set_secondary_friction_coeff(bd, value);
#else
        ROBOT_DART_WARNING(true, "DART supports the secondary friction coefficient from v.6.10 onwards!");
#endif
    }

    void Robot::set_secondary_friction_coeff(size_t body_index, double value)
    {
#if DART_VERSION_AT_LEAST(6, 10, 0)
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", );

        body_node_set_secondary_friction_coeff(_skeleton->getBodyNode(body_index), value);
#else
        ROBOT_DART_WARNING(true, "DART supports the secondary friction coefficient from v.6.10 onwards!");
#endif
    }

    void Robot::set_secondary_friction_coeffs(double value)
    {
#if DART_VERSION_AT_LEAST(6, 10, 0)
        for (auto bd : _skeleton->getBodyNodes())
            body_node_set_secondary_friction_coeff(bd, value);
#else
        ROBOT_DART_WARNING(true, "DART supports the secondary friction coefficient from v.6.10 onwards!");
#endif
    }

#if DART_VERSION_AT_LEAST(6, 10, 0)
    auto body_node_get_secondary_friction_coeff = [](dart::dynamics::BodyNode* body) {
        auto& dyn_shapes = body->getShapeNodesWith<dart::dynamics::DynamicsAspect>();
        for (auto& shape : dyn_shapes) {
            return shape->getDynamicsAspect()->getSecondaryFrictionCoeff(); // assume all shape nodes have the same friction
        }

        return 0.;
    };
#endif

    double Robot::secondary_friction_coeff(const std::string& body_name)
    {
#if DART_VERSION_AT_LEAST(6, 10, 0)
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", 0.);

        return body_node_get_secondary_friction_coeff(bd);
#else
        ROBOT_DART_WARNING(true, "DART supports the secondary friction coefficient from v.6.10 onwards!");
        return 0.;
#endif
    }

    double Robot::secondary_friction_coeff(size_t body_index)
    {
#if DART_VERSION_AT_LEAST(6, 10, 0)
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", 0.);
        return body_node_get_secondary_friction_coeff(_skeleton->getBodyNode(body_index));
#else
        ROBOT_DART_WARNING(true, "DART supports the secondary friction coefficient from v.6.10 onwards!");
        return 0.;
#endif
    }

    auto body_node_set_restitution_coeff = [](dart::dynamics::BodyNode* body, double value) {
#if DART_VERSION_AT_LEAST(6, 10, 0)
        auto& dyn_shapes = body->getShapeNodesWith<dart::dynamics::DynamicsAspect>();
        for (auto& shape : dyn_shapes) {
            shape->getDynamicsAspect()->setRestitutionCoeff(value);
        }
#else
        body->setRestitutionCoeff(value);
#endif
    };

    void Robot::set_restitution_coeff(const std::string& body_name, double value)
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", );

        body_node_set_restitution_coeff(bd, value);
    }

    void Robot::set_restitution_coeff(size_t body_index, double value)
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", );

        body_node_set_restitution_coeff(_skeleton->getBodyNode(body_index), value);
    }

    void Robot::set_restitution_coeffs(double value)
    {
        for (auto bd : _skeleton->getBodyNodes())
            body_node_set_restitution_coeff(bd, value);
    }

    auto body_node_get_restitution_coeff = [](dart::dynamics::BodyNode* body) {
#if DART_VERSION_AT_LEAST(6, 10, 0)
        auto& dyn_shapes = body->getShapeNodesWith<dart::dynamics::DynamicsAspect>();
        for (auto& shape : dyn_shapes) {
            return shape->getDynamicsAspect()->getRestitutionCoeff(); // assume all shape nodes have the same restitution
        }

        return 0.;
#else
        return body->getRestitutionCoeff();
#endif
    };

    double Robot::restitution_coeff(const std::string& body_name)
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", 0.);

        return body_node_get_restitution_coeff(bd);
    }

    double Robot::restitution_coeff(size_t body_index)
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", 0.);

        return body_node_get_restitution_coeff(_skeleton->getBodyNode(body_index));
    }

    Eigen::Isometry3d Robot::base_pose() const
    {
        if (free())
            return dart::math::expMap(_skeleton->getPositions().head(6));
        auto jt = _skeleton->getRootBodyNode()->getParentJoint();
        ROBOT_DART_ASSERT(jt != nullptr, "Skeleton does not have a proper root BodyNode!",
            Eigen::Isometry3d::Identity());
        return jt->getTransformFromParentBodyNode();
    }

    Eigen::Vector6d Robot::base_pose_vec() const
    {
        if (free())
            return _skeleton->getPositions().head(6);
        auto jt = _skeleton->getRootBodyNode()->getParentJoint();
        ROBOT_DART_ASSERT(jt != nullptr, "Skeleton does not have a proper root BodyNode!",
            Eigen::Vector6d::Zero());
        return dart::math::logMap(jt->getTransformFromParentBodyNode());
    }

    void Robot::set_base_pose(const Eigen::Isometry3d& tf)
    {
        auto jt = _skeleton->getRootBodyNode()->getParentJoint();
        if (jt) {
            if (free())
                jt->setPositions(dart::math::logMap(tf));
            else
                jt->setTransformFromParentBodyNode(tf);
        }
    }

    void Robot::set_base_pose(const Eigen::Vector6d& pose)
    {
        auto jt = _skeleton->getRootBodyNode()->getParentJoint();
        if (jt) {
            if (free())
                jt->setPositions(pose);
            else
                jt->setTransformFromParentBodyNode(dart::math::expMap(pose));
        }
    }

    size_t Robot::num_dofs() const { return _skeleton->getNumDofs(); }

    size_t Robot::num_joints() const { return _skeleton->getNumJoints(); }

    size_t Robot::num_bodies() const { return _skeleton->getNumBodyNodes(); }

    Eigen::Vector3d Robot::com() const { return _skeleton->getCOM(); }

    Eigen::Vector6d Robot::com_velocity() const { return _skeleton->getCOMSpatialVelocity(); }

    Eigen::Vector6d Robot::com_acceleration() const { return _skeleton->getCOMSpatialAcceleration(); }

    Eigen::VectorXd Robot::positions(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<0>(_skeleton, dof_names, _dof_map);
    }

    void Robot::set_positions(const Eigen::VectorXd& positions, const std::vector<std::string>& dof_names)
    {
        detail::set_dof_data<0>(positions, _skeleton, dof_names, _dof_map);
    }

    Eigen::VectorXd Robot::position_lower_limits(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<5>(_skeleton, dof_names, _dof_map);
    }

    void Robot::set_position_lower_limits(const Eigen::VectorXd& positions, const std::vector<std::string>& dof_names)
    {
        detail::set_dof_data<5>(positions, _skeleton, dof_names, _dof_map);
    }

    Eigen::VectorXd Robot::position_upper_limits(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<6>(_skeleton, dof_names, _dof_map);
    }

    void Robot::set_position_upper_limits(const Eigen::VectorXd& positions, const std::vector<std::string>& dof_names)
    {
        detail::set_dof_data<6>(positions, _skeleton, dof_names, _dof_map);
    }

    Eigen::VectorXd Robot::velocities(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<1>(_skeleton, dof_names, _dof_map);
    }

    void Robot::set_velocities(const Eigen::VectorXd& velocities, const std::vector<std::string>& dof_names)
    {
        detail::set_dof_data<1>(velocities, _skeleton, dof_names, _dof_map);
    }

    Eigen::VectorXd Robot::velocity_lower_limits(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<7>(_skeleton, dof_names, _dof_map);
    }

    void Robot::set_velocity_lower_limits(const Eigen::VectorXd& velocities, const std::vector<std::string>& dof_names)
    {
        detail::set_dof_data<7>(velocities, _skeleton, dof_names, _dof_map);
    }

    Eigen::VectorXd Robot::velocity_upper_limits(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<8>(_skeleton, dof_names, _dof_map);
    }

    void Robot::set_velocity_upper_limits(const Eigen::VectorXd& velocities, const std::vector<std::string>& dof_names)
    {
        detail::set_dof_data<8>(velocities, _skeleton, dof_names, _dof_map);
    }

    Eigen::VectorXd Robot::accelerations(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<2>(_skeleton, dof_names, _dof_map);
    }

    void Robot::set_accelerations(const Eigen::VectorXd& accelerations, const std::vector<std::string>& dof_names)
    {
        detail::set_dof_data<2>(accelerations, _skeleton, dof_names, _dof_map);
    }

    Eigen::VectorXd Robot::acceleration_lower_limits(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<9>(_skeleton, dof_names, _dof_map);
    }

    void Robot::set_acceleration_lower_limits(const Eigen::VectorXd& accelerations, const std::vector<std::string>& dof_names)
    {
        detail::set_dof_data<9>(accelerations, _skeleton, dof_names, _dof_map);
    }

    Eigen::VectorXd Robot::acceleration_upper_limits(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<10>(_skeleton, dof_names, _dof_map);
    }

    void Robot::set_acceleration_upper_limits(const Eigen::VectorXd& accelerations, const std::vector<std::string>& dof_names)
    {
        detail::set_dof_data<10>(accelerations, _skeleton, dof_names, _dof_map);
    }

    Eigen::VectorXd Robot::forces(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<3>(_skeleton, dof_names, _dof_map);
    }

    void Robot::set_forces(const Eigen::VectorXd& forces, const std::vector<std::string>& dof_names)
    {
        detail::set_dof_data<3>(forces, _skeleton, dof_names, _dof_map);
    }

    Eigen::VectorXd Robot::force_lower_limits(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<11>(_skeleton, dof_names, _dof_map);
    }

    void Robot::set_force_lower_limits(const Eigen::VectorXd& forces, const std::vector<std::string>& dof_names)
    {
        detail::set_dof_data<11>(forces, _skeleton, dof_names, _dof_map);
    }

    Eigen::VectorXd Robot::force_upper_limits(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<12>(_skeleton, dof_names, _dof_map);
    }

    void Robot::set_force_upper_limits(const Eigen::VectorXd& forces, const std::vector<std::string>& dof_names)
    {
        detail::set_dof_data<12>(forces, _skeleton, dof_names, _dof_map);
    }

    Eigen::VectorXd Robot::commands(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<4>(_skeleton, dof_names, _dof_map);
    }

    void Robot::set_commands(const Eigen::VectorXd& commands, const std::vector<std::string>& dof_names)
    {
        detail::set_dof_data<4>(commands, _skeleton, dof_names, _dof_map);
    }

    std::pair<Eigen::Vector6d, Eigen::Vector6d> Robot::force_torque(size_t joint_index) const
    {
        ROBOT_DART_ASSERT(joint_index < _skeleton->getNumJoints(), "Joint index out of bounds", {});
        auto jt = _skeleton->getJoint(joint_index);

        Eigen::Vector6d F1 = Eigen::Vector6d::Zero();
        Eigen::Vector6d F2 = Eigen::Vector6d::Zero();
        Eigen::Isometry3d T12 = jt->getRelativeTransform();

        auto child_body = jt->getChildBodyNode();
        // ROBOT_DART_ASSERT(child_body != nullptr, "Child BodyNode is nullptr", {});
        if (child_body)
            F2 = -dart::math::dAdT(jt->getTransformFromChildBodyNode(), child_body->getBodyForce());

        F1 = -dart::math::dAdInvR(T12, F2);

        // F1 contains the force applied by the parent Link on the Joint specified in the parent
        // Link frame, F2 contains the force applied by the child Link on the Joint specified in
        // the child Link frame
        return {F1, F2};
    }

    void Robot::set_external_force(const std::string& body_name, const Eigen::Vector3d& force, const Eigen::Vector3d& offset, bool force_local, bool offset_local)
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", );

        bd->setExtForce(force, offset, force_local, offset_local);
    }

    void Robot::set_external_force(size_t body_index, const Eigen::Vector3d& force, const Eigen::Vector3d& offset, bool force_local, bool offset_local)
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", );
        auto bd = _skeleton->getBodyNode(body_index);

        bd->setExtForce(force, offset, force_local, offset_local);
    }

    void Robot::add_external_force(const std::string& body_name, const Eigen::Vector3d& force, const Eigen::Vector3d& offset, bool force_local, bool offset_local)
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", );

        bd->addExtForce(force, offset, force_local, offset_local);
    }

    void Robot::add_external_force(size_t body_index, const Eigen::Vector3d& force, const Eigen::Vector3d& offset, bool force_local, bool offset_local)
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", );
        auto bd = _skeleton->getBodyNode(body_index);

        bd->addExtForce(force, offset, force_local, offset_local);
    }

    void Robot::set_external_torque(const std::string& body_name, const Eigen::Vector3d& torque, bool local)
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", );

        bd->setExtTorque(torque, local);
    }

    void Robot::set_external_torque(size_t body_index, const Eigen::Vector3d& torque, bool local)
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", );
        auto bd = _skeleton->getBodyNode(body_index);

        bd->setExtTorque(torque, local);
    }

    void Robot::add_external_torque(const std::string& body_name, const Eigen::Vector3d& torque, bool local)
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", );

        bd->addExtTorque(torque, local);
    }

    void Robot::add_external_torque(size_t body_index, const Eigen::Vector3d& torque, bool local)
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", );
        auto bd = _skeleton->getBodyNode(body_index);

        bd->addExtTorque(torque, local);
    }

    Eigen::Vector6d Robot::external_forces(const std::string& body_name) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", Eigen::Vector6d::Zero());

        return bd->getExternalForceGlobal();
    }

    Eigen::Vector6d Robot::external_forces(size_t body_index) const
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds",
            Eigen::Vector6d::Zero());
        auto bd = _skeleton->getBodyNode(body_index);

        return bd->getExternalForceGlobal();
    }

    Eigen::Isometry3d Robot::body_pose(const std::string& body_name) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", Eigen::Isometry3d::Identity());
        return bd->getWorldTransform();
    }

    Eigen::Isometry3d Robot::body_pose(size_t body_index) const
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", Eigen::Isometry3d::Identity());
        return _skeleton->getBodyNode(body_index)->getWorldTransform();
    }

    Eigen::Vector6d Robot::body_pose_vec(const std::string& body_name) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", Eigen::Vector6d::Zero());

        return dart::math::logMap(bd->getWorldTransform());
    }

    Eigen::Vector6d Robot::body_pose_vec(size_t body_index) const
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", Eigen::Vector6d::Zero());

        Eigen::Isometry3d bd_trans = _skeleton->getBodyNode(body_index)->getWorldTransform();

        return dart::math::logMap(bd_trans);
    }

    Eigen::Vector6d Robot::body_velocity(const std::string& body_name) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", Eigen::Vector6d::Zero());
        return bd->getSpatialVelocity(dart::dynamics::Frame::World(), dart::dynamics::Frame::World());
    }

    Eigen::Vector6d Robot::body_velocity(size_t body_index) const
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", Eigen::Vector6d::Zero());
        return _skeleton->getBodyNode(body_index)->getSpatialVelocity(dart::dynamics::Frame::World(), dart::dynamics::Frame::World());
    }

    Eigen::Vector6d Robot::body_acceleration(const std::string& body_name) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", Eigen::Vector6d::Zero());
        return bd->getSpatialAcceleration(dart::dynamics::Frame::World(), dart::dynamics::Frame::World());
    }

    Eigen::Vector6d Robot::body_acceleration(size_t body_index) const
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", Eigen::Vector6d::Zero());
        return _skeleton->getBodyNode(body_index)->getSpatialAcceleration(dart::dynamics::Frame::World(), dart::dynamics::Frame::World());
    }

    std::vector<std::string> Robot::body_names() const
    {
        std::vector<std::string> names;
        for (auto& bd : _skeleton->getBodyNodes())
            names.push_back(bd->getName());
        return names;
    }

    std::string Robot::body_name(size_t body_index) const
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", "");
        return _skeleton->getBodyNode(body_index)->getName();
    }

    void Robot::set_body_name(size_t body_index, const std::string& body_name)
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", );
        _skeleton->getBodyNode(body_index)->setName(body_name);
    }

    size_t Robot::body_index(const std::string& body_name) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd, "body_index : " + body_name + " is not in the skeleton", 0);
        return bd->getIndexInSkeleton();
    }

    double Robot::body_mass(const std::string& body_name) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", 0.);
        return bd->getMass();
    }

    double Robot::body_mass(size_t body_index) const
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", 0.);
        return _skeleton->getBodyNode(body_index)->getMass();
    }

    void Robot::set_body_mass(const std::string& body_name, double mass)
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", );
        bd->setMass(mass); // TO-DO: Recompute inertia?
    }

    void Robot::set_body_mass(size_t body_index, double mass)
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", );
        _skeleton->getBodyNode(body_index)->setMass(mass); // TO-DO: Recompute inertia?
    }

    void Robot::add_body_mass(const std::string& body_name, double mass)
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", );
        bd->setMass(mass + bd->getMass()); // TO-DO: Recompute inertia?
    }

    void Robot::add_body_mass(size_t body_index, double mass)
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", );
        auto bd = _skeleton->getBodyNode(body_index);
        bd->setMass(mass + bd->getMass()); // TO-DO: Recompute inertia?
    }

    Eigen::MatrixXd Robot::jacobian(const std::string& body_name, const std::vector<std::string>& dof_names) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", Eigen::MatrixXd());

        Eigen::MatrixXd jac = _skeleton->getWorldJacobian(bd);

        return _jacobian(jac, dof_names);
    }

    Eigen::MatrixXd Robot::jacobian_deriv(const std::string& body_name, const std::vector<std::string>& dof_names) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", Eigen::MatrixXd());

        Eigen::MatrixXd jac = _skeleton->getJacobianSpatialDeriv(bd, dart::dynamics::Frame::World());

        return _jacobian(jac, dof_names);
    }

    Eigen::MatrixXd Robot::com_jacobian(const std::vector<std::string>& dof_names) const
    {
        Eigen::MatrixXd jac = _skeleton->getCOMJacobian();

        return _jacobian(jac, dof_names);
    }

    Eigen::MatrixXd Robot::com_jacobian_deriv(const std::vector<std::string>& dof_names) const
    {
        Eigen::MatrixXd jac = _skeleton->getCOMJacobianSpatialDeriv();

        return _jacobian(jac, dof_names);
    }

    Eigen::MatrixXd Robot::mass_matrix(const std::vector<std::string>& dof_names) const
    {
        Eigen::MatrixXd M = _skeleton->getMassMatrix();

        return _mass_matrix(M, dof_names);
    }

    Eigen::MatrixXd Robot::aug_mass_matrix(const std::vector<std::string>& dof_names) const
    {
        Eigen::MatrixXd M = _skeleton->getAugMassMatrix();

        return _mass_matrix(M, dof_names);
    }

    Eigen::MatrixXd Robot::inv_mass_matrix(const std::vector<std::string>& dof_names) const
    {
        Eigen::MatrixXd M = _skeleton->getInvMassMatrix();

        return _mass_matrix(M, dof_names);
    }

    Eigen::MatrixXd Robot::inv_aug_mass_matrix(const std::vector<std::string>& dof_names) const
    {
        Eigen::MatrixXd M = _skeleton->getInvAugMassMatrix();

        return _mass_matrix(M, dof_names);
    }

    Eigen::VectorXd Robot::coriolis_forces(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<13>(_skeleton, dof_names, _dof_map);
    }

    Eigen::VectorXd Robot::gravity_forces(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<14>(_skeleton, dof_names, _dof_map);
    }

    Eigen::VectorXd Robot::coriolis_gravity_forces(const std::vector<std::string>& dof_names) const
    {
        return detail::dof_data<15>(_skeleton, dof_names, _dof_map);
    }

    Eigen::VectorXd Robot::vec_dof(const Eigen::VectorXd& vec, const std::vector<std::string>& dof_names) const
    {
        assert(vec.size() == static_cast<int>(_skeleton->getNumDofs()));

        Eigen::VectorXd ret(dof_names.size());
        for (size_t i = 0; i < dof_names.size(); i++) {
            auto it = _dof_map.find(dof_names[i]);
            ROBOT_DART_ASSERT(it != _dof_map.end(), "vec_dof: " + dof_names[i] + " is not in dof_map", Eigen::VectorXd());

            ret(i) = vec[it->second];
        }

        return ret;
    }

    void Robot::update_joint_dof_maps()
    {
        // DoFs
        _dof_map.clear();
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i)
            _dof_map[_skeleton->getDof(i)->getName()] = i;

        // Joints
        _joint_map.clear();
        for (size_t i = 0; i < _skeleton->getNumJoints(); ++i)
            _joint_map[_skeleton->getJoint(i)->getName()] = i;
    }

    const std::unordered_map<std::string, size_t>& Robot::dof_map() const { return _dof_map; }

    const std::unordered_map<std::string, size_t>& Robot::joint_map() const { return _joint_map; }

    std::vector<std::string> Robot::dof_names(bool filter_mimics, bool filter_locked, bool filter_passive) const
    {
        std::vector<std::string> names;
        for (auto& dof : _skeleton->getDofs()) {
            auto jt = dof->getJoint();
            if ((!filter_mimics
#if DART_VERSION_AT_LEAST(6, 7, 0)
                    || jt->getActuatorType() != dart::dynamics::Joint::MIMIC
#else
                    || true
#endif
                    )
                && (!filter_locked || jt->getActuatorType() != dart::dynamics::Joint::LOCKED)
                && (!filter_passive || jt->getActuatorType() != dart::dynamics::Joint::PASSIVE))
                names.push_back(dof->getName());
        }
        return names;
    }

    std::vector<std::string> Robot::mimic_dof_names() const
    {
        std::vector<std::string> names;
#if DART_VERSION_AT_LEAST(6, 7, 0)
        for (auto& dof : _skeleton->getDofs()) {
            auto jt = dof->getJoint();
            if (jt->getActuatorType() == dart::dynamics::Joint::MIMIC)
                names.push_back(dof->getName());
        }
#endif
        return names;
    }

    std::vector<std::string> Robot::locked_dof_names() const
    {
        std::vector<std::string> names;
        for (auto& dof : _skeleton->getDofs()) {
            auto jt = dof->getJoint();
            if (jt->getActuatorType() == dart::dynamics::Joint::LOCKED)
                names.push_back(dof->getName());
        }
        return names;
    }

    std::vector<std::string> Robot::passive_dof_names() const
    {
        std::vector<std::string> names;
        for (auto& dof : _skeleton->getDofs()) {
            auto jt = dof->getJoint();
            if (jt->getActuatorType() == dart::dynamics::Joint::PASSIVE)
                names.push_back(dof->getName());
        }
        return names;
    }

    std::string Robot::dof_name(size_t dof_index) const
    {
        ROBOT_DART_ASSERT(dof_index < _skeleton->getNumDofs(), "Dof index out of bounds", "");
        return _skeleton->getDof(dof_index)->getName();
    }

    size_t Robot::dof_index(const std::string& dof_name) const
    {
        if (_dof_map.empty()) {
            ROBOT_DART_WARNING(true,
                "DoF map is empty. Iterating over all skeleton DoFs to get the index. Consider "
                "calling update_joint_dof_maps() before using dof_index()");
            for (size_t i = 0; i < _skeleton->getNumDofs(); i++)
                if (_skeleton->getDof(i)->getName() == dof_name)
                    return i;
            ROBOT_DART_ASSERT(false, "dof_index : " + dof_name + " is not in the skeleton", 0);
        }
        auto it = _dof_map.find(dof_name);
        ROBOT_DART_ASSERT(it != _dof_map.end(), "dof_index : " + dof_name + " is not in DoF map", 0);
        return it->second;
    }

    std::vector<std::string> Robot::joint_names() const
    {
        std::vector<std::string> names;
        for (auto& jt : _skeleton->getJoints())
            names.push_back(jt->getName());
        return names;
    }

    std::string Robot::joint_name(size_t joint_index) const
    {
        ROBOT_DART_ASSERT(joint_index < _skeleton->getNumJoints(), "Joint index out of bounds", "");
        return _skeleton->getJoint(joint_index)->getName();
    }

    void Robot::set_joint_name(size_t joint_index, const std::string& joint_name)
    {
        ROBOT_DART_ASSERT(joint_index < _skeleton->getNumJoints(), "Joint index out of bounds", );
        _skeleton->getJoint(joint_index)->setName(joint_name);

        update_joint_dof_maps();
    }

    size_t Robot::joint_index(const std::string& joint_name) const
    {
        if (_joint_map.empty()) {
            ROBOT_DART_WARNING(true,
                "Joint map is empty. Iterating over all skeleton joints to get the index. "
                "Consider calling update_joint_dof_maps() before using joint_index()");
            for (size_t i = 0; i < _skeleton->getNumJoints(); i++)
                if (_skeleton->getJoint(i)->getName() == joint_name)
                    return i;
            ROBOT_DART_ASSERT(false, "joint_index : " + joint_name + " is not in the skeleton", 0);
        }
        auto it = _joint_map.find(joint_name);
        ROBOT_DART_ASSERT(it != _joint_map.end(), "joint_index : " + joint_name + " is not in Joint map", 0);
        return it->second;
    }

    void Robot::set_color_mode(const std::string& color_mode)
    {
        if (color_mode == "material")
            _set_color_mode(dart::dynamics::MeshShape::ColorMode::MATERIAL_COLOR, _skeleton);
        else if (color_mode == "assimp")
            _set_color_mode(dart::dynamics::MeshShape::ColorMode::COLOR_INDEX, _skeleton);
        else if (color_mode == "aspect")
            _set_color_mode(dart::dynamics::MeshShape::ColorMode::SHAPE_COLOR, _skeleton);
        else
            ROBOT_DART_EXCEPTION_ASSERT(false, "Unknown color mode. Valid values: material, assimp and aspect.");
    }

    void Robot::set_color_mode(const std::string& color_mode, const std::string& body_name)
    {
        dart::dynamics::MeshShape::ColorMode cmode;
        if (color_mode == "material")
            cmode = dart::dynamics::MeshShape::ColorMode::MATERIAL_COLOR;
        else if (color_mode == "assimp")
            cmode = dart::dynamics::MeshShape::ColorMode::COLOR_INDEX;
        else if (color_mode == "aspect")
            cmode = dart::dynamics::MeshShape::ColorMode::SHAPE_COLOR;
        else
            ROBOT_DART_EXCEPTION_ASSERT(false, "Unknown color mode. Valid values: material, assimp and aspect.");

        auto bn = _skeleton->getBodyNode(body_name);
        if (bn) {
            for (size_t j = 0; j < bn->getNumShapeNodes(); ++j) {
                dart::dynamics::ShapeNode* sn = bn->getShapeNode(j);
                _set_color_mode(cmode, sn);
            }
        }
    }

    void Robot::set_self_collision(bool enable_self_collisions, bool enable_adjacent_collisions)
    {
        _skeleton->setSelfCollisionCheck(enable_self_collisions);
        _skeleton->setAdjacentBodyCheck(enable_adjacent_collisions);
    }

    bool Robot::self_colliding() const
    {
        return _skeleton->getSelfCollisionCheck();
    }

    bool Robot::adjacent_colliding() const
    {
        return _skeleton->getAdjacentBodyCheck() && self_colliding();
    }

    void Robot::set_cast_shadows(bool cast_shadows) { _cast_shadows = cast_shadows; }

    bool Robot::cast_shadows() const { return _cast_shadows; }

    void Robot::set_ghost(bool ghost) { _is_ghost = ghost; }

    bool Robot::ghost() const { return _is_ghost; }

    void Robot::set_draw_axis(const std::string& body_name, double size)
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd, "Body name does not exist in skeleton", );
        std::pair<dart::dynamics::BodyNode*, double> p = {bd, size};
        auto iter = std::find(_axis_shapes.begin(), _axis_shapes.end(), p);
        if (iter == _axis_shapes.end())
            _axis_shapes.push_back(p);
    }

    void Robot::remove_all_drawing_axis()
    {
        _axis_shapes.clear();
    }

    const std::vector<std::pair<dart::dynamics::BodyNode*, double>>& Robot::drawing_axes() const { return _axis_shapes; }

    std::string Robot::_get_path(const std::string& filename) const
    {
        namespace fs = boost::filesystem;
        fs::path model_file(boost::trim_copy(filename));
        if (model_file.string()[0] == '/')
            return "/";

        // search current directory
        if (fs::exists(model_file))
            return fs::current_path().string();

        // search <current_directory>/robots
        if (fs::exists(fs::path("robots") / model_file))
            return (fs::current_path() / fs::path("robots")).string();

        // search $ROBOT_DART_PATH
        const char* env = std::getenv("ROBOT_DART_PATH");
        if (env != nullptr) {
            fs::path env_path(env);
            if (fs::exists(env_path / model_file))
                return env_path.string();
        }

        // search PREFIX/share/robot_dart/robots
        fs::path system_path(std::string(ROBOT_DART_PREFIX) + "/share/robot_dart/robots/");
        if (fs::exists(system_path / model_file))
            return system_path.string();

        ROBOT_DART_EXCEPTION_ASSERT(false, std::string("Could not find :") + filename);

        return std::string();
    }

    dart::dynamics::SkeletonPtr Robot::_load_model(const std::string& filename, const std::vector<std::pair<std::string, std::string>>& packages, bool is_urdf_string)
    {
        ROBOT_DART_EXCEPTION_ASSERT(!filename.empty(), "Empty URDF filename");

        dart::dynamics::SkeletonPtr tmp_skel;
        if (!is_urdf_string) {
            // search for the right directory for our files
            std::string file_dir = _get_path(filename);
            std::string model_file = file_dir + '/' + boost::trim_copy(filename);
            // store the name for future use
            _model_filename = model_file;
            _packages = packages;
            // std::cout << "RobotDART:: using: " << model_file << std::endl;

            // in C++17 we would use std::filesystem!
            boost::filesystem::path path(model_file);
            std::string extension = path.extension().string();
            if (extension == ".urdf") {
                dart::io::DartLoader loader;
                for (size_t i = 0; i < packages.size(); i++) {
                    std::string package = std::get<1>(packages[i]);
                    std::string package_path = _get_path(package);
                    loader.addPackageDirectory(
                        std::get<0>(packages[i]), package_path + "/" + package);
                }
                tmp_skel = loader.parseSkeleton(model_file);
            }
            else if (extension == ".sdf")
                tmp_skel = dart::io::SdfParser::readSkeleton(model_file);
            else if (extension == ".skel") {
                tmp_skel = dart::io::SkelParser::readSkeleton(model_file);
                // if the skel file contains a world
                // try to read the skeleton with name 'robot_name'
                if (!tmp_skel) {
                    dart::simulation::WorldPtr world = dart::io::SkelParser::readWorld(model_file);
                    tmp_skel = world->getSkeleton(_robot_name);
                }
            }
            else
                return nullptr;
        }
        else {
            // Load from URDF string
            dart::io::DartLoader loader;
            for (size_t i = 0; i < packages.size(); i++) {
                std::string package = std::get<1>(packages[i]);
                std::string package_path = _get_path(package);
                loader.addPackageDirectory(std::get<0>(packages[i]), package_path + "/" + package);
            }
            tmp_skel = loader.parseSkeletonString(filename, "");
        }

        if (tmp_skel == nullptr)
            return nullptr;

        tmp_skel->setName(_robot_name);
        // Set joint limits
        for (size_t i = 0; i < tmp_skel->getNumJoints(); ++i) {
#if DART_VERSION_AT_LEAST(6, 10, 0)
            tmp_skel->getJoint(i)->setLimitEnforcement(true);
#else
            tmp_skel->getJoint(i)->setPositionLimitEnforced(true);
#endif
        }

        _set_color_mode(dart::dynamics::MeshShape::ColorMode::SHAPE_COLOR, tmp_skel);

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

        update_joint_dof_maps();
    }

    void Robot::_set_color_mode(dart::dynamics::MeshShape::ColorMode color_mode, dart::dynamics::SkeletonPtr skel)
    {
        for (size_t i = 0; i < skel->getNumBodyNodes(); ++i) {
            dart::dynamics::BodyNode* bn = skel->getBodyNode(i);
            for (size_t j = 0; j < bn->getNumShapeNodes(); ++j) {
                dart::dynamics::ShapeNode* sn = bn->getShapeNode(j);
                _set_color_mode(color_mode, sn);
            }
        }
    }

    void Robot::_set_color_mode(dart::dynamics::MeshShape::ColorMode color_mode, dart::dynamics::ShapeNode* sn)
    {
        if (sn->getVisualAspect()) {
            dart::dynamics::MeshShape* ms
                = dynamic_cast<dart::dynamics::MeshShape*>(sn->getShape().get());
            if (ms)
                ms->setColorMode(color_mode);
        }
    }

    void Robot::_set_actuator_type(size_t joint_index, dart::dynamics::Joint::ActuatorType type, bool override_mimic, bool override_base)
    {
        ROBOT_DART_ASSERT(joint_index < _skeleton->getNumJoints(), "joint_index index out of bounds", );
        auto jt = _skeleton->getJoint(joint_index);
        // Do not override 6D base if robot is free and override_base is false
        if (free() && (!override_base && _skeleton->getRootJoint() == jt))
            return;
#if DART_VERSION_AT_LEAST(6, 7, 0)
        if (override_mimic || jt->getActuatorType() != dart::dynamics::Joint::MIMIC)
#endif
            jt->setActuatorType(type);
    }

    void Robot::_set_actuator_types(const std::vector<dart::dynamics::Joint::ActuatorType>& types, bool override_mimic, bool override_base)
    {
        ROBOT_DART_ASSERT(types.size() == _skeleton->getNumJoints(), "Actuator types vector size is not the same as the joints of the robot", );
        // Ignore first root joint if robot is free, and override_base is false
        bool ignore_base = free() && !override_base;
        auto root_jt = _skeleton->getRootJoint();
        for (size_t i = 0; i < _skeleton->getNumJoints(); ++i) {
            auto jt = _skeleton->getJoint(i);
            if (jt->getNumDofs() == 0 || (ignore_base && jt == root_jt))
                continue;
#if DART_VERSION_AT_LEAST(6, 7, 0)
            if (override_mimic || jt->getActuatorType() != dart::dynamics::Joint::MIMIC)
#endif
                jt->setActuatorType(types[i]);
        }
    }

    void Robot::_set_actuator_types(dart::dynamics::Joint::ActuatorType type, bool override_mimic, bool override_base)
    {
        // Ignore first root joint if robot is free, and override_base is false
        bool ignore_base = free() && !override_base;
        auto root_jt = _skeleton->getRootJoint();
        for (size_t i = 0; i < _skeleton->getNumJoints(); ++i) {
            auto jt = _skeleton->getJoint(i);
            if (jt->getNumDofs() == 0 || (ignore_base && jt == root_jt))
                continue;
#if DART_VERSION_AT_LEAST(6, 7, 0)
            if (override_mimic || jt->getActuatorType() != dart::dynamics::Joint::MIMIC)
#endif
                jt->setActuatorType(type);
        }
    }

    dart::dynamics::Joint::ActuatorType Robot::_actuator_type(size_t joint_index) const
    {
        ROBOT_DART_ASSERT(joint_index < _skeleton->getNumJoints(), "joint_index out of bounds", dart::dynamics::Joint::ActuatorType::FORCE);
        return _skeleton->getJoint(joint_index)->getActuatorType();
    }

    std::vector<dart::dynamics::Joint::ActuatorType> Robot::_actuator_types() const
    {
        std::vector<dart::dynamics::Joint::ActuatorType> types;
        for (size_t i = 0; i < _skeleton->getNumJoints(); ++i) {
            types.push_back(_skeleton->getJoint(i)->getActuatorType());
        }

        return types;
    }

    Eigen::MatrixXd Robot::_jacobian(const Eigen::MatrixXd& full_jacobian, const std::vector<std::string>& dof_names) const
    {
        if (dof_names.empty())
            return full_jacobian;

        Eigen::MatrixXd jac_ret(6, dof_names.size());

        for (size_t i = 0; i < dof_names.size(); i++) {
            auto it = _dof_map.find(dof_names[i]);
            ROBOT_DART_ASSERT(it != _dof_map.end(), "_jacobian: " + dof_names[i] + " is not in dof_map", Eigen::MatrixXd());

            jac_ret.col(i) = full_jacobian.col(it->second);
        }

        return jac_ret;
    }

    Eigen::MatrixXd Robot::_mass_matrix(const Eigen::MatrixXd& full_mass_matrix, const std::vector<std::string>& dof_names) const
    {
        if (dof_names.empty())
            return full_mass_matrix;

        Eigen::MatrixXd M_ret(dof_names.size(), dof_names.size());

        for (size_t i = 0; i < dof_names.size(); i++) {
            auto it = _dof_map.find(dof_names[i]);
            ROBOT_DART_ASSERT(it != _dof_map.end(), "mass_matrix: " + dof_names[i] + " is not in dof_map", Eigen::MatrixXd());

            M_ret(i, i) = full_mass_matrix(it->second, it->second);

            for (size_t j = i + 1; j < dof_names.size(); j++) {
                auto it2 = _dof_map.find(dof_names[j]);
                ROBOT_DART_ASSERT(it2 != _dof_map.end(), "mass_matrix: " + dof_names[j] + " is not in dof_map", Eigen::MatrixXd());

                M_ret(i, j) = full_mass_matrix(it->second, it2->second);
                M_ret(j, i) = full_mass_matrix(it2->second, it->second);
            }
        }

        return M_ret;
    }

    std::shared_ptr<Robot> Robot::create_box(const Eigen::Vector3d& dims, const Eigen::Isometry3d& tf, const std::string& type, double mass, const Eigen::Vector4d& color, const std::string& box_name)
    {
        return create_box(dims, dart::math::logMap(tf), type, mass, color, box_name);
    }

    std::shared_ptr<Robot> Robot::create_box(const Eigen::Vector3d& dims, const Eigen::Vector6d& pose, const std::string& type, double mass, const Eigen::Vector4d& color, const std::string& box_name)
    {
        ROBOT_DART_ASSERT((dims.array() > 0.).all(), "Dimensions should be bigger than zero!", nullptr);
        ROBOT_DART_ASSERT(mass > 0., "Box mass should be bigger than zero!", nullptr);

        dart::dynamics::SkeletonPtr box_skel = dart::dynamics::Skeleton::create(box_name);

        // Give the box a body
        dart::dynamics::BodyNodePtr body;
        if (type == "free")
            body = box_skel->createJointAndBodyNodePair<dart::dynamics::FreeJoint>(nullptr).second;
        else
            body = box_skel->createJointAndBodyNodePair<dart::dynamics::WeldJoint>(nullptr).second;
        body->setName(box_name);

        // Give the body a shape
        auto box = std::make_shared<dart::dynamics::BoxShape>(dims);
        auto box_node = body->createShapeNodeWith<dart::dynamics::VisualAspect,
            dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(box);
        box_node->getVisualAspect()->setColor(color);
        // Set up inertia
        dart::dynamics::Inertia inertia;
        inertia.setMass(mass);
        inertia.setMoment(box->computeInertia(mass));
        body->setInertia(inertia);

        // Put the body into position
        auto robot = std::make_shared<Robot>(box_skel, box_name);

        if (type == "free") // free floating
            robot->set_positions(pose);
        else // fixed
            body->getParentJoint()->setTransformFromParentBodyNode(dart::math::expMap(pose));

        return robot;
    }

    std::shared_ptr<Robot> Robot::create_ellipsoid(const Eigen::Vector3d& dims, const Eigen::Isometry3d& tf, const std::string& type, double mass, const Eigen::Vector4d& color, const std::string& ellipsoid_name)
    {
        return create_ellipsoid(dims, dart::math::logMap(tf), type, mass, color, ellipsoid_name);
    }

    std::shared_ptr<Robot> Robot::create_ellipsoid(const Eigen::Vector3d& dims, const Eigen::Vector6d& pose, const std::string& type, double mass, const Eigen::Vector4d& color, const std::string& ellipsoid_name)
    {
        ROBOT_DART_ASSERT((dims.array() > 0.).all(), "Dimensions should be bigger than zero!", nullptr);
        ROBOT_DART_ASSERT(mass > 0., "Box mass should be bigger than zero!", nullptr);

        dart::dynamics::SkeletonPtr ellipsoid_skel = dart::dynamics::Skeleton::create(ellipsoid_name);

        // Give the ellipsoid a body
        dart::dynamics::BodyNodePtr body;
        if (type == "free")
            body = ellipsoid_skel->createJointAndBodyNodePair<dart::dynamics::FreeJoint>(nullptr).second;
        else
            body = ellipsoid_skel->createJointAndBodyNodePair<dart::dynamics::WeldJoint>(nullptr).second;
        body->setName(ellipsoid_name);

        // Give the body a shape
        auto ellipsoid = std::make_shared<dart::dynamics::EllipsoidShape>(dims);
        auto ellipsoid_node = body->createShapeNodeWith<dart::dynamics::VisualAspect,
            dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(ellipsoid);
        ellipsoid_node->getVisualAspect()->setColor(color);
        // Set up inertia
        dart::dynamics::Inertia inertia;
        inertia.setMass(mass);
        inertia.setMoment(ellipsoid->computeInertia(mass));
        body->setInertia(inertia);

        auto robot = std::make_shared<Robot>(ellipsoid_skel, ellipsoid_name);

        // Put the body into position
        if (type == "free") // free floating
            robot->set_positions(pose);
        else // fixed
        {
            Eigen::Isometry3d T;
            T.linear() = dart::math::eulerXYZToMatrix(pose.head(3));
            T.translation() = pose.tail(3);
            body->getParentJoint()->setTransformFromParentBodyNode(T);
        }

        return robot;
    }
} // namespace robot_dart