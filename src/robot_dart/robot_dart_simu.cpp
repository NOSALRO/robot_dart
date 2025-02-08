#include "robot_dart_simu.hpp"
#include "gui_data.hpp"
#include "utils.hpp"
#include "utils_headers_dart_collision.hpp"
#include "utils_headers_dart_dynamics.hpp"

#include <sstream>

namespace robot_dart {
    namespace collision_filter {
        // This is inspired from Swift: https://developer.apple.com/documentation/spritekit/skphysicsbody#//apple_ref/occ/instp/SKPhysicsBody/collisionBitMask
        // https://stackoverflow.com/questions/39063949/cant-understand-how-collision-bit-mask-works
        class BitmaskContactFilter : public dart::collision::BodyNodeCollisionFilter {
        public:
            using DartCollisionConstPtr = const dart::collision::CollisionObject*;
            using DartShapeConstPtr = const dart::dynamics::ShapeNode*;

            struct Masks {
                uint32_t collision_mask = 0xffffffff;
                uint32_t category_mask = 0xffffffff;
            };

            virtual ~BitmaskContactFilter() = default;

            // This function follows DART's coding style as it needs to override a function
            bool ignoresCollision(DartCollisionConstPtr object1, DartCollisionConstPtr object2) const override
            {
                auto shape_node1 = object1->getShapeFrame()->asShapeNode();
                auto shape_node2 = object2->getShapeFrame()->asShapeNode();

                if (dart::collision::BodyNodeCollisionFilter::ignoresCollision(object1, object2))
                    return true;

                auto shape1_iter = _bitmask_map.find(shape_node1);
                auto shape2_iter = _bitmask_map.find(shape_node2);
                if (shape1_iter != _bitmask_map.end() && shape2_iter != _bitmask_map.end()) {
                    auto col_mask1 = shape1_iter->second.collision_mask;
                    auto cat_mask1 = shape1_iter->second.category_mask;

                    auto col_mask2 = shape2_iter->second.collision_mask;
                    auto cat_mask2 = shape2_iter->second.category_mask;

                    if ((col_mask1 & cat_mask2) == 0 && (col_mask2 & cat_mask1) == 0)
                        return true;
                }

                return false;
            }

            void add_to_map(DartShapeConstPtr shape, uint32_t col_mask, uint32_t cat_mask)
            {
                _bitmask_map[shape] = {col_mask, cat_mask};
            }

            void add_to_map(dart::dynamics::SkeletonPtr skel, uint32_t col_mask, uint32_t cat_mask)
            {
                for (std::size_t i = 0; i < skel->getNumShapeNodes(); ++i) {
                    auto shape = skel->getShapeNode(i);
                    this->add_to_map(shape, col_mask, cat_mask);
                }
            }

            void remove_from_map(DartShapeConstPtr shape)
            {
                auto shape_iter = _bitmask_map.find(shape);
                if (shape_iter != _bitmask_map.end())
                    _bitmask_map.erase(shape_iter);
            }

            void remove_from_map(dart::dynamics::SkeletonPtr skel)
            {
                for (std::size_t i = 0; i < skel->getNumShapeNodes(); ++i) {
                    auto shape = skel->getShapeNode(i);
                    this->remove_from_map(shape);
                }
            }

            void clear_all() { _bitmask_map.clear(); }

            Masks mask(DartShapeConstPtr shape) const
            {
                auto shape_iter = _bitmask_map.find(shape);
                if (shape_iter != _bitmask_map.end())
                    return shape_iter->second;
                return {0xffffffff, 0xffffffff};
            }

        private:
            // We need ShapeNodes and not BodyNodes, since in DART collision checking is performed in ShapeNode-level
            // in RobotDARTSimu, we only allow setting one mask per BodyNode; maybe we can improve the performance of this slightly
            std::unordered_map<DartShapeConstPtr, Masks> _bitmask_map;
        };
    } // namespace collision_filter

