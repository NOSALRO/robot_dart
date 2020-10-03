#ifndef ROBOT_DART_SIMU_HPP
#define ROBOT_DART_SIMU_HPP

#include <dart/simulation/World.hpp>

#include <robot_dart/descriptor/base_descriptor.hpp>
#include <robot_dart/gui/base.hpp>
#include <robot_dart/robot.hpp>
#include <robot_dart/scheduler.hpp>
#include <robot_dart/sensor/sensor.hpp>

namespace robot_dart {
    namespace simu {
        struct GUIData;

        // We use the Abode Source Sans Pro font: https://github.com/adobe-fonts/source-sans-pro
        // This font is licensed under SIL Open Font License 1.1: https://github.com/adobe-fonts/source-sans-pro/blob/release/LICENSE.md
        struct TextData {
            std::string text;
            Eigen::Affine2d transformation;
            Eigen::Vector4d color;
            std::uint8_t alignment;
            bool draw_background;
            Eigen::Vector4d background_color;
        };
    } // namespace simu

    class RobotDARTSimu {
    public:
        using robot_t = std::shared_ptr<Robot>;

        RobotDARTSimu(double timestep = 0.015);

        ~RobotDARTSimu();

        void run(double max_duration = 5.0, bool reset_commands = false);
        bool step_world(bool reset_commands = false);
        bool step(bool reset_commands = false);

        Scheduler& scheduler() { return _scheduler; }
        const Scheduler& scheduler() const { return _scheduler; }
        bool schedule(int freq) { return _scheduler(freq); }

        int physics_freq() const { return _physics_freq; }
        int control_freq() const { return _control_freq; }

        void set_control_freq(int frequency)
        {
            ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(
                frequency <= _physics_freq && "Control frequency needs to be less than physics frequency");
            _control_freq = frequency;
        }

        int graphics_freq() const { return _graphics_freq; }

        void set_graphics_freq(int frequency)
        {
            ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(
                frequency <= _physics_freq && "Graphics frequency needs to be less than physics frequency");
            _graphics_freq = frequency;
            _graphics->set_fps(_graphics_freq);
        }

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

        template <typename T, typename... Args>
        std::shared_ptr<T> add_sensor(Args&&... args)
        {
            add_sensor(std::make_shared<T>(std::forward<Args>(args)...));
            return std::static_pointer_cast<T>(_sensors.back());
        }

        void add_sensor(const std::shared_ptr<sensor::Sensor>& sensor);
        std::vector<std::shared_ptr<sensor::Sensor>> sensors() const;
        std::shared_ptr<sensor::Sensor> sensor(size_t index) const;

        void remove_sensor(const std::shared_ptr<sensor::Sensor>& sensor);
        void remove_sensor(size_t index);
        void remove_sensors(const std::string& type);
        void clear_sensors();

        double timestep() const;
        void set_timestep(double timestep, bool update_control_freq = true);

        Eigen::Vector3d gravity() const;
        void set_gravity(const Eigen::Vector3d& gravity);

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

        void enable_text_panel(bool enable = true);
        std::string text_panel_text() const;
        void set_text_panel(const std::string& str);

        void enable_status_bar(bool enable = true);
        std::string status_bar_text() const;

        std::shared_ptr<simu::TextData> add_text(const std::string& text, const Eigen::Affine2d& tf = Eigen::Affine2d::Identity(), Eigen::Vector4d color = Eigen::Vector4d(1, 1, 1, 1), std::uint8_t alignment = (1 | 3 << 3), bool draw_bg = false, Eigen::Vector4d bg_color = Eigen::Vector4d(0, 0, 0, 0.75));

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
        void _enable(std::shared_ptr<simu::TextData>& text, bool enable);

        dart::simulation::WorldPtr _world;
        size_t _old_index;
        bool _break;

        std::vector<std::shared_ptr<descriptor::BaseDescriptor>> _descriptors;
        std::vector<std::shared_ptr<sensor::Sensor>> _sensors;
        std::vector<robot_t> _robots;
        std::shared_ptr<gui::Base> _graphics;
        std::unique_ptr<simu::GUIData> _gui_data;
        std::shared_ptr<simu::TextData> _text_panel = nullptr;
        std::shared_ptr<simu::TextData> _status_bar = nullptr;

        Scheduler _scheduler;
        int _physics_freq = -1, _control_freq = -1, _graphics_freq = 40;
    };
} // namespace robot_dart

#endif
