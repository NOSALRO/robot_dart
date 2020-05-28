#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#include <dart/collision/fcl/FCLCollisionDetector.hpp>
#include <dart/constraint/ConstraintSolver.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    std::srand(std::time(NULL));

    std::vector<std::pair<std::string, std::string>> packages = {{"talos_description", "/home/user/rf_ws/src/talos_robot/talos_description"}};
    auto global_robot = std::make_shared<robot_dart::Robot>("res/models/talos.urdf", packages);

    global_robot->set_position_enforced(true);
    global_robot->skeleton()->setPosition(5, 1.2);
    global_robot->skeleton()->setPosition(2, 1.57);

    // Set actuator types to VELOCITY motors so that they stay in position without any controller
    global_robot->set_actuator_types(dart::dynamics::Joint::VELOCITY);
    // First 6-DOFs should always be FORCE if robot is floating base
    for (size_t i = 0; i < 6; i++)
        global_robot->set_actuator_type(i, dart::dynamics::Joint::FORCE);
    robot_dart::RobotDARTSimu simu(0.001);
    simu.world()->getConstraintSolver()->setCollisionDetector(dart::collision::FCLCollisionDetector::create());
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics<>>(&simu);
    simu.set_graphics(graphics);
    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
#endif
    simu.add_checkerboard_floor();
    simu.add_robot(global_robot);
    // simu.run(20.);
    Eigen::VectorXd q0(36);
    //dart is inverting base pos and rotation in comparison with opensot
    q0 << 0.0, 0.0, 1.57, 0.0, 0.0, 1.06, //floating_base
        0.0, 0.0, -0.26, 0.56, -0.33, 0.0, //left_leg
        0.0, 0.0, -0.26, 0.56, -0.33, 0.0, // right_leg
        0.0, 0.05, // torso
        0.4, 0.24, -0.6, -1.45, 0.0, 0.0, 0.0, //arm left
        -0.4, -0.24, 0.6, -1.45, 0.0, 0.0, 0.0, //arm right
        0.0, 0.0; // head

    auto dofs = global_robot->dof_names();
    for (auto& d : dofs) {
        std::cout << d << std::endl;
    }

    global_robot->set_positions(q0);

    simu.run(5.);
    std::vector<std::string> dof_to_control;
    dof_to_control.push_back("arm_left_4_joint");
    dof_to_control.push_back("torso_2_joint");
    Eigen::VectorXd cmd(2);
    cmd(0) = 0.1;
    cmd(1) = 0.1;

    global_robot->set_actuator_type(global_robot->dof_index("arm_left_5_joint"), dart::dynamics::detail::ActuatorType::MIMIC, true);
    global_robot->update_joint_dof_maps();

    Eigen::VectorXd cmd_full = Eigen::VectorXd::Zero(q0.size());
    cmd_full(global_robot->dof_index("arm_left_4_joint")) = 1;
    cmd_full(global_robot->dof_index("arm_left_5_joint")) = -1;
    for (int i = 0; i < 5000; i++) {
        global_robot->set_commands(cmd, dof_to_control);
        // global_robot->set_commands(cmd_full, {});
        simu.step_once();
    }

    global_robot.reset();
    return 0;
}
