#include "robot.hpp"
#include "utils.hpp"

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

// namespace alias for compatibility
namespace dart {
    namespace io = utils;
}
#endif

#include <robot_dart/control/robot_control.hpp>

namespace robot_dart {
    Robot::Robot(const std::string& model_file, const std::vector<std::pair<std::string, std::string>>& packages, const std::string& robot_name, bool is_urdf_string, std::vector<RobotDamage> damages) : _robot_name(robot_name), _skeleton(_load_model(model_file, packages, is_urdf_string))
    {
        ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(_skeleton != nullptr);
        _set_damages(damages);
        update_dof_map();
    }

    Robot::Robot(const std::string& model_file, const std::string& robot_name, bool is_urdf_string, std::vector<RobotDamage> damages) : Robot(model_file, std::vector<std::pair<std::string, std::string>>(), robot_name, is_urdf_string, damages) {}

    Robot::Robot(dart::dynamics::SkeletonPtr skeleton, const std::string& robot_name, std::vector<RobotDamage> damages) : _robot_name(robot_name), _skeleton(skeleton)
    {
        ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(_skeleton != nullptr);
        _skeleton->setName(robot_name);
        _set_damages(damages);
        update_dof_map();
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
        robot->_controllers.clear();
        for (auto& ctrl : _controllers) {
            robot->add_controller(ctrl->clone(), ctrl->weight());
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

    const std::string& Robot::name() const
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
        ROBOT_DART_ASSERT(index < _controllers.size(), "Controller index out of bounds", );
        _controllers.erase(_controllers.begin() + index);
    }

    void Robot::clear_controllers()
    {
        _controllers.clear();
    }

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

    void Robot::set_actuator_type(size_t dof, dart::dynamics::Joint::ActuatorType type, bool override_mimic)
    {
        ROBOT_DART_ASSERT(dof < _skeleton->getNumDofs(), "DOF index out of bounds", );
        auto jt = _skeleton->getDof(dof)->getJoint();
#if DART_VERSION_AT_LEAST(6, 7, 0)
        if (override_mimic || jt->getActuatorType() != dart::dynamics::Joint::MIMIC)
#endif
            jt->setActuatorType(type);
    }

    void Robot::set_actuator_types(const std::vector<dart::dynamics::Joint::ActuatorType>& types, bool override_mimic)
    {
        ROBOT_DART_ASSERT(types.size() == _skeleton->getNumDofs(), "Actuator types vector size is not the same as the DOFs of the robot", );
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            auto jt = _skeleton->getDof(i)->getJoint();
#if DART_VERSION_AT_LEAST(6, 7, 0)
            if (override_mimic || jt->getActuatorType() != dart::dynamics::Joint::MIMIC)
#endif
                jt->setActuatorType(types[i]);
        }
    }