    RobotDARTSimu::RobotDARTSimu(double timestep) : _world(std::make_shared<dart::simulation::World>()),
                                                    _old_index(0),
                                                    _break(false),
                                                    _scheduler(timestep),
                                                    _physics_freq(std::round(1. / timestep)),
                                                    _control_freq(_physics_freq)
    {
        _world->getConstraintSolver()->setCollisionDetector(dart::collision::DARTCollisionDetector::create());
        _world->getConstraintSolver()->getCollisionOption().collisionFilter = std::make_shared<collision_filter::BitmaskContactFilter>();
        _world->setTimeStep(timestep);
        _world->setTime(0.0);
        _graphics = std::make_shared<gui::Base>();

        _gui_data.reset(new simu::GUIData());
    }

    RobotDARTSimu::~RobotDARTSimu()
    {
        _robots.clear();
        _sensors.clear();
    }

    void RobotDARTSimu::run(double max_duration, bool reset_commands, bool force_position_bounds)
    {
        _break = false;
        double old_time = _world->getTime();
        double factor = _world->getTimeStep() / 2.;

        while ((_world->getTime() - old_time - max_duration) < -factor) {
            if (step(reset_commands, force_position_bounds))
                break;
        }
    }

    bool RobotDARTSimu::step_world(bool reset_commands, bool force_position_bounds)
    {
        if (_scheduler(_physics_freq)) {
            _world->step(reset_commands);
            if (force_position_bounds)
                for (auto& r : _robots)
                    r->force_position_bounds();
        }

        // Update graphics
        if (_scheduler(_graphics_freq)) {
            // Update default texts
            if (_text_panel) { // Need to re-transform as the size of the window might have changed
                Eigen::Affine2d tf = Eigen::Affine2d::Identity();
                tf.translate(Eigen::Vector2d(-static_cast<double>(_graphics->width()) / 2., _graphics->height() / 2.));
                // tf.translate(Eigen::Vector2d(-static_cast<double>(_graphics->width()) / 2., 0.));
                _text_panel->transformation = tf;
            }
            if (_status_bar) {
                _status_bar->text = status_bar_text(); // this is dynamic text (timings)
                Eigen::Affine2d tf = Eigen::Affine2d::Identity();
                tf.translate(Eigen::Vector2d(-static_cast<double>(_graphics->width()) / 2., -static_cast<double>(_graphics->height() / 2.)));
                // tf.translate(Eigen::Vector2d(-static_cast<double>(_graphics->width()) / 2., 0.));
                _status_bar->transformation = tf;
            }

            // Update robot-specific GUI data
            for (auto& robot : _robots) {
                _gui_data->update_robot(robot);
            }

            _graphics->refresh();
        }

        // update sensors
        for (auto& sensor : _sensors) {
            if (sensor->active() && _scheduler(sensor->frequency())) {
                sensor->refresh(_world->getTime());
            }
        }

        _old_index++;
        _scheduler.step();

        return _break || _graphics->done();
    }

    bool RobotDARTSimu::step(bool reset_commands, bool force_position_bounds)
    {
        if (_scheduler(_control_freq)) {
            for (auto& robot : _robots) {
                robot->update(_world->getTime());
            }
        }

        return step_world(reset_commands, force_position_bounds);
    }

    std::shared_ptr<gui::Base> RobotDARTSimu::graphics() const
    {
        return _graphics;
    }

    void RobotDARTSimu::set_graphics(const std::shared_ptr<gui::Base>& graphics)
    {
        _graphics = graphics;
        _graphics->set_simu(this);
        _graphics->set_fps(_graphics_freq);
    }

    dart::simulation::WorldPtr RobotDARTSimu::world()
    {
        return _world;
    }

    void RobotDARTSimu::add_sensor(const std::shared_ptr<sensor::Sensor>& sensor)
    {
        _sensors.push_back(sensor);
        sensor->set_simu(this);
        sensor->init();
    }

    std::vector<std::shared_ptr<sensor::Sensor>> RobotDARTSimu::sensors() const
    {
        return _sensors;
    }

    std::shared_ptr<sensor::Sensor> RobotDARTSimu::sensor(size_t index) const
    {
        ROBOT_DART_ASSERT(index < _sensors.size(), "Sensor index out of bounds", nullptr);
        return _sensors[index];
    }

    void RobotDARTSimu::remove_sensor(const std::shared_ptr<sensor::Sensor>& sensor)
    {
        auto it = std::find(_sensors.begin(), _sensors.end(), sensor);
        if (it != _sensors.end()) {
            _sensors.erase(it);
        }
    }

