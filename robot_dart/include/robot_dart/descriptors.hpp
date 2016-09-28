#ifndef ROBOT_DART_DESCRIPTORS_HPP
#define ROBOT_DART_DESCRIPTORS_HPP

#include <algorithm>
#include <map>
#include <vector>
#include <numeric>

#include <Eigen/Core>

namespace robot_dart {

    namespace descriptors {

        struct DescriptorBase {
        public:
            template <typename Simu, typename robot>
            void operator()(Simu& simu, std::shared_ptr<robot> rob)
            {
                assert(false);
            }

            template <typename T>
            void get(T& results)
            {
                assert(false);
            }
        };

        // struct EndEffectorTraj : public DescriptorBase {
        // public:
        //     EndEffectorTraj() {}
        //
        //     template <typename Simu, typename robot>
        //     void operator()(Simu& simu, std::shared_ptr<robot> rob)
        //     {
        //         _eef_traj.push_back(rob->end_effector_pos());
        //     }
        //
        //     void get(std::vector<Eigen::Vector3d>& results)
        //     {
        //         results = _eef_traj;
        //     }
        //
        // protected:
        //     std::vector<Eigen::Vector3d> _eef_traj;
        // };
    }
}

#endif
