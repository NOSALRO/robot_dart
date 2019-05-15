#ifndef ROBOT_DART_SIMU_HPP
#define ROBOT_DART_SIMU_HPP

#include <dart/simulation/World.hpp>

#include <robot_dart/descriptor/base_descriptor.hpp>
#include <robot_dart/graphics/base_graphics.hpp>
#include <robot_dart/robot.hpp>

namespace robot_dart {
    class RobotDARTSimu {
    public:
        using robot_t = std::shared_ptr<Robot>;

        RobotDARTSimu(double time_step = 0.015);

        ~RobotDARTSimu();

        void run(double max_duration = 5.0);

        std::shared_ptr<graphics::BaseGraphics> graphics() const;
        void set_graphics(const std::shared_ptr<graphics::BaseGraphics>& graphics);

        dart::simulation::WorldPtr world();

        template <typename Descriptor>
        void add_descriptor(size_t desc_dump = 1)
        {
            add_descriptor(std::make_shared<Descriptor>(Descriptor{*this, desc_dump}));
        }

        void add_descriptor(const std::shared_ptr<descriptor::BaseDescriptor>& desc);
        std::vector<std::shared_ptr<descriptor::BaseDescriptor>> descriptors() const;
        std::shared_ptr<descriptor::BaseDescriptor> descriptor(size_t index) const;

        void remove_descriptor(const std::shared_ptr<descriptor::BaseDescriptor>& desc);
        void remove_descriptor(size_t index);
        void clear_descriptors();

        void add_camera(const std::shared_ptr<graphics::BaseGraphics>& cam);
        std::vector<std::shared_ptr<graphics::BaseGraphics>> cameras() const;
        std::shared_ptr<graphics::BaseGraphics> camera(size_t index) const;

        void remove_camera(const std::shared_ptr<graphics::BaseGraphics>& desc);
        void remove_camera(size_t index);
        void clear_cameras();

        double step() const;
        void set_step(double step);

        void stop_sim(bool disable = true);
        bool halted_sim() const;

        size_t num_robots() const;
        std::vector<robot_t> robots() const;
        robot_t robot(size_t index) const;

        void add_robot(const robot_t& robot);
        void remove_robot(const robot_t& robot);
        void remove_robot(size_t index);
        void clear_robots();

        void add_floor(double floor_width = 10.0, double floor_height = 0.1, const Eigen::Vector6d& pose = Eigen::Vector6d::Zero(), const std::string& floor_name = "floor");

    protected:
        dart::simulation::WorldPtr _world;
        size_t _old_index;
        bool _break;
        std::vector<std::shared_ptr<descriptor::BaseDescriptor>> _descriptors;
        std::vector<std::shared_ptr<graphics::BaseGraphics>> _cameras; // designed to include mainly graphcis::CameraOSR
        std::vector<robot_t> _robots;
        std::shared_ptr<graphics::BaseGraphics> _graphics;
    };
} // namespace robot_dart

#endif