    void RobotDARTSimu::remove_sensor(size_t index)
    {
        ROBOT_DART_ASSERT(index < _sensors.size(), "Sensor index out of bounds", );
        _sensors.erase(_sensors.begin() + index);
    }

    void RobotDARTSimu::remove_sensors(const std::string& type)
    {
        for (int i = 0; i < static_cast<int>(_sensors.size()); i++) {
            auto& sensor = _sensors[i];
            if (sensor->type() == type) {
                _sensors.erase(_sensors.begin() + i);
                i--;
            }
        }
    }

    void RobotDARTSimu::clear_sensors()
    {
        _sensors.clear();
    }

    double RobotDARTSimu::timestep() const
    {
        return _world->getTimeStep();
    }

    void RobotDARTSimu::set_timestep(double timestep, bool update_control_freq)
    {
        _world->setTimeStep(timestep);
        _physics_freq = std::round(1. / timestep);
        if (update_control_freq)
            _control_freq = _physics_freq;

        _scheduler.reset(timestep, _scheduler.sync(), _scheduler.current_time(), _scheduler.real_time());
    }

    Eigen::Vector3d RobotDARTSimu::gravity() const
    {
        return _world->getGravity();
    }

    void RobotDARTSimu::set_gravity(const Eigen::Vector3d& gravity)
    {
        _world->setGravity(gravity);
    }

    void RobotDARTSimu::stop_sim(bool disable)
    {
        _break = disable;
    }

    bool RobotDARTSimu::halted_sim() const
    {
        return _break;
    }

    size_t RobotDARTSimu::num_robots() const
    {
        return _robots.size();
    }

    const std::vector<std::shared_ptr<Robot>>& RobotDARTSimu::robots() const
    {
        return _robots;
    }

    std::shared_ptr<Robot> RobotDARTSimu::robot(size_t index) const
    {
        ROBOT_DART_ASSERT(index < _robots.size(), "Robot index out of bounds", nullptr);
        return _robots[index];
    }

    int RobotDARTSimu::robot_index(const std::shared_ptr<Robot>& robot) const
    {
        auto it = std::find(_robots.begin(), _robots.end(), robot);
        ROBOT_DART_ASSERT(it != _robots.end(), "Robot index out of bounds", -1);
        return std::distance(_robots.begin(), it);
    }

    void RobotDARTSimu::add_robot(const std::shared_ptr<Robot>& robot)
    {
        if (robot->skeleton()) {
            _robots.push_back(robot);
            _world->addSkeleton(robot->skeleton());

            robot->_post_addition(this);

            _gui_data->update_robot(robot);
        }
    }

    void RobotDARTSimu::add_visual_robot(const std::shared_ptr<Robot>& robot)
    {
        if (robot->skeleton()) {
            // make robot a pure visual one -- assuming that the color is already set
            // visual robots do not do physics updates
            robot->skeleton()->setMobile(false);
            for (auto& bd : robot->skeleton()->getBodyNodes()) {
#if DART_VERSION_AT_LEAST(6, 13, 0)
                // visual robots do not have collisions
                bd->eachShapeNodeWith<dart::dynamics::CollisionAspect>([](dart::dynamics::ShapeNode* shape) {
                    shape->removeAspect<dart::dynamics::CollisionAspect>();
                });
#else
                auto& collision_shapes = bd->getShapeNodesWith<dart::dynamics::CollisionAspect>();
                for (auto& shape : collision_shapes) {
                    shape->removeAspect<dart::dynamics::CollisionAspect>();
                }
#endif
            }

            // visual robots, by default, use the color from the VisualAspect
            robot->set_color_mode("aspect");

            // visual robots do not cast shadows
            robot->set_cast_shadows(false);
            // set the ghost/visual flag
            robot->set_ghost(true);
            robot->_post_addition(this);

            _robots.push_back(robot);
            _world->addSkeleton(robot->skeleton());

            _gui_data->update_robot(robot);
        }
    }

