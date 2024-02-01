

# Class robot\_dart::Robot



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**Robot**](classrobot__dart_1_1Robot.md)








Inherits the following classes: std::enable_shared_from_this< Robot >


Inherited by the following classes: [robot\_dart::robots::A1](classrobot__dart_1_1robots_1_1A1.md),  [robot\_dart::robots::Arm](classrobot__dart_1_1robots_1_1Arm.md),  [robot\_dart::robots::Franka](classrobot__dart_1_1robots_1_1Franka.md),  [robot\_dart::robots::Hexapod](classrobot__dart_1_1robots_1_1Hexapod.md),  [robot\_dart::robots::ICub](classrobot__dart_1_1robots_1_1ICub.md),  [robot\_dart::robots::Iiwa](classrobot__dart_1_1robots_1_1Iiwa.md),  [robot\_dart::robots::Pendulum](classrobot__dart_1_1robots_1_1Pendulum.md),  [robot\_dart::robots::Talos](classrobot__dart_1_1robots_1_1Talos.md),  [robot\_dart::robots::Tiago](classrobot__dart_1_1robots_1_1Tiago.md),  [robot\_dart::robots::Ur3e](classrobot__dart_1_1robots_1_1Ur3e.md),  [robot\_dart::robots::Vx300](classrobot__dart_1_1robots_1_1Vx300.md)
