    void Robot::set_actuator_types(dart::dynamics::Joint::ActuatorType type, bool override_mimic)
    {
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            auto jt = _skeleton->getDof(i)->getJoint();
#if DART_VERSION_AT_LEAST(6, 7, 0)
            if (override_mimic || jt->getActuatorType() != dart::dynamics::Joint::MIMIC)
#endif
                jt->setActuatorType(type);
        }
    }

    dart::dynamics::Joint::ActuatorType Robot::actuator_type(size_t dof) const
    {
        ROBOT_DART_ASSERT(dof < _skeleton->getNumDofs(), "DOF index out of bounds", dart::dynamics::Joint::ActuatorType::FORCE);
        return _skeleton->getDof(dof)->getJoint()->getActuatorType();
    }

    std::vector<dart::dynamics::Joint::ActuatorType> Robot::actuator_types() const
    {
        std::vector<dart::dynamics::Joint::ActuatorType> types;
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            types.push_back(_skeleton->getDof(i)->getJoint()->getActuatorType());
        }

        return types;
    }

    void Robot::set_position_enforced(size_t dof, bool enforced)
    {
        ROBOT_DART_ASSERT(dof < _skeleton->getNumDofs(), "DOF index out of bounds", );
#if DART_VERSION_AT_LEAST(6, 10, 0)
        _skeleton->getDof(dof)->getJoint()->setLimitEnforcement(enforced);
#else
        _skeleton->getDof(dof)->getJoint()->setPositionLimitEnforced(enforced);
#endif
    }

    void Robot::set_position_enforced(const std::vector<bool>& enforced)
    {
        ROBOT_DART_ASSERT(enforced.size() == _skeleton->getNumDofs(), "Position enforced vector size is not the same as the DOFs of the robot", );
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
#if DART_VERSION_AT_LEAST(6, 10, 0)
            _skeleton->getDof(i)->getJoint()->setLimitEnforcement(enforced[i]);
#else
            _skeleton->getDof(i)->getJoint()->setPositionLimitEnforced(enforced[i]);
#endif
        }
    }

    void Robot::set_position_enforced(bool enforced)
    {
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
#if DART_VERSION_AT_LEAST(6, 10, 0)
            _skeleton->getDof(i)->getJoint()->setLimitEnforcement(enforced);
#else
            _skeleton->getDof(i)->getJoint()->setPositionLimitEnforced(enforced);
#endif
        }
    }

    bool Robot::position_enforced(size_t dof) const
    {
        ROBOT_DART_ASSERT(dof < _skeleton->getNumDofs(), "DOF index out of bounds", false);
#if DART_VERSION_AT_LEAST(6, 10, 0)
        return _skeleton->getDof(dof)->getJoint()->areLimitsEnforced();
#else
        return _skeleton->getDof(dof)->getJoint()->isPositionLimitEnforced();
#endif
    }

    std::vector<bool> Robot::position_enforced() const
    {
        std::vector<bool> pos;
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
#if DART_VERSION_AT_LEAST(6, 10, 0)
            pos.push_back(_skeleton->getDof(i)->getJoint()->areLimitsEnforced());
#else
            pos.push_back(_skeleton->getDof(i)->getJoint()->isPositionLimitEnforced());
#endif
        }

        return pos;
    }

    void Robot::set_damping_coeff(size_t dof, double damp)
    {
        ROBOT_DART_ASSERT(dof < _skeleton->getNumDofs(), "DOF index out of bounds", );
        _skeleton->getDof(dof)->setDampingCoefficient(damp);
    }

    void Robot::set_damping_coeffs(const std::vector<double>& damps)
    {
        ROBOT_DART_ASSERT(damps.size() == _skeleton->getNumDofs(), "Damping coefficient vector size is not the same as the DOFs of the robot", );
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            _skeleton->getDof(i)->setDampingCoefficient(damps[i]);
        }
    }

    void Robot::set_damping_coeffs(double damp)
    {
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            _skeleton->getDof(i)->setDampingCoefficient(damp);
        }
    }

    double Robot::damping_coeff(size_t dof) const
    {
        ROBOT_DART_ASSERT(dof < _skeleton->getNumDofs(), "DOF index out of bounds", 0.);
        return _skeleton->getDof(dof)->getDampingCoefficient();
    }

    std::vector<double> Robot::damping_coeffs() const
    {
        std::vector<double> damps;
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            damps.push_back(_skeleton->getDof(i)->getDampingCoefficient());
        }

        return damps;
    }

    void Robot::set_cfriction_coeff(size_t dof, double cfriction)
    {
        ROBOT_DART_ASSERT(dof < _skeleton->getNumDofs(), "DOF index out of bounds", );
        _skeleton->getDof(dof)->setCoulombFriction(cfriction);
    }

    void Robot::set_cfriction_coeffs(const std::vector<double>& cfrictions)
    {
        ROBOT_DART_ASSERT(cfrictions.size() == _skeleton->getNumDofs(), "Damping coefficient vector size is not the same as the DOFs of the robot", );
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            _skeleton->getDof(i)->setCoulombFriction(cfrictions[i]);
        }
    }

    void Robot::set_cfriction_coeffs(double cfriction)
    {
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            _skeleton->getDof(i)->setCoulombFriction(cfriction);
        }
    }

    double Robot::cfriction_coeff(size_t dof) const
    {
        ROBOT_DART_ASSERT(dof < _skeleton->getNumDofs(), "DOF index out of bounds", 0.);
        return _skeleton->getDof(dof)->getCoulombFriction();
    }

    std::vector<double> Robot::cfriction_coeffs() const
    {
        std::vector<double> cfrictions;
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            cfrictions.push_back(_skeleton->getDof(i)->getCoulombFriction());
        }

        return cfrictions;
    }

    Eigen::Isometry3d Robot::base_pose() const
    {
        auto jt = _skeleton->getRootBodyNode()->getParentJoint();
        ROBOT_DART_ASSERT(jt != nullptr, "Skeleton does not have a proper root BodyNode!", Eigen::Isometry3d::Identity());
        return jt->getTransformFromParentBodyNode();
    }

    void Robot::set_base_pose(const Eigen::Isometry3d& tf)
    {
        auto jt = _skeleton->getRootBodyNode()->getParentJoint();
        if (jt)
            jt->setTransformFromParentBodyNode(tf);
    }

    size_t Robot::num_dofs() const
    {
        return _skeleton->getNumDofs();
    }

    size_t Robot::num_joints() const
    {
        return _skeleton->getNumJoints();
    }

    size_t Robot::num_bodies() const
    {
        return _skeleton->getNumBodyNodes();
    }

    Eigen::Vector3d Robot::com() const
    {
        return _skeleton->getCOM();
    }

    Eigen::Vector6d Robot::com_velocity() const
    {
        return _skeleton->getCOMSpatialVelocity();
    }

    Eigen::Vector6d Robot::com_acceleration() const
    {
        return _skeleton->getCOMSpatialAcceleration();
    }

    Eigen::VectorXd Robot::_get_dof_data(int content, const std::vector<std::string>& dof_names) 
    {
        Eigen::VectorXd data;
        if(!dof_names.empty()){
            data.resize(dof_names.size());
            for(size_t i =0; dof_names.size(); i++) {
                ROBOT_DART_ASSERT(_full_dof_map.find(dof_names[i])!=_full_dof_map.end(), "_get_dof_data : " + dof_names[i] + " is not in _full_dof_map", {} );
                if(content==0){
                    data(i) = _skeleton->getDof(_full_dof_map[dof_names[i]])->getPosition();
                } else if(content==1){
                    data(i) = _skeleton->getDof(_full_dof_map[dof_names[i]])->getVelocity();
                } else if(content==2){
                    data(i) = _skeleton->getDof(_full_dof_map[dof_names[i]])->getAcceleration();
                } else if(content==3){
                    data(i) = _skeleton->getDof(_full_dof_map[dof_names[i]])->getForce();
                } 
            }   
        }
        else{
            if(content==0){
                data = _skeleton->getPositions();
            } else if(content==1){
                data = _skeleton->getVelocities();
            } else if(content==2){
                data = _skeleton->getAccelerations();
            } else if(content==3){
                data = _skeleton->getForces();
            } 
        }
        return data;
    }

     void Robot::_set_dof_data(int content,const Eigen::VectorXd& data, const std::vector<std::string>& dof_names) 
    {
        Eigen::VectorXd ordered_data;
        if(!dof_names.empty()){
            ROBOT_DART_ASSERT((size_t)data.size()==dof_names.size(), "_set_dof_data : data should have the same size and order than dof_names",);
            ordered_data =  Robot::_get_dof_data(content);
            for(size_t i = 0; i < dof_names.size(); i++) {
                ROBOT_DART_ASSERT(_full_dof_map.find(dof_names[i])!=_full_dof_map.end(), "_set_dof_data : " + dof_names[i] + " is not in _full_dof_map",);
                auto joint = _skeleton->getDof(_full_dof_map[dof_names[i]])->getJoint();
                ROBOT_DART_ASSERT(joint->getActuatorType() != dart::dynamics::Joint::MIMIC, "_set_dof_data : " + joint->getName() + " is a MIMIC joint",);
                ordered_data(_controllable_dof[dof_names[i]]) = data(i);
            }
        }
        else{
            ordered_data = data;
        }
        if(content==0){
            _skeleton->setPositions(ordered_data);
        } else if(content==1){
            _skeleton->setPositions(ordered_data);
        } else if(content==2){
            _skeleton->setPositions(ordered_data);
        } else if(content==3){
            _skeleton->setPositions(ordered_data);
        } 
    }

    Eigen::VectorXd Robot::positions(const std::vector<std::string>& dof_names) 
    {
        return _get_dof_data(0,dof_names);
    }

    void Robot::set_positions(const Eigen::VectorXd& positions, const std::vector<std::string>& dof_names)
    {
        _set_dof_data(0, positions, dof_names);
    }

    Eigen::VectorXd Robot::velocities(const std::vector<std::string>& dof_names)
    {
        return _get_dof_data(1, dof_names);
    }

    void Robot::set_velocities(const Eigen::VectorXd& velocities, const std::vector<std::string>& dof_names)
    {
        _set_dof_data(1, velocities, dof_names);
    }

    Eigen::VectorXd Robot::accelerations(const std::vector<std::string>& dof_names)
    {
        return _get_dof_data(2, dof_names);
    }

    void Robot::set_accelerations(const Eigen::VectorXd& accelerations, const std::vector<std::string>& dof_names)
    {
        _set_dof_data(2, accelerations, dof_names);
    }

    Eigen::VectorXd Robot::forces(const std::vector<std::string>& dof_names)
    {
        return _get_dof_data(3,dof_names);
    }

    void Robot::set_forces(const Eigen::VectorXd& forces, const std::vector<std::string>& dof_names)
    {
        _set_dof_data(3, forces, dof_names); 
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

        // F1 contains the force applied by the parent Link on the Joint specified in the parent Link frame
        // F2 contains the force applied by the child Link on the Joint specified in the child Link frame
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

    void Robot::clear_external_forces()
    {
        _skeleton->clearExternalForces();
    }

    Eigen::Vector6d Robot::external_forces(const std::string& body_name) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", Eigen::Vector6d::Zero());

        return bd->getExternalForceGlobal();
    }

    Eigen::Vector6d Robot::external_forces(size_t body_index) const
    {
        ROBOT_DART_ASSERT(body_index < _skeleton->getNumBodyNodes(), "BodyNode index out of bounds", Eigen::Vector6d::Zero());
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

    std::vector<std::string> Robot::dof_names() const
    {
        std::vector<std::string> names;
        for (auto& dof : _skeleton->getDofs())
            names.push_back(dof->getName());
        return names;
    }
    
    void  Robot::update_dof_map()
    {
        _controllable_dof.clear();
        _mimic_dof_map.clear();
        _full_dof_map.clear();
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            auto joint = _skeleton->getDof(i)->getJoint();
            if (joint->getActuatorType() != dart::dynamics::Joint::MIMIC){
                _controllable_dof[_skeleton->getDof(i)->getName()] = i;
            }
            else{
                _mimic_dof_map[_skeleton->getDof(i)->getName()] = i;
            }
            _full_dof_map[_skeleton->getDof(i)->getName()] = i;
        }
    }

    std::string Robot::dof_name(size_t dof_index) const
    {
        ROBOT_DART_ASSERT(dof_index < _skeleton->getNumDofs(), "Dof index out of bounds", "");
        return _skeleton->getDof(dof_index)->getName();
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
    }

    void Robot::set_color_mode(dart::dynamics::MeshShape::ColorMode color_mode)
    {
        _set_color_mode(color_mode, _skeleton);
    }

    void Robot::set_color_mode(dart::dynamics::MeshShape::ColorMode color_mode, const std::string& body_name)
    {
        auto bn = _skeleton->getBodyNode(body_name);
        if (bn) {
            for (size_t j = 0; j < bn->getNumShapeNodes(); ++j) {
                dart::dynamics::ShapeNode* sn = bn->getShapeNode(j);
                _set_color_mode(color_mode, sn);
            }
        }
    }

    dart::dynamics::SkeletonPtr Robot::_load_model(const std::string& filename, const std::vector<std::pair<std::string, std::string>>& packages, bool is_urdf_string)
    {
        // Remove spaces from beginning of the filename/path
        std::string model_file = filename;
        model_file.erase(model_file.begin(), std::find_if(model_file.begin(), model_file.end(), [](int ch) {
            return !std::isspace(ch);
        }));

        if (model_file[0] != '/') {
            constexpr size_t max_size = 512;
            char buff[max_size];
            auto val = getcwd(buff, max_size);
            ROBOT_DART_ASSERT(val, "Something bad happenned when trying to read current path", nullptr);
            model_file = std::string(buff) + "/" + model_file;
        }

        dart::dynamics::SkeletonPtr tmp_skel;
        if (!is_urdf_string) {
            std::string extension = model_file.substr(model_file.find_last_of(".") + 1);
            if (extension == "urdf") {
                dart::io::DartLoader loader;
                for (size_t i = 0; i < packages.size(); i++) {
                    loader.addPackageDirectory(std::get<0>(packages[i]), std::get<1>(packages[i]));
                }
                tmp_skel = loader.parseSkeleton(model_file);
            }
            else if (extension == "sdf")
                tmp_skel = dart::io::SdfParser::readSkeleton(model_file);
            else if (extension == "skel") {
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
                loader.addPackageDirectory(std::get<0>(packages[i]), std::get<1>(packages[i]));
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
            dart::dynamics::MeshShape* ms = dynamic_cast<dart::dynamics::MeshShape*>(sn->getShape().get());
            if (ms)
                ms->setColorMode(color_mode);
        }
    }

    std::shared_ptr<Robot> Robot::create_box(const Eigen::Vector3d& dims, const Eigen::Vector6d& pose, const std::string& type, double mass, const Eigen::Vector4d& color, const std::string& box_name)
    {
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
        auto box_node = body->createShapeNodeWith<dart::dynamics::VisualAspect, dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(box);
        box_node->getVisualAspect()->setColor(color);
        // Set up inertia
        dart::dynamics::Inertia inertia;
        inertia.setMass(mass);
        inertia.setMoment(box->computeInertia(mass));
        body->setInertia(inertia);

        // Put the body into position
        if (type == "free") // free floating
            box_skel->setPositions(pose);
        else // fixed
        {
            Eigen::Isometry3d T;
            T.linear() = dart::math::eulerXYZToMatrix(pose.head(3));
            T.translation() = pose.tail(3);
            body->getParentJoint()->setTransformFromParentBodyNode(T);
        }

        return std::make_shared<Robot>(box_skel, box_name);
    }

    std::shared_ptr<Robot> Robot::create_ellipsoid(const Eigen::Vector3d& dims, const Eigen::Vector6d& pose, const std::string& type, double mass, const Eigen::Vector4d& color, const std::string& ellipsoid_name)
    {
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
        auto ellipsoid_node = body->createShapeNodeWith<dart::dynamics::VisualAspect, dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(ellipsoid);
        ellipsoid_node->getVisualAspect()->setColor(color);
        // Set up inertia
        dart::dynamics::Inertia inertia;
        inertia.setMass(mass);
        inertia.setMoment(ellipsoid->computeInertia(mass));
        body->setInertia(inertia);

        // Put the body into position
        if (type == "free") // free floating
            ellipsoid_skel->setPositions(pose);
        else // fixed
        {
            Eigen::Isometry3d T;
            T.linear() = dart::math::eulerXYZToMatrix(pose.head(3));
            T.translation() = pose.tail(3);
            body->getParentJoint()->setTransformFromParentBodyNode(T);
        }

        return std::make_shared<Robot>(ellipsoid_skel, ellipsoid_name);
    }
} // namespace robot_dart