    void RobotDARTSimu::remove_robot(const std::shared_ptr<Robot>& robot)
    {
        auto it = std::find(_robots.begin(), _robots.end(), robot);
        if (it != _robots.end()) {
            robot->_post_removal(this);
            _gui_data->remove_robot(robot);
            _world->removeSkeleton(robot->skeleton());
            _robots.erase(it);
        }
    }

    void RobotDARTSimu::remove_robot(size_t index)
    {
        ROBOT_DART_ASSERT(index < _robots.size(), "Robot index out of bounds", );
        _robots[index]->_post_removal(this);
        _gui_data->remove_robot(_robots[index]);
        _world->removeSkeleton(_robots[index]->skeleton());
        _robots.erase(_robots.begin() + index);
    }

    void RobotDARTSimu::clear_robots()
    {
        for (auto& robot : _robots) {
            robot->_post_removal(this);
            _gui_data->remove_robot(robot);
            _world->removeSkeleton(robot->skeleton());
        }
        _robots.clear();
    }

    simu::GUIData* RobotDARTSimu::gui_data() { return &(*_gui_data); }

    void RobotDARTSimu::enable_text_panel(bool enable, double font_size)
    {
        _enable(_text_panel, enable, font_size);
        if (enable) {
            _text_panel->alignment = 3 << 2; // alignment of status bar should be LineTop; TO-DO: Check how to get types automatically from Magnum?
            // _text_panel->draw_background = true; // we want to draw a background
            // _text_panel->background_color = Eigen::Vector4d(0, 0, 0, 0.75); // black-transparent bar
        }
    }

    void RobotDARTSimu::enable_status_bar(bool enable, double font_size)
    {
        _enable(_status_bar, enable, font_size);
        if (enable) {
            _status_bar->alignment = 1 << 2; // alignment of status bar should be LineBottom; TO-DO: Check how to get types automatically from Magnum?
            _status_bar->draw_background = true; // we want to draw a background
            _status_bar->background_color = Eigen::Vector4d(0, 0, 0, 0.75); // black-transparent bar
        }
    }

    void RobotDARTSimu::_enable(std::shared_ptr<simu::TextData>& text, bool enable, double font_size)
    {
        if (!text && enable) {
            text = add_text("");
        }
        else if (!enable) {
            if (text)
                _gui_data->remove_text(text);
            text = nullptr;
        }
        if (text && font_size > 0)
            text->font_size = font_size;
    }

    void RobotDARTSimu::set_text_panel(const std::string& str)
    {
        ROBOT_DART_ASSERT(_text_panel, "Panel text object not created. Use enable_text_panel() to create it.", );
        _text_panel->text = str;
    }

    std::string RobotDARTSimu::text_panel_text() const
    {
        ROBOT_DART_ASSERT(_text_panel, "Panel text object not created. Returning empty string.", "");
        return _text_panel->text;
    }

    std::string RobotDARTSimu::status_bar_text() const
    {
        std::ostringstream out;
        out.precision(3);
        double rt = _scheduler.real_time();

        out << std::fixed << "[simulation time: " << _world->getTime()
            << "s ] ["
            << "wall time: " << rt << "s] [";
        out.precision(1);
        out << _scheduler.real_time_factor() << "x]";
        out << " [it: " << _scheduler.it_duration() * 1e3 << " ms]";
        out << (_scheduler.sync() ? " [sync]" : " [no-sync]");

        return out.str();
    }

    std::shared_ptr<simu::TextData> RobotDARTSimu::add_text(const std::string& text, const Eigen::Affine2d& tf, Eigen::Vector4d color, std::uint8_t alignment, bool draw_bg, Eigen::Vector4d bg_color, double font_size)
    {
        return _gui_data->add_text(text, tf, color, alignment, draw_bg, bg_color, font_size);
    }

