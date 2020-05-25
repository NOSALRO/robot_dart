#ifndef ROBOT_DART_SIMU_HPP
#define ROBOT_DART_SIMU_HPP

#include <dart/simulation/World.hpp>

#include <robot_dart/descriptor/base_descriptor.hpp>
#include <robot_dart/gui/base.hpp>
#include <robot_dart/robot.hpp>

namespace robot_dart {
    namespace simu {
        struct GUIData;
    }

    class RobotDARTSimu {
    public:
        using robot_t = std::shared_ptr<Robot>;

        RobotDARTSimu(double time_step = 0.015);

        ~RobotDARTSimu();

        void run(double max_duration = 5.0);
        void step_once();

        std::shared_ptr<gui::Base> graphics() const;
        void set_graphics(const std::shared_ptr<gui::Base>& graphics);

        dart::simulation::WorldPtr world();

        template <typename Descriptor>
        void add_descriptor(size_t desc_dump = 1)
        {
            add_descriptor(std::make_shared<Descriptor>(Descriptor{this, desc_dump}));
        }

        void add_descriptor(const std::shared_ptr<descriptor::BaseDescriptor>& desc);
        std::vector<std::shared_ptr<descriptor::BaseDescriptor>> descriptors() const;
        std::shared_ptr<descriptor::BaseDescriptor> descriptor(size_t index) const;

        void remove_descriptor(const std::shared_ptr<descriptor::BaseDescriptor>& desc);
        void remove_descriptor(size_t index);
        void clear_descriptors();

        void add_camera(const std::shared_ptr<gui::Base>& cam);
        std::vector<std::shared_ptr<gui::Base>> cameras() const;
        std::shared_ptr<gui::Base> camera(size_t index) const;

        void remove_camera(const std::shared_ptr<gui::Base>& desc);
        void remove_camera(size_t index);
        void clear_cameras();

        double step() const;
        void set_step(double step);

        void stop_sim(bool disable = true);
        bool halted_sim() const;

        size_t num_robots() const;
        const std::vector<robot_t>& robots() const;
        robot_t robot(size_t index) const;
        int robot_index(const robot_t& robot) const;

        void add_robot(const robot_t& robot);
        void add_visual_robot(const robot_t& robot);
        void remove_robot(const robot_t& robot);
        void remove_robot(size_t index);
        void clear_robots();

        simu::GUIData* gui_data();

        void add_floor(double floor_width = 10.0, double floor_height = 0.1, const Eigen::Vector6d& pose = Eigen::Vector6d::Zero(), const std::string& floor_name = "floor");
        void add_checkerboard_floor(double floor_width = 10.0, double floor_height = 0.1, double size = 1., const Eigen::Vector6d& pose = Eigen::Vector6d::Zero(), const std::string& floor_name = "checkerboard_floor");

        void set_collision_detector(const std::string& collision_detector); // collision_detector can be "DART", "FCL", "Ode" or "Bullet" (case does not matter)
        const std::string& collision_detector() const;

        // Bitmask collision filtering
        void set_collision_mask(size_t robot_index, uint16_t mask);
        void set_collision_mask(size_t robot_index, const std::string& body_name, uint16_t mask);
        void set_collision_mask(size_t robot_index, size_t body_index, uint16_t mask);

        uint16_t collision_mask(size_t robot_index, const std::string& body_name);
        uint16_t collision_mask(size_t robot_index, size_t body_index);

        void remove_collision_mask(size_t robot_index);
        void remove_collision_mask(size_t robot_index, const std::string& body_name);
        void remove_collision_mask(size_t robot_index, size_t body_index);

        void remove_all_collision_masks();

    protected:
        dart::simulation::WorldPtr _world;
        size_t _old_index;
        bool _break;

        std::vector<std::shared_ptr<descriptor::BaseDescriptor>> _descriptors;
        std::vector<std::shared_ptr<gui::Base>> _cameras; // designed to include mainly graphcis::CameraOSR
        std::vector<robot_t> _robots;
        std::shared_ptr<gui::Base> _graphics;
        std::unique_ptr<simu::GUIData> _gui_data;
    };
} // namespace robot_dart

#endif
