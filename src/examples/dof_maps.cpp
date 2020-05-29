#include <algorithm>
#include <cstdlib>
#include <iostream>

#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    std::srand(std::time(NULL));

    std::vector<std::pair<std::string, std::string>> packages = {{"iiwa14", std::string(RESPATH) + "/models/meshes"}};
    auto global_robot = std::make_shared<robot_dart::Robot>("res/models/iiwa14.urdf", packages);

    global_robot->fix_to_world();
    global_robot->set_position_enforced(true);
    
    // Set actuator types to VELOCITY motors so that they stay in position without any controller
    global_robot->set_actuator_types(dart::dynamics::Joint::VELOCITY);

    float dt = 0.001;
    robot_dart::RobotDARTSimu simu(dt);
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics<>>(&simu);
    simu.set_graphics(graphics);
    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
#endif

    /**************** Use dofs names to control the robot *********************************************/

    //Set Initial Position for the robot
    Eigen::VectorXd q0(7);
    q0 << 0.0, M_PI / 3., 0.0, -M_PI / 4.0, 0.0, 0.0, 0.0;
    global_robot->set_positions(q0);


    //Get dofs names
    auto dofs = global_robot->dof_names();
    std::cout << "Dofs : " << std::endl;
    for (auto& d : dofs) {
        std::cout << d << std::endl;
    }

    simu.add_checkerboard_floor();
    simu.add_robot(global_robot);

    //Create a custom update loop using step_world instead of run
    int sim_time = 10/dt;

    //Create a vector of dofs to control
    std::vector<std::string> dof_to_control;
    dof_to_control.push_back("iiwa_joint_1");
    dof_to_control.push_back("iiwa_joint_4");
    Eigen::VectorXd cmd(2), pos(2), full_pos(q0.size());
    cmd(0) = 0.1;
    cmd(1) = -0.1;

    for (int i = 0; i <sim_time; i++) {
        global_robot->set_commands(cmd, dof_to_control);
        pos = global_robot->positions(dof_to_control);//get dof_to_control positions only
        full_pos = global_robot->positions();//get all dofs positions
        simu.step_world();
    }

    //If you wanted to control every dofs you could have used :
    // Eigen::VectorXd cmd_full = Eigen::VectorXd::Constant(q0.size(),0.1);
    // for (int i = 0; i <sim_time; i++) {
    //     global_robot->set_commands(cmd_full); 
    //     simu.step_world();
    // }

    /**************** Mimic / Passive / Locked joint handling ********************************************************/

    int mimic_index = global_robot->dof_index("iiwa_joint_5");   
    int locked_index = global_robot->dof_index("iiwa_joint_6");
    int passive_index = global_robot->dof_index("iiwa_joint_7");

    bool filter_mimics = true, filter_locked = true, filter_passive = true;

    global_robot->set_actuator_type(mimic_index, dart::dynamics::Joint::MIMIC, true);
    global_robot->set_actuator_type(locked_index, dart::dynamics::Joint::LOCKED, true);
    global_robot->set_actuator_type(passive_index, dart::dynamics::Joint::PASSIVE, true);

    //Get the controllable joints (You cannot send a command to a mimic or passive or locked joint)
    auto controllable_dofs = global_robot->dof_names(filter_mimics, filter_locked, filter_passive);

    std::cout << "Filtered dofs : " << std::endl;
    for (auto& cd : controllable_dofs) {
        std::cout << cd << std::endl;
    }
    cmd.resize(controllable_dofs.size());
    cmd.setConstant(-0.1);

    for (int i = 0; i <sim_time; i++) {
        global_robot->set_commands(cmd, controllable_dofs);
        // global_robot->set_commands(Eigen::VectorXd::Constant(q0.size(),0.1)); // You can try it won't work and display a warning
        simu.step_world();
    }

    global_robot.reset();
    return 0;
}