#include <robot_dart/control/manifold_control.hpp>
#include "robot_dart/robot.hpp"
#include "robot_dart/utils.hpp"

namespace robot_dart {
    namespace control {
   
        ManifoldControl::ManifoldControl(const std::vector<int>& t_rotational_dofs ):
          m_rotational_dofs(t_rotational_dofs), PDControl(){}


        ManifoldControl::ManifoldControl(
            const std::vector<int>& t_rotational_dofs,
            const Eigen::VectorXd& ctrl, bool full_control):
          m_rotational_dofs(t_rotational_dofs), PDControl(ctrl, full_control){}


        ManifoldControl::ManifoldControl(
            const std::vector<int>& t_rotational_dofs,
            const Eigen::VectorXd& ctrl,
            const std::vector<std::string>& controllable_dofs):
          m_rotational_dofs(t_rotational_dofs), PDControl(ctrl,controllable_dofs ){}


        Eigen::VectorXd ManifoldControl::calculate(double)
        {
            ROBOT_DART_ASSERT(_control_dof == _ctrl.size(), "ManifoldControl: Controller parameters size is not the same as DOFs of the robot", Eigen::VectorXd::Zero(_control_dof));
            auto robot = _robot.lock();

            Eigen::VectorXd& target_positions = _ctrl;

            Eigen::VectorXd q = robot->positions(_controllable_dofs);
            Eigen::VectorXd dq = robot->velocities(_controllable_dofs);


            /* Assuming all joints to be rotational e.g. franka robot
             * error computed on the manifold
             */

            Eigen::VectorXd error(_ctrl.size() );
            for(int i=0; i<_ctrl.size(); ++i){
              if( m_rotational_dofs[i]==1){
                error(i) = boxMinus( target_positions(i),  q(i) );
              }
              else{
                error(i) =  target_positions(i) - q(i);
              }
            }

            Eigen::VectorXd commands = _Kp.array() * error.array() - _Kd.array() * dq.array();

            return commands;
        }

        double ManifoldControl::boxMinus(const double alfa,  const double beta)
        {
          Eigen::Matrix2d R_alfa;
          Eigen::Matrix2d R_beta;
          R_alfa << cos( alfa ), -sin( alfa),
            sin(alfa), cos(alfa);
          R_beta << cos( beta), -sin(beta),
            sin(beta), cos(beta);
          Eigen::Matrix2d R = R_beta.transpose() * R_alfa;
          return atan2( R(1,0), R(0,0));
        }
    }
}