    std::shared_ptr<Robot> RobotDARTSimu::add_floor(double floor_width, double floor_height, const Eigen::Isometry3d& tf, const std::string& floor_name)
    {
        ROBOT_DART_ASSERT((_world->getSkeleton(floor_name) == nullptr), "We cannot have 2 floors with the name '" + floor_name + "'", nullptr);
        ROBOT_DART_ASSERT((floor_width > 0. && floor_height > 0.), "Floor dimensions should be bigger than zero!", nullptr);

        dart::dynamics::SkeletonPtr floor_skel = dart::dynamics::Skeleton::create(floor_name);

        // Give the floor a body
        dart::dynamics::BodyNodePtr body = floor_skel->createJointAndBodyNodePair<dart::dynamics::WeldJoint>(nullptr).second;

        // Give the body a shape
        auto box = std::make_shared<dart::dynamics::BoxShape>(Eigen::Vector3d(floor_width, floor_width, floor_height));
        auto box_node = body->createShapeNodeWith<dart::dynamics::VisualAspect, dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(box);
        box_node->getVisualAspect()->setColor(dart::Color::Gray());

        // Put the body into position
        Eigen::Isometry3d new_tf = tf;
        new_tf.translate(Eigen::Vector3d(0., 0., -floor_height / 2.0));
        body->getParentJoint()->setTransformFromParentBodyNode(new_tf);

        auto floor_robot = std::make_shared<Robot>(floor_skel, floor_name);
        add_robot(floor_robot);
        return floor_robot;
    }

    std::shared_ptr<Robot> RobotDARTSimu::add_checkerboard_floor(double floor_width, double floor_height, double size, const Eigen::Isometry3d& tf, const std::string& floor_name, const Eigen::Vector4d& first_color, const Eigen::Vector4d& second_color)
    {
        ROBOT_DART_ASSERT((_world->getSkeleton(floor_name) == nullptr), "We cannot have 2 floors with the name '" + floor_name + "'", nullptr);
        ROBOT_DART_ASSERT((floor_width > 0. && floor_height > 0. && size > 0.), "Floor dimensions should be bigger than zero!", nullptr);

        // Add main floor skeleton
        dart::dynamics::SkeletonPtr main_floor_skel = dart::dynamics::Skeleton::create(floor_name + "_main");

        // Give the floor a body
        dart::dynamics::BodyNodePtr main_body = main_floor_skel->createJointAndBodyNodePair<dart::dynamics::WeldJoint>(nullptr).second;

        // Give the body a shape
        auto box = std::make_shared<dart::dynamics::BoxShape>(Eigen::Vector3d(floor_width, floor_width, floor_height));
        // No visual shape for this one; only collision and dynamics
        main_body->createShapeNodeWith<dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(box);

        // Put the body into position
        Eigen::Isometry3d new_tf = tf;
        new_tf.translate(Eigen::Vector3d(0., 0., -floor_height / 2.0));
        main_body->getParentJoint()->setTransformFromParentBodyNode(new_tf);

        // Add visual bodies just for visualization
        int step = std::ceil(floor_width / size);
        int c = 0;
        for (int i = 0; i < step; i++) {
            c = i;
            for (int j = 0; j < step; j++) {
                Eigen::Vector3d init_pose;
                init_pose << -floor_width / 2. + size / 2 + i * size, -floor_width / 2. + size / 2 + j * size, 0.;
                int id = i * step + j;

                dart::dynamics::WeldJoint::Properties properties;
                properties.mName = "joint_" + std::to_string(id);
                dart::dynamics::BodyNode::Properties bd_properties;
                bd_properties.mName = "body_" + std::to_string(id);
                dart::dynamics::BodyNodePtr body = main_body->createChildJointAndBodyNodePair<dart::dynamics::WeldJoint>(properties, bd_properties).second;

                auto box = std::make_shared<dart::dynamics::BoxShape>(Eigen::Vector3d(size, size, floor_height));
                // no collision/dynamics for these ones; only visual shape
                auto box_node = body->createShapeNodeWith<dart::dynamics::VisualAspect>(box);
                if (c % 2 == 0)
                    box_node->getVisualAspect()->setColor(second_color);
                else
                    box_node->getVisualAspect()->setColor(first_color);

                // Put the body into position
                Eigen::Isometry3d tf(Eigen::Isometry3d::Identity());
                tf.translation() = init_pose;
                body->getParentJoint()->setTransformFromParentBodyNode(tf);

                c++;
            }
        }

        auto floor_robot = std::make_shared<Robot>(main_floor_skel, floor_name);
        add_robot(floor_robot);
        return floor_robot;
    }

