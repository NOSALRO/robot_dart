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
    }

    Robot::Robot(const std::string& model_file, const std::string& robot_name, bool is_urdf_string, std::vector<RobotDamage> damages) : Robot(model_file, std::vector<std::pair<std::string, std::string>>(), robot_name, is_urdf_string, damages) {}

    Robot::Robot(dart::dynamics::SkeletonPtr skeleton, const std::string& robot_name, std::vector<RobotDamage> damages) : _robot_name(robot_name), _skeleton(skeleton)
    {
        ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(_skeleton != nullptr);
        _skeleton->setName(robot_name);
        _set_damages(damages);
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
        if (fixed())
            return;
        Eigen::Isometry3d tf(dart::math::expAngular(_skeleton->getPositions().head(3)));
        tf.translation() = _skeleton->getPositions().segment(3, 3);
        _skeleton->getRootBodyNode()->changeParentJointType<dart::dynamics::WeldJoint>();
        _skeleton->getRootBodyNode()->getParentJoint()->setTransformFromParentBodyNode(tf);

        reinit_controllers();
    }

    // pose: Orientation-Position
    void Robot::free_from_world(const Eigen::Vector6d& pose)
    {
        if (free())
            return;
        Eigen::Isometry3d tf(dart::math::expAngular(pose.head(3)));
        tf.translation() = pose.segment(3, 3);
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

    void Robot::set_actuator_type(size_t dof, dart::dynamics::Joint::ActuatorType type)
    {
        ROBOT_DART_ASSERT(dof < _skeleton->getNumDofs(), "DOF index out of bounds", );
        _skeleton->getDof(dof)->getJoint()->setActuatorType(type);
    }

    void Robot::set_actuator_types(const std::vector<dart::dynamics::Joint::ActuatorType>& types)
    {
        ROBOT_DART_ASSERT(types.size() == _skeleton->getNumDofs(), "Actuator types vector size is not the same as the DOFs of the robot", );
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            _skeleton->getDof(i)->getJoint()->setActuatorType(types[i]);
        }
    }

    void Robot::set_actuator_types(dart::dynamics::Joint::ActuatorType type)
    {
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
            _skeleton->getDof(i)->getJoint()->setActuatorType(type);
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
#if DART_VERSION_AT_LEAST(6, 1, 0)
        _skeleton->getDof(dof)->getJoint()->setLimitEnforcement(enforced);
#else
        _skeleton->getDof(dof)->getJoint()->setPositionLimitEnforced(enforced);
#endif
    }

    void Robot::set_position_enforced(const std::vector<bool>& enforced)
    {
        ROBOT_DART_ASSERT(enforced.size() == _skeleton->getNumDofs(), "Position enforced vector size is not the same as the DOFs of the robot", );
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
#if DART_VERSION_AT_LEAST(6, 1, 0)
            _skeleton->getDof(i)->getJoint()->setLimitEnforcement(enforced[i]);
#else
            _skeleton->getDof(i)->getJoint()->setPositionLimitEnforced(enforced[i]);
#endif
        }
    }

    void Robot::set_position_enforced(bool enforced)
    {
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
#if DART_VERSION_AT_LEAST(6, 1, 0)
            _skeleton->getDof(i)->getJoint()->setLimitEnforcement(enforced);
#else
            _skeleton->getDof(i)->getJoint()->setPositionLimitEnforced(enforced);
#endif
        }
    }

    bool Robot::position_enforced(size_t dof) const
    {
        ROBOT_DART_ASSERT(dof < _skeleton->getNumDofs(), "DOF index out of bounds", false);
#if DART_VERSION_AT_LEAST(6, 1, 0)
        return _skeleton->getDof(dof)->getJoint()->areLimitsEnforced();
#else
        return _skeleton->getDof(dof)->getJoint()->isPositionLimitEnforced();
#endif
    }

    std::vector<bool> Robot::position_enforced() const
    {
        std::vector<bool> pos;
        for (size_t i = 0; i < _skeleton->getNumDofs(); ++i) {
#if DART_VERSION_AT_LEAST(6, 1, 0)
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

    void Robot::set_base_pose(const Eigen::Isometry3d& tf)
    {
        auto jt = _skeleton->getRootBodyNode()->getParentJoint();
        if (jt)
            jt->setTransformFromParentBodyNode(tf);
    }

    Eigen::Vector3d Robot::body_pos(const std::string& body_name) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        if (bd)
            return bd->getTransform().translation();

        return Eigen::Vector3d::Zero();
    }

    Eigen::Matrix3d Robot::body_rot(const std::string& body_name) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        if (bd)
            return bd->getTransform().linear();
        return Eigen::Matrix3d::Identity();
    }

    Eigen::Isometry3d Robot::body_trans(const std::string& body_name) const
    {
        auto bd = _skeleton->getBodyNode(body_name);
        if (bd)
            return bd->getTransform();
        return Eigen::Isometry3d::Identity();
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
#if DART_VERSION_AT_LEAST(6, 1, 0)
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
