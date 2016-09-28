#ifndef ROBOT_DART_VISUALIZATIONS_HPP
#define ROBOT_DART_VISUALIZATIONS_HPP

#include <Eigen/Core>

namespace robot_dart {
    namespace visualizations {

        template <typename Params>
        struct PointingArrow {
        public:
            PointingArrow() : init(false) {}

            template <typename Simu, typename robot>
            void operator()(Simu& simu, std::shared_ptr<robot> rob)
            {
                if (!init) {
                    dart::dynamics::ArrowShape::Properties arrow_properties;
                    arrow_properties.mRadius = Params::radius();
                    _arrow = std::shared_ptr<dart::dynamics::ArrowShape>(new dart::dynamics::ArrowShape(
                        Params::head(),
                        Params::tail(),
                        arrow_properties, Params::color()));
                    auto bn = simu.world()->getSkeleton(Params::skel_name())->getBodyNode(Params::body_name());
                    bn->template createShapeNodeWith<dart::dynamics::VisualAspect>(_arrow);
                    init = true;
                }

                _arrow->setPositions(Params::head(), Params::tail());
            }

        protected:
            std::shared_ptr<dart::dynamics::ArrowShape> _arrow;
            bool init;
        };
    }
}

#endif