    void RobotDARTSimu::set_collision_detector(const std::string& collision_detector)
    {
        std::string coll = collision_detector;
        for (auto& c : coll)
            c = tolower(c);

        if (coll == "dart")
            _world->getConstraintSolver()->setCollisionDetector(dart::collision::DARTCollisionDetector::create());
        else if (coll == "fcl")
            _world->getConstraintSolver()->setCollisionDetector(dart::collision::FCLCollisionDetector::create());
        else if (coll == "bullet") {
#if (HAVE_BULLET == 1)
            _world->getConstraintSolver()->setCollisionDetector(dart::collision::BulletCollisionDetector::create());
#else
            ROBOT_DART_WARNING(true, "DART is not installed with Bullet! Cannot set BulletCollisionDetector!");
#endif
        }
        else if (coll == "ode") {
#if (HAVE_ODE == 1)
            _world->getConstraintSolver()->setCollisionDetector(dart::collision::OdeCollisionDetector::create());
#else
            ROBOT_DART_WARNING(true, "DART is not installed with ODE! Cannot set OdeCollisionDetector!");
#endif
        }
    }

    const std::string& RobotDARTSimu::collision_detector() const { return _world->getConstraintSolver()->getCollisionDetector()->getType(); }

    void RobotDARTSimu::set_collision_masks(size_t robot_index, uint32_t category_mask, uint32_t collision_mask)
    {
        ROBOT_DART_ASSERT(robot_index < _robots.size(), "Robot index out of bounds", );
        auto coll_filter = std::static_pointer_cast<collision_filter::BitmaskContactFilter>(_world->getConstraintSolver()->getCollisionOption().collisionFilter);
        coll_filter->add_to_map(_robots[robot_index]->skeleton(), collision_mask, category_mask);
    }

    void RobotDARTSimu::set_collision_masks(size_t robot_index, const std::string& body_name, uint32_t category_mask, uint32_t collision_mask)
    {
        ROBOT_DART_ASSERT(robot_index < _robots.size(), "Robot index out of bounds", );
        auto bd = _robots[robot_index]->skeleton()->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", );
        auto coll_filter = std::static_pointer_cast<collision_filter::BitmaskContactFilter>(_world->getConstraintSolver()->getCollisionOption().collisionFilter);
#if DART_VERSION_AT_LEAST(6, 13, 0)
        bd->eachShapeNode([coll_filter, collision_mask, category_mask](dart::dynamics::ShapeNode* shape) { coll_filter->add_to_map(shape, collision_mask, category_mask); });
#else
        for (auto& shape : bd->getShapeNodes())
            coll_filter->add_to_map(shape, collision_mask, category_mask);
#endif
    }

    void RobotDARTSimu::set_collision_masks(size_t robot_index, size_t body_index, uint32_t category_mask, uint32_t collision_mask)
    {
        ROBOT_DART_ASSERT(robot_index < _robots.size(), "Robot index out of bounds", );
        auto skel = _robots[robot_index]->skeleton();
        ROBOT_DART_ASSERT(body_index < skel->getNumBodyNodes(), "BodyNode index out of bounds", );
        auto bd = skel->getBodyNode(body_index);
        auto coll_filter = std::static_pointer_cast<collision_filter::BitmaskContactFilter>(_world->getConstraintSolver()->getCollisionOption().collisionFilter);
#if DART_VERSION_AT_LEAST(6, 13, 0)
        bd->eachShapeNode([coll_filter, collision_mask, category_mask](dart::dynamics::ShapeNode* shape) { coll_filter->add_to_map(shape, collision_mask, category_mask); });
#else
        for (auto& shape : bd->getShapeNodes())
            coll_filter->add_to_map(shape, collision_mask, category_mask);
#endif
    }

    uint32_t RobotDARTSimu::collision_mask(size_t robot_index, const std::string& body_name)
    {
        ROBOT_DART_ASSERT(robot_index < _robots.size(), "Robot index out of bounds", 0xffffffff);
        auto bd = _robots[robot_index]->skeleton()->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", 0xffffffff);
        auto coll_filter = std::static_pointer_cast<collision_filter::BitmaskContactFilter>(_world->getConstraintSolver()->getCollisionOption().collisionFilter);

        uint32_t mask = 0xffffffff;
#if DART_VERSION_AT_LEAST(6, 13, 0)
        bd->eachShapeNode([coll_filter, &mask](dart::dynamics::ShapeNode* shape) { mask &= coll_filter->mask(shape).collision_mask; });
#else
        for (auto& shape : bd->getShapeNodes())
            mask &= coll_filter->mask(shape).collision_mask;
#endif

        return mask;
    }

