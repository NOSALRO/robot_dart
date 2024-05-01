

# Class robot\_dart::robots::Hexapod



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**robots**](namespacerobot__dart_1_1robots.md) **>** [**Hexapod**](classrobot__dart_1_1robots_1_1Hexapod.md)








Inherits the following classes: [robot\_dart::Robot](classrobot__dart_1_1Robot.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Hexapod**](#function-hexapod) (const std::string & urdf="pexod.urdf") <br> |
| virtual void | [**reset**](#function-reset) () override<br> |


## Public Functions inherited from robot_dart::Robot

See [robot\_dart::Robot](classrobot__dart_1_1Robot.md)

| Type | Name |
| ---: | :--- |
|   | [**Robot**](#function-robot-13) (const std::string & model\_file, const std::vector&lt; std::pair&lt; std::string, std::string &gt; &gt; & packages, const std::string & robot\_name="robot", bool is\_urdf\_string=false, bool cast\_shadows=true) <br> |
|   | [**Robot**](#function-robot-23) (const std::string & model\_file, const std::string & robot\_name="robot", bool is\_urdf\_string=false, bool cast\_shadows=true) <br> |
|   | [**Robot**](#function-robot-33) (dart::dynamics::SkeletonPtr skeleton, const std::string & robot\_name="robot", bool cast\_shadows=true) <br> |
|  Eigen::VectorXd | [**acceleration\_lower\_limits**](#function-acceleration_lower_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**acceleration\_upper\_limits**](#function-acceleration_upper_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**accelerations**](#function-accelerations) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  std::vector&lt; std::shared\_ptr&lt; [**control::RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; &gt; | [**active\_controllers**](#function-active_controllers) () const<br> |
|  std::string | [**actuator\_type**](#function-actuator_type) (const std::string & joint\_name) const<br> |
|  std::vector&lt; std::string &gt; | [**actuator\_types**](#function-actuator_types) (const std::vector&lt; std::string &gt; & joint\_names={}) const<br> |
|  void | [**add\_body\_mass**](#function-add_body_mass-12) (const std::string & body\_name, double mass) <br> |
|  void | [**add\_body\_mass**](#function-add_body_mass-22) (size\_t body\_index, double mass) <br> |
|  void | [**add\_controller**](#function-add_controller) (const std::shared\_ptr&lt; [**control::RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; & controller, double weight=1.0) <br> |
|  void | [**add\_external\_force**](#function-add_external_force-12) (const std::string & body\_name, const Eigen::Vector3d & force, const Eigen::Vector3d & offset=Eigen::Vector3d::Zero(), bool force\_local=false, bool offset\_local=true) <br> |
|  void | [**add\_external\_force**](#function-add_external_force-22) (size\_t body\_index, const Eigen::Vector3d & force, const Eigen::Vector3d & offset=Eigen::Vector3d::Zero(), bool force\_local=false, bool offset\_local=true) <br> |
|  void | [**add\_external\_torque**](#function-add_external_torque-12) (const std::string & body\_name, const Eigen::Vector3d & torque, bool local=false) <br> |
|  void | [**add\_external\_torque**](#function-add_external_torque-22) (size\_t body\_index, const Eigen::Vector3d & torque, bool local=false) <br> |
|  bool | [**adjacent\_colliding**](#function-adjacent_colliding) () const<br> |
|  Eigen::MatrixXd | [**aug\_mass\_matrix**](#function-aug_mass_matrix) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::Isometry3d | [**base\_pose**](#function-base_pose) () const<br> |
|  Eigen::Vector6d | [**base\_pose\_vec**](#function-base_pose_vec) () const<br> |
|  Eigen::Vector6d | [**body\_acceleration**](#function-body_acceleration-12) (const std::string & body\_name) const<br> |
|  Eigen::Vector6d | [**body\_acceleration**](#function-body_acceleration-22) (size\_t body\_index) const<br> |
|  size\_t | [**body\_index**](#function-body_index) (const std::string & body\_name) const<br> |
|  double | [**body\_mass**](#function-body_mass-12) (const std::string & body\_name) const<br> |
|  double | [**body\_mass**](#function-body_mass-22) (size\_t body\_index) const<br> |
|  std::string | [**body\_name**](#function-body_name) (size\_t body\_index) const<br> |
|  std::vector&lt; std::string &gt; | [**body\_names**](#function-body_names) () const<br> |
|  dart::dynamics::BodyNode \* | [**body\_node**](#function-body_node-12) (const std::string & body\_name) <br> |
|  dart::dynamics::BodyNode \* | [**body\_node**](#function-body_node-22) (size\_t body\_index) <br> |
|  Eigen::Isometry3d | [**body\_pose**](#function-body_pose-12) (const std::string & body\_name) const<br> |
|  Eigen::Isometry3d | [**body\_pose**](#function-body_pose-22) (size\_t body\_index) const<br> |
|  Eigen::Vector6d | [**body\_pose\_vec**](#function-body_pose_vec-12) (const std::string & body\_name) const<br> |
|  Eigen::Vector6d | [**body\_pose\_vec**](#function-body_pose_vec-22) (size\_t body\_index) const<br> |
|  Eigen::Vector6d | [**body\_velocity**](#function-body_velocity-12) (const std::string & body\_name) const<br> |
|  Eigen::Vector6d | [**body\_velocity**](#function-body_velocity-22) (size\_t body\_index) const<br> |
|  bool | [**cast\_shadows**](#function-cast_shadows) () const<br> |
|  void | [**clear\_controllers**](#function-clear_controllers) () <br> |
|  void | [**clear\_external\_forces**](#function-clear_external_forces) () <br> |
|  void | [**clear\_internal\_forces**](#function-clear_internal_forces) () <br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**clone**](#function-clone) () const<br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**clone\_ghost**](#function-clone_ghost) (const std::string & ghost\_name="ghost", const Eigen::Vector4d & ghost\_color={0.3, 0.3, 0.3, 0.7}) const<br> |
|  Eigen::Vector3d | [**com**](#function-com) () const<br> |
|  Eigen::Vector6d | [**com\_acceleration**](#function-com_acceleration) () const<br> |
|  Eigen::MatrixXd | [**com\_jacobian**](#function-com_jacobian) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::MatrixXd | [**com\_jacobian\_deriv**](#function-com_jacobian_deriv) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::Vector6d | [**com\_velocity**](#function-com_velocity) () const<br> |
|  Eigen::VectorXd | [**commands**](#function-commands) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**constraint\_forces**](#function-constraint_forces) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  std::shared\_ptr&lt; [**control::RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; | [**controller**](#function-controller) (size\_t index) const<br> |
|  std::vector&lt; std::shared\_ptr&lt; [**control::RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; &gt; | [**controllers**](#function-controllers) () const<br> |
|  Eigen::VectorXd | [**coriolis\_forces**](#function-coriolis_forces) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**coriolis\_gravity\_forces**](#function-coriolis_gravity_forces) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  std::vector&lt; double &gt; | [**coulomb\_coeffs**](#function-coulomb_coeffs) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  std::vector&lt; double &gt; | [**damping\_coeffs**](#function-damping_coeffs) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  dart::dynamics::DegreeOfFreedom \* | [**dof**](#function-dof-12) (const std::string & dof\_name) <br> |
|  dart::dynamics::DegreeOfFreedom \* | [**dof**](#function-dof-22) (size\_t dof\_index) <br> |
|  size\_t | [**dof\_index**](#function-dof_index) (const std::string & dof\_name) const<br> |
|  const std::unordered\_map&lt; std::string, size\_t &gt; & | [**dof\_map**](#function-dof_map) () const<br> |
|  std::string | [**dof\_name**](#function-dof_name) (size\_t dof\_index) const<br> |
|  std::vector&lt; std::string &gt; | [**dof\_names**](#function-dof_names) (bool filter\_mimics=false, bool filter\_locked=false, bool filter\_passive=false) const<br> |
|  const std::vector&lt; std::pair&lt; dart::dynamics::BodyNode \*, double &gt; &gt; & | [**drawing\_axes**](#function-drawing_axes) () const<br> |
|  Eigen::Vector6d | [**external\_forces**](#function-external_forces-12) (const std::string & body\_name) const<br> |
|  Eigen::Vector6d | [**external\_forces**](#function-external_forces-22) (size\_t body\_index) const<br> |
|  void | [**fix\_to\_world**](#function-fix_to_world) () <br> |
|  bool | [**fixed**](#function-fixed) () const<br> |
|  Eigen::VectorXd | [**force\_lower\_limits**](#function-force_lower_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  void | [**force\_position\_bounds**](#function-force_position_bounds) () <br> |
|  std::pair&lt; Eigen::Vector6d, Eigen::Vector6d &gt; | [**force\_torque**](#function-force_torque) (size\_t joint\_index) const<br> |
|  Eigen::VectorXd | [**force\_upper\_limits**](#function-force_upper_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**forces**](#function-forces) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  bool | [**free**](#function-free) () const<br> |
|  void | [**free\_from\_world**](#function-free_from_world) (const Eigen::Vector6d & pose=Eigen::Vector6d::Zero()) <br> |
|  double | [**friction\_coeff**](#function-friction_coeff-12) (const std::string & body\_name) <br> |
|  double | [**friction\_coeff**](#function-friction_coeff-22) (size\_t body\_index) <br> |
|  Eigen::Vector3d | [**friction\_dir**](#function-friction_dir-12) (const std::string & body\_name) <br> |
|  Eigen::Vector3d | [**friction\_dir**](#function-friction_dir-22) (size\_t body\_index) <br> |
|  bool | [**ghost**](#function-ghost) () const<br> |
|  Eigen::VectorXd | [**gravity\_forces**](#function-gravity_forces) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::MatrixXd | [**inv\_aug\_mass\_matrix**](#function-inv_aug_mass_matrix) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::MatrixXd | [**inv\_mass\_matrix**](#function-inv_mass_matrix) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::MatrixXd | [**jacobian**](#function-jacobian) (const std::string & body\_name, const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::MatrixXd | [**jacobian\_deriv**](#function-jacobian_deriv) (const std::string & body\_name, const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  dart::dynamics::Joint \* | [**joint**](#function-joint-12) (const std::string & joint\_name) <br> |
|  dart::dynamics::Joint \* | [**joint**](#function-joint-22) (size\_t joint\_index) <br> |
|  size\_t | [**joint\_index**](#function-joint_index) (const std::string & joint\_name) const<br> |
|  const std::unordered\_map&lt; std::string, size\_t &gt; & | [**joint\_map**](#function-joint_map) () const<br> |
|  std::string | [**joint\_name**](#function-joint_name) (size\_t joint\_index) const<br> |
|  std::vector&lt; std::string &gt; | [**joint\_names**](#function-joint_names) () const<br> |
|  std::vector&lt; std::string &gt; | [**locked\_dof\_names**](#function-locked_dof_names) () const<br> |
|  Eigen::MatrixXd | [**mass\_matrix**](#function-mass_matrix) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  std::vector&lt; std::string &gt; | [**mimic\_dof\_names**](#function-mimic_dof_names) () const<br> |
|  const std::string & | [**model\_filename**](#function-model_filename) () const<br> |
|  const std::vector&lt; std::pair&lt; std::string, std::string &gt; &gt; & | [**model\_packages**](#function-model_packages) () const<br> |
|  const std::string & | [**name**](#function-name) () const<br> |
|  size\_t | [**num\_bodies**](#function-num_bodies) () const<br> |
|  size\_t | [**num\_controllers**](#function-num_controllers) () const<br> |
|  size\_t | [**num\_dofs**](#function-num_dofs) () const<br> |
|  size\_t | [**num\_joints**](#function-num_joints) () const<br> |
|  std::vector&lt; std::string &gt; | [**passive\_dof\_names**](#function-passive_dof_names) () const<br> |
|  std::vector&lt; bool &gt; | [**position\_enforced**](#function-position_enforced) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**position\_lower\_limits**](#function-position_lower_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**position\_upper\_limits**](#function-position_upper_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**positions**](#function-positions) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  void | [**reinit\_controllers**](#function-reinit_controllers) () <br> |
|  void | [**remove\_all\_drawing\_axis**](#function-remove_all_drawing_axis) () <br> |
|  void | [**remove\_controller**](#function-remove_controller-12) (const std::shared\_ptr&lt; [**control::RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; & controller) <br> |
|  void | [**remove\_controller**](#function-remove_controller-22) (size\_t index) <br> |
| virtual void | [**reset**](#function-reset) () <br> |
|  void | [**reset\_commands**](#function-reset_commands) () <br> |
|  double | [**restitution\_coeff**](#function-restitution_coeff-12) (const std::string & body\_name) <br> |
|  double | [**restitution\_coeff**](#function-restitution_coeff-22) (size\_t body\_index) <br> |
|  double | [**secondary\_friction\_coeff**](#function-secondary_friction_coeff-12) (const std::string & body\_name) <br> |
|  double | [**secondary\_friction\_coeff**](#function-secondary_friction_coeff-22) (size\_t body\_index) <br> |
|  bool | [**self\_colliding**](#function-self_colliding) () const<br> |
|  void | [**set\_acceleration\_lower\_limits**](#function-set_acceleration_lower_limits) (const Eigen::VectorXd & accelerations, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_acceleration\_upper\_limits**](#function-set_acceleration_upper_limits) (const Eigen::VectorXd & accelerations, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_accelerations**](#function-set_accelerations) (const Eigen::VectorXd & accelerations, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_actuator\_type**](#function-set_actuator_type) (const std::string & type, const std::string & joint\_name, bool override\_mimic=false, bool override\_base=false) <br> |
|  void | [**set\_actuator\_types**](#function-set_actuator_types) (const std::string & type, const std::vector&lt; std::string &gt; & joint\_names={}, bool override\_mimic=false, bool override\_base=false) <br> |
|  void | [**set\_base\_pose**](#function-set_base_pose-12) (const Eigen::Isometry3d & tf) <br> |
|  void | [**set\_base\_pose**](#function-set_base_pose-22) (const Eigen::Vector6d & pose) <br>_set base pose: pose is a 6D vector (first 3D orientation in angle-axis and last 3D translation)_  |
|  void | [**set\_body\_mass**](#function-set_body_mass-12) (const std::string & body\_name, double mass) <br> |
|  void | [**set\_body\_mass**](#function-set_body_mass-22) (size\_t body\_index, double mass) <br> |
|  void | [**set\_body\_name**](#function-set_body_name) (size\_t body\_index, const std::string & body\_name) <br> |
|  void | [**set\_cast\_shadows**](#function-set_cast_shadows) (bool cast\_shadows=true) <br> |
|  void | [**set\_color\_mode**](#function-set_color_mode-12) (const std::string & color\_mode) <br> |
|  void | [**set\_color\_mode**](#function-set_color_mode-22) (const std::string & color\_mode, const std::string & body\_name) <br> |
|  void | [**set\_commands**](#function-set_commands) (const Eigen::VectorXd & commands, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_coulomb\_coeffs**](#function-set_coulomb_coeffs-12) (const std::vector&lt; double &gt; & cfrictions, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_coulomb\_coeffs**](#function-set_coulomb_coeffs-22) (double cfriction, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_damping\_coeffs**](#function-set_damping_coeffs-12) (const std::vector&lt; double &gt; & damps, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_damping\_coeffs**](#function-set_damping_coeffs-22) (double damp, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_draw\_axis**](#function-set_draw_axis) (const std::string & body\_name, double size=0.25) <br> |
|  void | [**set\_external\_force**](#function-set_external_force-12) (const std::string & body\_name, const Eigen::Vector3d & force, const Eigen::Vector3d & offset=Eigen::Vector3d::Zero(), bool force\_local=false, bool offset\_local=true) <br> |
|  void | [**set\_external\_force**](#function-set_external_force-22) (size\_t body\_index, const Eigen::Vector3d & force, const Eigen::Vector3d & offset=Eigen::Vector3d::Zero(), bool force\_local=false, bool offset\_local=true) <br> |
|  void | [**set\_external\_torque**](#function-set_external_torque-12) (const std::string & body\_name, const Eigen::Vector3d & torque, bool local=false) <br> |
|  void | [**set\_external\_torque**](#function-set_external_torque-22) (size\_t body\_index, const Eigen::Vector3d & torque, bool local=false) <br> |
|  void | [**set\_force\_lower\_limits**](#function-set_force_lower_limits) (const Eigen::VectorXd & forces, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_force\_upper\_limits**](#function-set_force_upper_limits) (const Eigen::VectorXd & forces, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_forces**](#function-set_forces) (const Eigen::VectorXd & forces, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_friction\_coeff**](#function-set_friction_coeff-12) (const std::string & body\_name, double value) <br> |
|  void | [**set\_friction\_coeff**](#function-set_friction_coeff-22) (size\_t body\_index, double value) <br> |
|  void | [**set\_friction\_coeffs**](#function-set_friction_coeffs) (double value) <br> |
|  void | [**set\_friction\_dir**](#function-set_friction_dir-12) (const std::string & body\_name, const Eigen::Vector3d & direction) <br> |
|  void | [**set\_friction\_dir**](#function-set_friction_dir-22) (size\_t body\_index, const Eigen::Vector3d & direction) <br> |
|  void | [**set\_ghost**](#function-set_ghost) (bool ghost=true) <br> |
|  void | [**set\_joint\_name**](#function-set_joint_name) (size\_t joint\_index, const std::string & joint\_name) <br> |
|  void | [**set\_mimic**](#function-set_mimic) (const std::string & joint\_name, const std::string & mimic\_joint\_name, double multiplier=1., double offset=0.) <br> |
|  void | [**set\_position\_enforced**](#function-set_position_enforced-12) (const std::vector&lt; bool &gt; & enforced, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_position\_enforced**](#function-set_position_enforced-22) (bool enforced, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_position\_lower\_limits**](#function-set_position_lower_limits) (const Eigen::VectorXd & positions, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_position\_upper\_limits**](#function-set_position_upper_limits) (const Eigen::VectorXd & positions, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_positions**](#function-set_positions) (const Eigen::VectorXd & positions, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_restitution\_coeff**](#function-set_restitution_coeff-12) (const std::string & body\_name, double value) <br> |
|  void | [**set\_restitution\_coeff**](#function-set_restitution_coeff-22) (size\_t body\_index, double value) <br> |
|  void | [**set\_restitution\_coeffs**](#function-set_restitution_coeffs) (double value) <br> |
|  void | [**set\_secondary\_friction\_coeff**](#function-set_secondary_friction_coeff-12) (const std::string & body\_name, double value) <br> |
|  void | [**set\_secondary\_friction\_coeff**](#function-set_secondary_friction_coeff-22) (size\_t body\_index, double value) <br> |
|  void | [**set\_secondary\_friction\_coeffs**](#function-set_secondary_friction_coeffs) (double value) <br> |
|  void | [**set\_self\_collision**](#function-set_self_collision) (bool enable\_self\_collisions=true, bool enable\_adjacent\_collisions=false) <br> |
|  void | [**set\_spring\_stiffnesses**](#function-set_spring_stiffnesses-12) (const std::vector&lt; double &gt; & stiffnesses, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_spring\_stiffnesses**](#function-set_spring_stiffnesses-22) (double stiffness, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_velocities**](#function-set_velocities) (const Eigen::VectorXd & velocities, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_velocity\_lower\_limits**](#function-set_velocity_lower_limits) (const Eigen::VectorXd & velocities, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_velocity\_upper\_limits**](#function-set_velocity_upper_limits) (const Eigen::VectorXd & velocities, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  dart::dynamics::SkeletonPtr | [**skeleton**](#function-skeleton) () <br> |
|  std::vector&lt; double &gt; | [**spring\_stiffnesses**](#function-spring_stiffnesses) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  void | [**update**](#function-update) (double t) <br> |
|  void | [**update\_joint\_dof\_maps**](#function-update_joint_dof_maps) () <br> |
|  Eigen::VectorXd | [**vec\_dof**](#function-vec_dof) (const Eigen::VectorXd & vec, const std::vector&lt; std::string &gt; & dof\_names) const<br> |
|  Eigen::VectorXd | [**velocities**](#function-velocities) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**velocity\_lower\_limits**](#function-velocity_lower_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**velocity\_upper\_limits**](#function-velocity_upper_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
| virtual  | [**~Robot**](#function-robot) () <br> |




## Public Static Functions inherited from robot_dart::Robot

See [robot\_dart::Robot](classrobot__dart_1_1Robot.md)

| Type | Name |
| ---: | :--- |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**create\_box**](#function-create_box-12) (const Eigen::Vector3d & dims, const Eigen::Isometry3d & tf, const std::string & type="free", double mass=1.0, const Eigen::Vector4d & color=dart::Color::Red(1.0), const std::string & box\_name="box") <br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**create\_box**](#function-create_box-22) (const Eigen::Vector3d & dims, const Eigen::Vector6d & pose=Eigen::Vector6d::Zero(), const std::string & type="free", double mass=1.0, const Eigen::Vector4d & color=dart::Color::Red(1.0), const std::string & box\_name="box") <br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**create\_ellipsoid**](#function-create_ellipsoid-12) (const Eigen::Vector3d & dims, const Eigen::Isometry3d & tf, const std::string & type="free", double mass=1.0, const Eigen::Vector4d & color=dart::Color::Red(1.0), const std::string & ellipsoid\_name="ellipsoid") <br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**create\_ellipsoid**](#function-create_ellipsoid-22) (const Eigen::Vector3d & dims, const Eigen::Vector6d & pose=Eigen::Vector6d::Zero(), const std::string & type="free", double mass=1.0, const Eigen::Vector4d & color=dart::Color::Red(1.0), const std::string & ellipsoid\_name="ellipsoid") <br> |












## Protected Attributes inherited from robot_dart::Robot

See [robot\_dart::Robot](classrobot__dart_1_1Robot.md)

| Type | Name |
| ---: | :--- |
|  std::vector&lt; std::pair&lt; dart::dynamics::BodyNode \*, double &gt; &gt; | [**\_axis\_shapes**](#variable-_axis_shapes)  <br> |
|  bool | [**\_cast\_shadows**](#variable-_cast_shadows)  <br> |
|  std::vector&lt; std::shared\_ptr&lt; [**control::RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; &gt; | [**\_controllers**](#variable-_controllers)  <br> |
|  std::unordered\_map&lt; std::string, size\_t &gt; | [**\_dof\_map**](#variable-_dof_map)  <br> |
|  bool | [**\_is\_ghost**](#variable-_is_ghost)  <br> |
|  std::unordered\_map&lt; std::string, size\_t &gt; | [**\_joint\_map**](#variable-_joint_map)  <br> |
|  std::string | [**\_model\_filename**](#variable-_model_filename)  <br> |
|  std::vector&lt; std::pair&lt; std::string, std::string &gt; &gt; | [**\_packages**](#variable-_packages)  <br> |
|  std::string | [**\_robot\_name**](#variable-_robot_name)  <br> |
|  dart::dynamics::SkeletonPtr | [**\_skeleton**](#variable-_skeleton)  <br> |
































## Protected Functions inherited from robot_dart::Robot

See [robot\_dart::Robot](classrobot__dart_1_1Robot.md)

| Type | Name |
| ---: | :--- |
|  dart::dynamics::Joint::ActuatorType | [**\_actuator\_type**](#function-_actuator_type) (size\_t joint\_index) const<br> |
|  std::vector&lt; dart::dynamics::Joint::ActuatorType &gt; | [**\_actuator\_types**](#function-_actuator_types) () const<br> |
|  std::string | [**\_get\_path**](#function-_get_path) (const std::string & filename) const<br> |
|  Eigen::MatrixXd | [**\_jacobian**](#function-_jacobian) (const Eigen::MatrixXd & full\_jacobian, const std::vector&lt; std::string &gt; & dof\_names) const<br> |
|  dart::dynamics::SkeletonPtr | [**\_load\_model**](#function-_load_model) (const std::string & filename, const std::vector&lt; std::pair&lt; std::string, std::string &gt; &gt; & packages=std::vector&lt; std::pair&lt; std::string, std::string &gt; &gt;(), bool is\_urdf\_string=false) <br> |
|  Eigen::MatrixXd | [**\_mass\_matrix**](#function-_mass_matrix) (const Eigen::MatrixXd & full\_mass\_matrix, const std::vector&lt; std::string &gt; & dof\_names) const<br> |
| virtual void | [**\_post\_addition**](#function-_post_addition) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \*) <br>_Function called by_ [_**RobotDARTSimu**_](classrobot__dart_1_1RobotDARTSimu.md) _object when adding the robot to the world._ |
| virtual void | [**\_post\_removal**](#function-_post_removal) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \*) <br>_Function called by_ [_**RobotDARTSimu**_](classrobot__dart_1_1RobotDARTSimu.md) _object when removing the robot to the world._ |
|  void | [**\_set\_actuator\_type**](#function-_set_actuator_type) (size\_t joint\_index, dart::dynamics::Joint::ActuatorType type, bool override\_mimic=false, bool override\_base=false) <br> |
|  void | [**\_set\_actuator\_types**](#function-_set_actuator_types-12) (const std::vector&lt; dart::dynamics::Joint::ActuatorType &gt; & types, bool override\_mimic=false, bool override\_base=false) <br> |
|  void | [**\_set\_actuator\_types**](#function-_set_actuator_types-22) (dart::dynamics::Joint::ActuatorType type, bool override\_mimic=false, bool override\_base=false) <br> |
|  void | [**\_set\_color\_mode**](#function-_set_color_mode-12) (dart::dynamics::MeshShape::ColorMode color\_mode, dart::dynamics::SkeletonPtr skel) <br> |
|  void | [**\_set\_color\_mode**](#function-_set_color_mode-22) (dart::dynamics::MeshShape::ColorMode color\_mode, dart::dynamics::ShapeNode \* sn) <br> |






## Public Functions Documentation




### function Hexapod 

```C++
inline robot_dart::robots::Hexapod::Hexapod (
    const std::string & urdf="pexod.urdf"
) 
```




<hr>



### function reset 

```C++
inline virtual void robot_dart::robots::Hexapod::reset () override
```



Implements [*robot\_dart::Robot::reset*](classrobot__dart_1_1Robot.md#function-reset)


<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/robots/hexapod.hpp`

