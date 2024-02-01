

# File base.hpp

[**File List**](files.md) **>** [**gui**](dir_6a9d4b7ec29c938d1d9a486c655cfc8a.md) **>** [**base.hpp**](base_8hpp.md)

[Go to the documentation of this file](base_8hpp.md)

```C++

#ifndef ROBOT_DART_GUI_BASE_HPP
#define ROBOT_DART_GUI_BASE_HPP

#include <robot_dart/gui/helper.hpp>

namespace robot_dart {
    class RobotDARTSimu;

    namespace gui {
        class Base {
        public:
            Base() {}

            virtual ~Base() {}

            virtual void set_simu(RobotDARTSimu* simu) { _simu = simu; }
            const RobotDARTSimu* simu() const { return _simu; }

            virtual bool done() const { return false; }

            virtual void refresh() {}

            virtual void set_render_period(double) {}

            virtual void set_enable(bool) {}
            virtual void set_fps(int) {}

            virtual Image image() { return Image(); }
            virtual GrayscaleImage depth_image() { return GrayscaleImage(); }
            virtual GrayscaleImage raw_depth_image() { return GrayscaleImage(); }
            virtual DepthImage depth_array() { return DepthImage(); }

            virtual size_t width() const { return 0; }
            virtual size_t height() const { return 0; }

        protected:
            RobotDARTSimu* _simu = nullptr;
        };
    } // namespace gui
} // namespace robot_dart

#endif

```