    uint32_t RobotDARTSimu::collision_mask(size_t robot_index, size_t body_index)
    {
        ROBOT_DART_ASSERT(robot_index < _robots.size(), "Robot index out of bounds", 0xffffffff);
        auto skel = _robots[robot_index]->skeleton();
        ROBOT_DART_ASSERT(body_index < skel->getNumBodyNodes(), "BodyNode index out of bounds", 0xffffffff);
        auto bd = skel->getBodyNode(body_index);
        auto coll_filter = std::static_pointer_cast<collision_filter::BitmaskContactFilter>(_world->getConstraintSolver()->getCollisionOption().collisionFilter);

        uint32_t mask = 0xffffffff;
#if DART_VERSION_AT_LEAST(6, 13, 0)
        bd->eachShapeNode([coll_filter, &mask](dart::dynamics::ShapeNode* shape) { mask &= coll_filter->mask(shape).collision_mask; });
#else
        for (auto& shape : bd->getShapeNodes())
            mask &= coll_filter->mask(shape).collision_mask;
#endif

        return mask;
    }

    uint32_t RobotDARTSimu::collision_category(size_t robot_index, const std::string& body_name)
    {
        ROBOT_DART_ASSERT(robot_index < _robots.size(), "Robot index out of bounds", 0xffffffff);
        auto bd = _robots[robot_index]->skeleton()->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", 0xffffffff);
        auto coll_filter = std::static_pointer_cast<collision_filter::BitmaskContactFilter>(_world->getConstraintSolver()->getCollisionOption().collisionFilter);

        uint32_t mask = 0xffffffff;
#if DART_VERSION_AT_LEAST(6, 13, 0)
        bd->eachShapeNode([coll_filter, &mask](dart::dynamics::ShapeNode* shape) { mask &= coll_filter->mask(shape).category_mask; });
#else
        for (auto& shape : bd->getShapeNodes())
            mask &= coll_filter->mask(shape).category_mask;
#endif

        return mask;
    }

    uint32_t RobotDARTSimu::collision_category(size_t robot_index, size_t body_index)
    {
        ROBOT_DART_ASSERT(robot_index < _robots.size(), "Robot index out of bounds", 0xffffffff);
        auto skel = _robots[robot_index]->skeleton();
        ROBOT_DART_ASSERT(body_index < skel->getNumBodyNodes(), "BodyNode index out of bounds", 0xffffffff);
        auto bd = skel->getBodyNode(body_index);
        auto coll_filter = std::static_pointer_cast<collision_filter::BitmaskContactFilter>(_world->getConstraintSolver()->getCollisionOption().collisionFilter);

        uint32_t mask = 0xffffffff;
#if DART_VERSION_AT_LEAST(6, 13, 0)
        bd->eachShapeNode([coll_filter, &mask](dart::dynamics::ShapeNode* shape) { mask &= coll_filter->mask(shape).category_mask; });
#else
        for (auto& shape : bd->getShapeNodes())
            mask &= coll_filter->mask(shape).category_mask;
#endif

        return mask;
    }

    std::pair<uint32_t, uint32_t> RobotDARTSimu::collision_masks(size_t robot_index, const std::string& body_name)
    {
        std::pair<uint32_t, uint32_t> mask = {0xffffffff, 0xffffffff};
        ROBOT_DART_ASSERT(robot_index < _robots.size(), "Robot index out of bounds", mask);
        auto bd = _robots[robot_index]->skeleton()->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", mask);
        auto coll_filter = std::static_pointer_cast<collision_filter::BitmaskContactFilter>(_world->getConstraintSolver()->getCollisionOption().collisionFilter);

#if DART_VERSION_AT_LEAST(6, 13, 0)
        bd->eachShapeNode([coll_filter, &mask](dart::dynamics::ShapeNode* shape) {
            mask.first &= coll_filter->mask(shape).collision_mask;
            mask.second &= coll_filter->mask(shape).category_mask;
        });
#else
        for (auto& shape : bd->getShapeNodes()) {
            mask.first &= coll_filter->mask(shape).collision_mask;
            mask.second &= coll_filter->mask(shape).category_mask;
        }
#endif

        return mask;
    }

