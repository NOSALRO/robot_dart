#include <robot_dart/control/pd_control.hpp>

namespace robot_dart {
    namespace control {

        class ManifoldControl: public PDControl {
        public:

            ManifoldControl(const std::vector<int>& t_rotational_dofs );
            ManifoldControl(
                const std::vector<int>& t_rotational_dofs,
                const Eigen::VectorXd& ctrl, bool full_control = false);
            ManifoldControl(
                const std::vector<int>& t_rotational_dofs,
                const Eigen::VectorXd& ctrl,
                const std::vector<std::string>& controllable_dofs);

            Eigen::VectorXd calculate(double) override;

            double boxMinus(const double alfa,  const double beta);
        private:
            const std::vector<int> m_rotational_dofs;
        };
    }
}
