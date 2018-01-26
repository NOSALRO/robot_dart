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
            void operator()(Simu& simu, std::vector<std::shared_ptr<robot>> rob)
            {
                assert(false);
            }

            template <typename T>
            void get(T& results)
            {
                assert(false);
            }
        };
    }
}

#endif