    std::pair<uint32_t, uint32_t> RobotDARTSimu::collision_masks(size_t robot_index, size_t body_index)
    {
        std::pair<uint32_t, uint32_t> mask = {0xffffffff, 0xffffffff};
        ROBOT_DART_ASSERT(robot_index < _robots.size(), "Robot index out of bounds", mask);
        auto skel = _robots[robot_index]->skeleton();
        ROBOT_DART_ASSERT(body_index < skel->getNumBodyNodes(), "BodyNode index out of bounds", mask);
        auto bd = skel->getBodyNode(body_index);
        auto coll_filter = std::static_pointer_cast<collision_filter::BitmaskContactFilter>(_world->getConstraintSolver()->getCollisionOption().collisionFilter);

#if DART_VERSION_AT_LEAST(6, 13, 0)
        bd->eachShapeNode([coll_filter, &mask](dart::dynamics::ShapeNode* shape) {
            mask.first &= coll_filter->mask(shape).collision_mask;
            mask.second &= coll_filter->mask(shape).category_mask;
        });
#else
        for (auto& shape : bd->getShapeNodes()) {
            mask.first &= coll_filter->mask(shape).collision_mask;
            mask.second &= coll_filter->mask(shape).category_mask;
        }
#endif

        return mask;
    }

    void RobotDARTSimu::remove_collision_masks(size_t robot_index)
    {
        ROBOT_DART_ASSERT(robot_index < _robots.size(), "Robot index out of bounds", );
        auto coll_filter = std::static_pointer_cast<collision_filter::BitmaskContactFilter>(_world->getConstraintSolver()->getCollisionOption().collisionFilter);
        coll_filter->remove_from_map(_robots[robot_index]->skeleton());
    }

    void RobotDARTSimu::remove_collision_masks(size_t robot_index, const std::string& body_name)
    {
        ROBOT_DART_ASSERT(robot_index < _robots.size(), "Robot index out of bounds", );
        auto bd = _robots[robot_index]->skeleton()->getBodyNode(body_name);
        ROBOT_DART_ASSERT(bd != nullptr, "BodyNode does not exist in skeleton!", );
        auto coll_filter = std::static_pointer_cast<collision_filter::BitmaskContactFilter>(_world->getConstraintSolver()->getCollisionOption().collisionFilter);
#if DART_VERSION_AT_LEAST(6, 13, 0)
        bd->eachShapeNode([coll_filter](dart::dynamics::ShapeNode* shape) { coll_filter->remove_from_map(shape); });
#else
        for (auto& shape : bd->getShapeNodes())
            coll_filter->remove_from_map(shape);
#endif
    }

    void RobotDARTSimu::remove_collision_masks(size_t robot_index, size_t body_index)
    {
        ROBOT_DART_ASSERT(robot_index < _robots.size(), "Robot index out of bounds", );
        auto skel = _robots[robot_index]->skeleton();
        ROBOT_DART_ASSERT(body_index < skel->getNumBodyNodes(), "BodyNode index out of bounds", );
        auto bd = skel->getBodyNode(body_index);
        auto coll_filter = std::static_pointer_cast<collision_filter::BitmaskContactFilter>(_world->getConstraintSolver()->getCollisionOption().collisionFilter);
#if DART_VERSION_AT_LEAST(6, 13, 0)
        bd->eachShapeNode([coll_filter](dart::dynamics::ShapeNode* shape) { coll_filter->remove_from_map(shape); });
#else
        for (auto& shape : bd->getShapeNodes())
            coll_filter->remove_from_map(shape);
#endif
    }

    void RobotDARTSimu::remove_all_collision_masks()
    {
        auto coll_filter = std::static_pointer_cast<collision_filter::BitmaskContactFilter>(_world->getConstraintSolver()->getCollisionOption().collisionFilter);
        coll_filter->clear_all();
    }
} // namespace robot_dart
