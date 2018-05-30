#ifndef ROBOT_DART_CONTROL_HEXA_CONTROL
#define ROBOT_DART_CONTROL_HEXA_CONTROL

#include <hexapod_controller/hexapod_controller_simple.hpp>

#include <robot_dart/control/policy_control.hpp>

namespace robot_dart {
    namespace control {

        struct HexaPolicy {
        public:
            void set_params(const std::vector<double>& ctrl)
            {
                _controller.set_parameters(ctrl);
            }

            size_t output_size() const { return 18; }

            Eigen::VectorXd query(const std::shared_ptr<robot_dart::Robot>& robot, double t)
            {
                if (!_h_params_set) {
                    _dt = robot->skeleton()->getTimeStep();
                }
                auto angles = _controller.pos(t);

                Eigen::VectorXd target_positions = Eigen::VectorXd::Zero(18 + 6);
                for (size_t i = 0; i < angles.size(); i++)
                    target_positions(i + 6) = ((i % 3 == 1) ? 1.0 : -1.0) * angles[i];

                Eigen::VectorXd q = robot->skeleton()->getPositions();
                Eigen::VectorXd q_err = target_positions - q;

                double gain = 1.0 / (dart::math::constants<double>::pi() * _dt);
                Eigen::VectorXd vel = q_err * gain;

                return vel.tail(18);
            }

            void set_h_params(const std::vector<double>& h_params)
            {
                _dt = h_params[0];
                _h_params_set = true;
            }

            std::vector<double> h_params() const
            {
                return std::vector<double>(1, _dt);
            }

        protected:
            hexapod_controller::HexapodControllerSimple _controller;
            double _dt;
            bool _h_params_set = false;
        };

        using HexaControl = robot_dart::control::PolicyControl<HexaPolicy>;
    } // namespace control
} // namespace robot_dart

#endif