## Public Functions

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


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**create\_box**](#function-create_box-12) (const Eigen::Vector3d & dims, const Eigen::Isometry3d & tf, const std::string & type="free", double mass=1.0, const Eigen::Vector4d & color=dart::Color::Red(1.0), const std::string & box\_name="box") <br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**create\_box**](#function-create_box-22) (const Eigen::Vector3d & dims, const Eigen::Vector6d & pose=Eigen::Vector6d::Zero(), const std::string & type="free", double mass=1.0, const Eigen::Vector4d & color=dart::Color::Red(1.0), const std::string & box\_name="box") <br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**create\_ellipsoid**](#function-create_ellipsoid-12) (const Eigen::Vector3d & dims, const Eigen::Isometry3d & tf, const std::string & type="free", double mass=1.0, const Eigen::Vector4d & color=dart::Color::Red(1.0), const std::string & ellipsoid\_name="ellipsoid") <br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**create\_ellipsoid**](#function-create_ellipsoid-22) (const Eigen::Vector3d & dims, const Eigen::Vector6d & pose=Eigen::Vector6d::Zero(), const std::string & type="free", double mass=1.0, const Eigen::Vector4d & color=dart::Color::Red(1.0), const std::string & ellipsoid\_name="ellipsoid") <br> |






## Protected Attributes

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
















## Protected Functions

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




### function Robot [1/3]

```C++
robot_dart::Robot::Robot (
    const std::string & model_file,
    const std::vector< std::pair< std::string, std::string > > & packages,
    const std::string & robot_name="robot",
    bool is_urdf_string=false,
    bool cast_shadows=true
) 
```






### function Robot [2/3]

```C++
robot_dart::Robot::Robot (
    const std::string & model_file,
    const std::string & robot_name="robot",
    bool is_urdf_string=false,
    bool cast_shadows=true
) 
```






### function Robot [3/3]

```C++
robot_dart::Robot::Robot (
    dart::dynamics::SkeletonPtr skeleton,
    const std::string & robot_name="robot",
    bool cast_shadows=true
) 
```






### function acceleration\_lower\_limits 

```C++
Eigen::VectorXd robot_dart::Robot::acceleration_lower_limits (
    const std::vector< std::string > & dof_names={}
) const
```






### function acceleration\_upper\_limits 

```C++
Eigen::VectorXd robot_dart::Robot::acceleration_upper_limits (
    const std::vector< std::string > & dof_names={}
) const
```






### function accelerations 

```C++
Eigen::VectorXd robot_dart::Robot::accelerations (
    const std::vector< std::string > & dof_names={}
) const
```






### function active\_controllers 

```C++
std::vector< std::shared_ptr< control::RobotControl > > robot_dart::Robot::active_controllers () const
```






### function actuator\_type 

```C++
std::string robot_dart::Robot::actuator_type (
    const std::string & joint_name
) const
```






### function actuator\_types 

```C++
std::vector< std::string > robot_dart::Robot::actuator_types (
    const std::vector< std::string > & joint_names={}
) const
```






### function add\_body\_mass [1/2]

```C++
void robot_dart::Robot::add_body_mass (
    const std::string & body_name,
    double mass
) 
```






### function add\_body\_mass [2/2]

```C++
void robot_dart::Robot::add_body_mass (
    size_t body_index,
    double mass
) 
```






### function add\_controller 

```C++
void robot_dart::Robot::add_controller (
    const std::shared_ptr< control::RobotControl > & controller,
    double weight=1.0
) 
```






### function add\_external\_force [1/2]

```C++
void robot_dart::Robot::add_external_force (
    const std::string & body_name,
    const Eigen::Vector3d & force,
    const Eigen::Vector3d & offset=Eigen::Vector3d::Zero(),
    bool force_local=false,
    bool offset_local=true
) 
```






### function add\_external\_force [2/2]

```C++
void robot_dart::Robot::add_external_force (
    size_t body_index,
    const Eigen::Vector3d & force,
    const Eigen::Vector3d & offset=Eigen::Vector3d::Zero(),
    bool force_local=false,
    bool offset_local=true
) 
```






### function add\_external\_torque [1/2]

```C++
void robot_dart::Robot::add_external_torque (
    const std::string & body_name,
    const Eigen::Vector3d & torque,
    bool local=false
) 
```






### function add\_external\_torque [2/2]

```C++
void robot_dart::Robot::add_external_torque (
    size_t body_index,
    const Eigen::Vector3d & torque,
    bool local=false
) 
```






### function adjacent\_colliding 

```C++
bool robot_dart::Robot::adjacent_colliding () const
```






### function aug\_mass\_matrix 

```C++
Eigen::MatrixXd robot_dart::Robot::aug_mass_matrix (
    const std::vector< std::string > & dof_names={}
) const
```






### function base\_pose 

```C++
Eigen::Isometry3d robot_dart::Robot::base_pose () const
```






### function base\_pose\_vec 

```C++
Eigen::Vector6d robot_dart::Robot::base_pose_vec () const
```






### function body\_acceleration [1/2]

```C++
Eigen::Vector6d robot_dart::Robot::body_acceleration (
    const std::string & body_name
) const
```






### function body\_acceleration [2/2]

```C++
Eigen::Vector6d robot_dart::Robot::body_acceleration (
    size_t body_index
) const
```






### function body\_index 

```C++
size_t robot_dart::Robot::body_index (
    const std::string & body_name
) const
```






### function body\_mass [1/2]

```C++
double robot_dart::Robot::body_mass (
    const std::string & body_name
) const
```






### function body\_mass [2/2]

```C++
double robot_dart::Robot::body_mass (
    size_t body_index
) const
```






### function body\_name 

```C++
std::string robot_dart::Robot::body_name (
    size_t body_index
) const
```






### function body\_names 

```C++
std::vector< std::string > robot_dart::Robot::body_names () const
```






### function body\_node [1/2]

```C++
dart::dynamics::BodyNode * robot_dart::Robot::body_node (
    const std::string & body_name
) 
```






### function body\_node [2/2]

```C++
dart::dynamics::BodyNode * robot_dart::Robot::body_node (
    size_t body_index
) 
```






### function body\_pose [1/2]

```C++
Eigen::Isometry3d robot_dart::Robot::body_pose (
    const std::string & body_name
) const
```






### function body\_pose [2/2]

```C++
Eigen::Isometry3d robot_dart::Robot::body_pose (
    size_t body_index
) const
```






### function body\_pose\_vec [1/2]

```C++
Eigen::Vector6d robot_dart::Robot::body_pose_vec (
    const std::string & body_name
) const
```






### function body\_pose\_vec [2/2]

```C++
Eigen::Vector6d robot_dart::Robot::body_pose_vec (
    size_t body_index
) const
```






### function body\_velocity [1/2]

```C++
Eigen::Vector6d robot_dart::Robot::body_velocity (
    const std::string & body_name
) const
```






### function body\_velocity [2/2]

```C++
Eigen::Vector6d robot_dart::Robot::body_velocity (
    size_t body_index
) const
```






### function cast\_shadows 

```C++
bool robot_dart::Robot::cast_shadows () const
```






### function clear\_controllers 

```C++
void robot_dart::Robot::clear_controllers () 
```






### function clear\_external\_forces 

```C++
void robot_dart::Robot::clear_external_forces () 
```






### function clear\_internal\_forces 

```C++
void robot_dart::Robot::clear_internal_forces () 
```






### function clone 

```C++
std::shared_ptr< Robot > robot_dart::Robot::clone () const
```






### function clone\_ghost 

```C++
std::shared_ptr< Robot > robot_dart::Robot::clone_ghost (
    const std::string & ghost_name="ghost",
    const Eigen::Vector4d & ghost_color={0.3, 0.3, 0.3, 0.7}
) const
```






### function com 

```C++
Eigen::Vector3d robot_dart::Robot::com () const
```






### function com\_acceleration 

```C++
Eigen::Vector6d robot_dart::Robot::com_acceleration () const
```






### function com\_jacobian 

```C++
Eigen::MatrixXd robot_dart::Robot::com_jacobian (
    const std::vector< std::string > & dof_names={}
) const
```






### function com\_jacobian\_deriv 

```C++
Eigen::MatrixXd robot_dart::Robot::com_jacobian_deriv (
    const std::vector< std::string > & dof_names={}
) const
```






### function com\_velocity 

```C++
Eigen::Vector6d robot_dart::Robot::com_velocity () const
```






### function commands 

```C++
Eigen::VectorXd robot_dart::Robot::commands (
    const std::vector< std::string > & dof_names={}
) const
```






### function constraint\_forces 

```C++
Eigen::VectorXd robot_dart::Robot::constraint_forces (
    const std::vector< std::string > & dof_names={}
) const
```






### function controller 

```C++
std::shared_ptr< control::RobotControl > robot_dart::Robot::controller (
    size_t index
) const
```






### function controllers 

```C++
std::vector< std::shared_ptr< control::RobotControl > > robot_dart::Robot::controllers () const
```






### function coriolis\_forces 

```C++
Eigen::VectorXd robot_dart::Robot::coriolis_forces (
    const std::vector< std::string > & dof_names={}
) const
```






### function coriolis\_gravity\_forces 

```C++
Eigen::VectorXd robot_dart::Robot::coriolis_gravity_forces (
    const std::vector< std::string > & dof_names={}
) const
```






### function coulomb\_coeffs 

```C++
std::vector< double > robot_dart::Robot::coulomb_coeffs (
    const std::vector< std::string > & dof_names={}
) const
```






### function damping\_coeffs 

```C++
std::vector< double > robot_dart::Robot::damping_coeffs (
    const std::vector< std::string > & dof_names={}
) const
```






### function dof [1/2]

```C++
dart::dynamics::DegreeOfFreedom * robot_dart::Robot::dof (
    const std::string & dof_name
) 
```






### function dof [2/2]

```C++
dart::dynamics::DegreeOfFreedom * robot_dart::Robot::dof (
    size_t dof_index
) 
```






### function dof\_index 

```C++
size_t robot_dart::Robot::dof_index (
    const std::string & dof_name
) const
```






### function dof\_map 

```C++
const std::unordered_map< std::string, size_t > & robot_dart::Robot::dof_map () const
```






### function dof\_name 

```C++
std::string robot_dart::Robot::dof_name (
    size_t dof_index
) const
```






### function dof\_names 

```C++
std::vector< std::string > robot_dart::Robot::dof_names (
    bool filter_mimics=false,
    bool filter_locked=false,
    bool filter_passive=false
) const
```






### function drawing\_axes 

```C++
const std::vector< std::pair< dart::dynamics::BodyNode *, double > > & robot_dart::Robot::drawing_axes () const
```






### function external\_forces [1/2]

```C++
Eigen::Vector6d robot_dart::Robot::external_forces (
    const std::string & body_name
) const
```






### function external\_forces [2/2]

```C++
Eigen::Vector6d robot_dart::Robot::external_forces (
    size_t body_index
) const
```






### function fix\_to\_world 

```C++
void robot_dart::Robot::fix_to_world () 
```






### function fixed 

```C++
bool robot_dart::Robot::fixed () const
```






### function force\_lower\_limits 

```C++
Eigen::VectorXd robot_dart::Robot::force_lower_limits (
    const std::vector< std::string > & dof_names={}
) const
```






### function force\_position\_bounds 

```C++
void robot_dart::Robot::force_position_bounds () 
```






### function force\_torque 

```C++
std::pair< Eigen::Vector6d, Eigen::Vector6d > robot_dart::Robot::force_torque (
    size_t joint_index
) const
```






### function force\_upper\_limits 

```C++
Eigen::VectorXd robot_dart::Robot::force_upper_limits (
    const std::vector< std::string > & dof_names={}
) const
```






### function forces 

```C++
Eigen::VectorXd robot_dart::Robot::forces (
    const std::vector< std::string > & dof_names={}
) const
```






### function free 

```C++
bool robot_dart::Robot::free () const
```






### function free\_from\_world 

```C++
void robot_dart::Robot::free_from_world (
    const Eigen::Vector6d & pose=Eigen::Vector6d::Zero()
) 
```






### function friction\_coeff [1/2]

```C++
double robot_dart::Robot::friction_coeff (
    const std::string & body_name
) 
```






### function friction\_coeff [2/2]

```C++
double robot_dart::Robot::friction_coeff (
    size_t body_index
) 
```






### function friction\_dir [1/2]

```C++
Eigen::Vector3d robot_dart::Robot::friction_dir (
    const std::string & body_name
) 
```






### function friction\_dir [2/2]

```C++
Eigen::Vector3d robot_dart::Robot::friction_dir (
    size_t body_index
) 
```






### function ghost 

```C++
bool robot_dart::Robot::ghost () const
```






### function gravity\_forces 

```C++
Eigen::VectorXd robot_dart::Robot::gravity_forces (
    const std::vector< std::string > & dof_names={}
) const
```






### function inv\_aug\_mass\_matrix 

```C++
Eigen::MatrixXd robot_dart::Robot::inv_aug_mass_matrix (
    const std::vector< std::string > & dof_names={}
) const
```






### function inv\_mass\_matrix 

```C++
Eigen::MatrixXd robot_dart::Robot::inv_mass_matrix (
    const std::vector< std::string > & dof_names={}
) const
```






### function jacobian 

```C++
Eigen::MatrixXd robot_dart::Robot::jacobian (
    const std::string & body_name,
    const std::vector< std::string > & dof_names={}
) const
```






### function jacobian\_deriv 

```C++
Eigen::MatrixXd robot_dart::Robot::jacobian_deriv (
    const std::string & body_name,
    const std::vector< std::string > & dof_names={}
) const
```






### function joint [1/2]

```C++
dart::dynamics::Joint * robot_dart::Robot::joint (
    const std::string & joint_name
) 
```






### function joint [2/2]

```C++
dart::dynamics::Joint * robot_dart::Robot::joint (
    size_t joint_index
) 
```






### function joint\_index 

```C++
size_t robot_dart::Robot::joint_index (
    const std::string & joint_name
) const
```






### function joint\_map 

```C++
const std::unordered_map< std::string, size_t > & robot_dart::Robot::joint_map () const
```






### function joint\_name 

```C++
std::string robot_dart::Robot::joint_name (
    size_t joint_index
) const
```






### function joint\_names 

```C++
std::vector< std::string > robot_dart::Robot::joint_names () const
```






### function locked\_dof\_names 

```C++
std::vector< std::string > robot_dart::Robot::locked_dof_names () const
```






### function mass\_matrix 

```C++
Eigen::MatrixXd robot_dart::Robot::mass_matrix (
    const std::vector< std::string > & dof_names={}
) const
```






### function mimic\_dof\_names 

```C++
std::vector< std::string > robot_dart::Robot::mimic_dof_names () const
```






### function model\_filename 

```C++
inline const std::string & robot_dart::Robot::model_filename () const
```






### function model\_packages 

```C++
inline const std::vector< std::pair< std::string, std::string > > & robot_dart::Robot::model_packages () const
```






### function name 

```C++
const std::string & robot_dart::Robot::name () const
```






### function num\_bodies 

```C++
size_t robot_dart::Robot::num_bodies () const
```






### function num\_controllers 

```C++
size_t robot_dart::Robot::num_controllers () const
```






### function num\_dofs 

```C++
size_t robot_dart::Robot::num_dofs () const
```






### function num\_joints 

```C++
size_t robot_dart::Robot::num_joints () const
```






### function passive\_dof\_names 

```C++
std::vector< std::string > robot_dart::Robot::passive_dof_names () const
```






### function position\_enforced 

```C++
std::vector< bool > robot_dart::Robot::position_enforced (
    const std::vector< std::string > & dof_names={}
) const
```






### function position\_lower\_limits 

```C++
Eigen::VectorXd robot_dart::Robot::position_lower_limits (
    const std::vector< std::string > & dof_names={}
) const
```






### function position\_upper\_limits 

```C++
Eigen::VectorXd robot_dart::Robot::position_upper_limits (
    const std::vector< std::string > & dof_names={}
) const
```






### function positions 

```C++
Eigen::VectorXd robot_dart::Robot::positions (
    const std::vector< std::string > & dof_names={}
) const
```






### function reinit\_controllers 

```C++
void robot_dart::Robot::reinit_controllers () 
```






### function remove\_all\_drawing\_axis 

```C++
void robot_dart::Robot::remove_all_drawing_axis () 
```






### function remove\_controller [1/2]

```C++
void robot_dart::Robot::remove_controller (
    const std::shared_ptr< control::RobotControl > & controller
) 
```






### function remove\_controller [2/2]

```C++
void robot_dart::Robot::remove_controller (
    size_t index
) 
```






### function reset 

```C++
virtual void robot_dart::Robot::reset () 
```






### function reset\_commands 

```C++
void robot_dart::Robot::reset_commands () 
```






### function restitution\_coeff [1/2]

```C++
double robot_dart::Robot::restitution_coeff (
    const std::string & body_name
) 
```






### function restitution\_coeff [2/2]

```C++
double robot_dart::Robot::restitution_coeff (
    size_t body_index
) 
```






### function secondary\_friction\_coeff [1/2]

```C++
double robot_dart::Robot::secondary_friction_coeff (
    const std::string & body_name
) 
```






### function secondary\_friction\_coeff [2/2]

```C++
double robot_dart::Robot::secondary_friction_coeff (
    size_t body_index
) 
```






### function self\_colliding 

```C++
bool robot_dart::Robot::self_colliding () const
```






### function set\_acceleration\_lower\_limits 

```C++
void robot_dart::Robot::set_acceleration_lower_limits (
    const Eigen::VectorXd & accelerations,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_acceleration\_upper\_limits 

```C++
void robot_dart::Robot::set_acceleration_upper_limits (
    const Eigen::VectorXd & accelerations,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_accelerations 

```C++
void robot_dart::Robot::set_accelerations (
    const Eigen::VectorXd & accelerations,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_actuator\_type 

```C++
void robot_dart::Robot::set_actuator_type (
    const std::string & type,
    const std::string & joint_name,
    bool override_mimic=false,
    bool override_base=false
) 
```






### function set\_actuator\_types 

```C++
void robot_dart::Robot::set_actuator_types (
    const std::string & type,
    const std::vector< std::string > & joint_names={},
    bool override_mimic=false,
    bool override_base=false
) 
```






### function set\_base\_pose [1/2]

```C++
void robot_dart::Robot::set_base_pose (
    const Eigen::Isometry3d & tf
) 
```






### function set\_base\_pose [2/2]

```C++
void robot_dart::Robot::set_base_pose (
    const Eigen::Vector6d & pose
) 
```






### function set\_body\_mass [1/2]

```C++
void robot_dart::Robot::set_body_mass (
    const std::string & body_name,
    double mass
) 
```






### function set\_body\_mass [2/2]

```C++
void robot_dart::Robot::set_body_mass (
    size_t body_index,
    double mass
) 
```






### function set\_body\_name 

```C++
void robot_dart::Robot::set_body_name (
    size_t body_index,
    const std::string & body_name
) 
```






### function set\_cast\_shadows 

```C++
void robot_dart::Robot::set_cast_shadows (
    bool cast_shadows=true
) 
```






### function set\_color\_mode [1/2]

```C++
void robot_dart::Robot::set_color_mode (
    const std::string & color_mode
) 
```






### function set\_color\_mode [2/2]

```C++
void robot_dart::Robot::set_color_mode (
    const std::string & color_mode,
    const std::string & body_name
) 
```






### function set\_commands 

```C++
void robot_dart::Robot::set_commands (
    const Eigen::VectorXd & commands,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_coulomb\_coeffs [1/2]

```C++
void robot_dart::Robot::set_coulomb_coeffs (
    const std::vector< double > & cfrictions,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_coulomb\_coeffs [2/2]

```C++
void robot_dart::Robot::set_coulomb_coeffs (
    double cfriction,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_damping\_coeffs [1/2]

```C++
void robot_dart::Robot::set_damping_coeffs (
    const std::vector< double > & damps,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_damping\_coeffs [2/2]

```C++
void robot_dart::Robot::set_damping_coeffs (
    double damp,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_draw\_axis 

```C++
void robot_dart::Robot::set_draw_axis (
    const std::string & body_name,
    double size=0.25
) 
```






### function set\_external\_force [1/2]

```C++
void robot_dart::Robot::set_external_force (
    const std::string & body_name,
    const Eigen::Vector3d & force,
    const Eigen::Vector3d & offset=Eigen::Vector3d::Zero(),
    bool force_local=false,
    bool offset_local=true
) 
```






### function set\_external\_force [2/2]

```C++
void robot_dart::Robot::set_external_force (
    size_t body_index,
    const Eigen::Vector3d & force,
    const Eigen::Vector3d & offset=Eigen::Vector3d::Zero(),
    bool force_local=false,
    bool offset_local=true
) 
```






### function set\_external\_torque [1/2]

```C++
void robot_dart::Robot::set_external_torque (
    const std::string & body_name,
    const Eigen::Vector3d & torque,
    bool local=false
) 
```






### function set\_external\_torque [2/2]

```C++
void robot_dart::Robot::set_external_torque (
    size_t body_index,
    const Eigen::Vector3d & torque,
    bool local=false
) 
```






### function set\_force\_lower\_limits 

```C++
void robot_dart::Robot::set_force_lower_limits (
    const Eigen::VectorXd & forces,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_force\_upper\_limits 

```C++
void robot_dart::Robot::set_force_upper_limits (
    const Eigen::VectorXd & forces,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_forces 

```C++
void robot_dart::Robot::set_forces (
    const Eigen::VectorXd & forces,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_friction\_coeff [1/2]

```C++
void robot_dart::Robot::set_friction_coeff (
    const std::string & body_name,
    double value
) 
```






### function set\_friction\_coeff [2/2]

```C++
void robot_dart::Robot::set_friction_coeff (
    size_t body_index,
    double value
) 
```






### function set\_friction\_coeffs 

```C++
void robot_dart::Robot::set_friction_coeffs (
    double value
) 
```






### function set\_friction\_dir [1/2]

```C++
void robot_dart::Robot::set_friction_dir (
    const std::string & body_name,
    const Eigen::Vector3d & direction
) 
```






### function set\_friction\_dir [2/2]

```C++
void robot_dart::Robot::set_friction_dir (
    size_t body_index,
    const Eigen::Vector3d & direction
) 
```






### function set\_ghost 

```C++
void robot_dart::Robot::set_ghost (
    bool ghost=true
) 
```






### function set\_joint\_name 

```C++
void robot_dart::Robot::set_joint_name (
    size_t joint_index,
    const std::string & joint_name
) 
```






### function set\_mimic 

```C++
void robot_dart::Robot::set_mimic (
    const std::string & joint_name,
    const std::string & mimic_joint_name,
    double multiplier=1.,
    double offset=0.
) 
```






### function set\_position\_enforced [1/2]

```C++
void robot_dart::Robot::set_position_enforced (
    const std::vector< bool > & enforced,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_position\_enforced [2/2]

```C++
void robot_dart::Robot::set_position_enforced (
    bool enforced,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_position\_lower\_limits 

```C++
void robot_dart::Robot::set_position_lower_limits (
    const Eigen::VectorXd & positions,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_position\_upper\_limits 

```C++
void robot_dart::Robot::set_position_upper_limits (
    const Eigen::VectorXd & positions,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_positions 

```C++
void robot_dart::Robot::set_positions (
    const Eigen::VectorXd & positions,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_restitution\_coeff [1/2]

```C++
void robot_dart::Robot::set_restitution_coeff (
    const std::string & body_name,
    double value
) 
```






### function set\_restitution\_coeff [2/2]

```C++
void robot_dart::Robot::set_restitution_coeff (
    size_t body_index,
    double value
) 
```






### function set\_restitution\_coeffs 

```C++
void robot_dart::Robot::set_restitution_coeffs (
    double value
) 
```






### function set\_secondary\_friction\_coeff [1/2]

```C++
void robot_dart::Robot::set_secondary_friction_coeff (
    const std::string & body_name,
    double value
) 
```






### function set\_secondary\_friction\_coeff [2/2]

```C++
void robot_dart::Robot::set_secondary_friction_coeff (
    size_t body_index,
    double value
) 
```






### function set\_secondary\_friction\_coeffs 

```C++
void robot_dart::Robot::set_secondary_friction_coeffs (
    double value
) 
```






### function set\_self\_collision 

```C++
void robot_dart::Robot::set_self_collision (
    bool enable_self_collisions=true,
    bool enable_adjacent_collisions=false
) 
```






### function set\_spring\_stiffnesses [1/2]

```C++
void robot_dart::Robot::set_spring_stiffnesses (
    const std::vector< double > & stiffnesses,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_spring\_stiffnesses [2/2]

```C++
void robot_dart::Robot::set_spring_stiffnesses (
    double stiffness,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_velocities 

```C++
void robot_dart::Robot::set_velocities (
    const Eigen::VectorXd & velocities,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_velocity\_lower\_limits 

```C++
void robot_dart::Robot::set_velocity_lower_limits (
    const Eigen::VectorXd & velocities,
    const std::vector< std::string > & dof_names={}
) 
```






### function set\_velocity\_upper\_limits 

```C++
void robot_dart::Robot::set_velocity_upper_limits (
    const Eigen::VectorXd & velocities,
    const std::vector< std::string > & dof_names={}
) 
```






### function skeleton 

```C++
dart::dynamics::SkeletonPtr robot_dart::Robot::skeleton () 
```






### function spring\_stiffnesses 

```C++
std::vector< double > robot_dart::Robot::spring_stiffnesses (
    const std::vector< std::string > & dof_names={}
) const
```






### function update 

```C++
void robot_dart::Robot::update (
    double t
) 
```






### function update\_joint\_dof\_maps 

```C++
void robot_dart::Robot::update_joint_dof_maps () 
```






### function vec\_dof 

```C++
Eigen::VectorXd robot_dart::Robot::vec_dof (
    const Eigen::VectorXd & vec,
    const std::vector< std::string > & dof_names
) const
```






### function velocities 

```C++
Eigen::VectorXd robot_dart::Robot::velocities (
    const std::vector< std::string > & dof_names={}
) const
```






### function velocity\_lower\_limits 

```C++
Eigen::VectorXd robot_dart::Robot::velocity_lower_limits (
    const std::vector< std::string > & dof_names={}
) const
```






### function velocity\_upper\_limits 

```C++
Eigen::VectorXd robot_dart::Robot::velocity_upper_limits (
    const std::vector< std::string > & dof_names={}
) const
```






### function ~Robot 

```C++
inline virtual robot_dart::Robot::~Robot () 
```



## Public Static Functions Documentation




### function create\_box [1/2]

```C++
static std::shared_ptr< Robot > robot_dart::Robot::create_box (
    const Eigen::Vector3d & dims,
    const Eigen::Isometry3d & tf,
    const std::string & type="free",
    double mass=1.0,
    const Eigen::Vector4d & color=dart::Color::Red(1.0),
    const std::string & box_name="box"
) 
```






### function create\_box [2/2]

```C++
static std::shared_ptr< Robot > robot_dart::Robot::create_box (
    const Eigen::Vector3d & dims,
    const Eigen::Vector6d & pose=Eigen::Vector6d::Zero(),
    const std::string & type="free",
    double mass=1.0,
    const Eigen::Vector4d & color=dart::Color::Red(1.0),
    const std::string & box_name="box"
) 
```






### function create\_ellipsoid [1/2]

```C++
static std::shared_ptr< Robot > robot_dart::Robot::create_ellipsoid (
    const Eigen::Vector3d & dims,
    const Eigen::Isometry3d & tf,
    const std::string & type="free",
    double mass=1.0,
    const Eigen::Vector4d & color=dart::Color::Red(1.0),
    const std::string & ellipsoid_name="ellipsoid"
) 
```






### function create\_ellipsoid [2/2]

```C++
static std::shared_ptr< Robot > robot_dart::Robot::create_ellipsoid (
    const Eigen::Vector3d & dims,
    const Eigen::Vector6d & pose=Eigen::Vector6d::Zero(),
    const std::string & type="free",
    double mass=1.0,
    const Eigen::Vector4d & color=dart::Color::Red(1.0),
    const std::string & ellipsoid_name="ellipsoid"
) 
```



## Protected Attributes Documentation




### variable \_axis\_shapes 

```C++
std::vector<std::pair<dart::dynamics::BodyNode*, double> > robot_dart::Robot::_axis_shapes;
```






### variable \_cast\_shadows 

```C++
bool robot_dart::Robot::_cast_shadows;
```






### variable \_controllers 

```C++
std::vector<std::shared_ptr<control::RobotControl> > robot_dart::Robot::_controllers;
```






### variable \_dof\_map 

```C++
std::unordered_map<std::string, size_t> robot_dart::Robot::_dof_map;
```






### variable \_is\_ghost 

```C++
bool robot_dart::Robot::_is_ghost;
```






### variable \_joint\_map 

```C++
std::unordered_map<std::string, size_t> robot_dart::Robot::_joint_map;
```






### variable \_model\_filename 

```C++
std::string robot_dart::Robot::_model_filename;
```






### variable \_packages 

```C++
std::vector<std::pair<std::string, std::string> > robot_dart::Robot::_packages;
```






### variable \_robot\_name 

```C++
std::string robot_dart::Robot::_robot_name;
```






### variable \_skeleton 

```C++
dart::dynamics::SkeletonPtr robot_dart::Robot::_skeleton;
```



## Protected Functions Documentation




### function \_actuator\_type 

```C++
dart::dynamics::Joint::ActuatorType robot_dart::Robot::_actuator_type (
    size_t joint_index
) const
```






### function \_actuator\_types 

```C++
std::vector< dart::dynamics::Joint::ActuatorType > robot_dart::Robot::_actuator_types () const
```






### function \_get\_path 

```C++
std::string robot_dart::Robot::_get_path (
    const std::string & filename
) const
```






### function \_jacobian 

```C++
Eigen::MatrixXd robot_dart::Robot::_jacobian (
    const Eigen::MatrixXd & full_jacobian,
    const std::vector< std::string > & dof_names
) const
```






### function \_load\_model 

```C++
dart::dynamics::SkeletonPtr robot_dart::Robot::_load_model (
    const std::string & filename,
    const std::vector< std::pair< std::string, std::string > > & packages=std::vector< std::pair< std::string, std::string > >(),
    bool is_urdf_string=false
) 
```






### function \_mass\_matrix 

```C++
Eigen::MatrixXd robot_dart::Robot::_mass_matrix (
    const Eigen::MatrixXd & full_mass_matrix,
    const std::vector< std::string > & dof_names
) const
```






### function \_post\_addition 

```C++
inline virtual void robot_dart::Robot::_post_addition (
    RobotDARTSimu *
) 
```






### function \_post\_removal 

```C++
inline virtual void robot_dart::Robot::_post_removal (
    RobotDARTSimu *
) 
```






### function \_set\_actuator\_type 

```C++
void robot_dart::Robot::_set_actuator_type (
    size_t joint_index,
    dart::dynamics::Joint::ActuatorType type,
    bool override_mimic=false,
    bool override_base=false
) 
```






### function \_set\_actuator\_types [1/2]

```C++
void robot_dart::Robot::_set_actuator_types (
    const std::vector< dart::dynamics::Joint::ActuatorType > & types,
    bool override_mimic=false,
    bool override_base=false
) 
```






### function \_set\_actuator\_types [2/2]

```C++
void robot_dart::Robot::_set_actuator_types (
    dart::dynamics::Joint::ActuatorType type,
    bool override_mimic=false,
    bool override_base=false
) 
```






### function \_set\_color\_mode [1/2]

```C++
void robot_dart::Robot::_set_color_mode (
    dart::dynamics::MeshShape::ColorMode color_mode,
    dart::dynamics::SkeletonPtr skel
) 
```






### function \_set\_color\_mode [2/2]

```C++
void robot_dart::Robot::_set_color_mode (
    dart::dynamics::MeshShape::ColorMode color_mode,
    dart::dynamics::ShapeNode * sn
) 
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/robot.hpp`

