#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/iiwa.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    auto robot = std::make_shared<robot_dart::robots::Iiwa>();

    // Set actuator types to SERVO motors so that they stay in position without any controller
    robot->set_actuator_types("servo");

    float dt = 0.001;
    robot_dart::RobotDARTSimu simu(dt);
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);
    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
#endif

    /**************** Use dofs names to control the robot *********************************************/

    //Set Initial Position for the robot
    Eigen::VectorXd q0 = robot_dart::make_vector({0.0, M_PI / 3., 0.0, -M_PI / 4.0, 0.0, 0.0, 0.0});
    robot->set_positions(q0);

    //Get dofs names
    auto dofs = robot->dof_names();
    std::cout << "Dofs : " << std::endl;
    for (auto& d : dofs) {
        std::cout << d << std::endl;
    }

    simu.add_checkerboard_floor();
    simu.add_robot(robot);

    //Create a custom update loop using step_world instead of run
    int sim_time = 10 / dt;

    //Create a vector of dofs to control
    std::vector<std::string> dof_to_control;
    dof_to_control.push_back("iiwa_joint_1");
    dof_to_control.push_back("iiwa_joint_4");
    Eigen::VectorXd cmd = robot_dart::make_vector({0.1, -0.1}),
                    pos(2),
                    full_pos(q0.size());

    for (int i = 0; i < sim_time; i++) {
        robot->set_commands(cmd, dof_to_control);
        pos = robot->positions(dof_to_control); //get dof_to_control positions only
        full_pos = robot->positions(); //get all dofs positions
        if (simu.step_world()) // do not update controllers (it will override set_commands)
            break;
    }

    // If you wanted to control every dofs you could have used :
    // Eigen::VectorXd cmd_full = Eigen::VectorXd::Constant(q0.size(), 0.1);
    // for (int i = 0; i < sim_time; i++) {
    //     robot->set_commands(cmd_full);
    //     simu.step_world(); // do not update controllers (it will override set_commands)
    // }

    /**************** Mimic / Passive / Locked joint handling ********************************************************/
    bool filter_mimics = true, filter_locked = true, filter_passive = true;

    robot->set_mimic("iiwa_joint_5", "iiwa_joint_1");
    robot->set_actuator_type("locked", "iiwa_joint_6");
    robot->set_actuator_type("passive", "iiwa_joint_7");

    //Get the controllable joints (You cannot send a command to a mimic or passive or locked joint)
    auto controllable_dofs = robot->dof_names(filter_mimics, filter_locked, filter_passive);

    std::cout << "Filtered dofs : " << std::endl;
    for (auto& cd : controllable_dofs) {
        std::cout << cd << std::endl;
    }

    cmd.resize(controllable_dofs.size());
    cmd.setConstant(-0.1);

    for (int i = 0; i < sim_time; i++) {
        robot->set_commands(cmd, controllable_dofs);
        // robot->set_commands(Eigen::VectorXd::Constant(q0.size(), -0.1)); // This also works, but you will be getting a lot of warnings from DART; the comands to the mimic/passive/locked joints are ignored!
        if (simu.step_world()) // do not update controllers (it will override set_commands)
            break;
    }

    robot.reset();
    return 0;
}