

# Class robot\_dart::robots::Talos



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**robots**](namespacerobot__dart_1_1robots.md) **>** [**Talos**](classrobot__dart_1_1robots_1_1Talos.md)



_datasheet:_ [https://pal-robotics.com/wp-content/uploads/2019/07/Datasheet\_TALOS.pdf](https://pal-robotics.com/wp-content/uploads/2019/07/Datasheet_TALOS.pdf) __

* `#include <robot_dart/robots/talos.hpp>`



Inherits the following classes: [robot\_dart::Robot](classrobot__dart_1_1Robot.md)


Inherited by the following classes: [robot\_dart::robots::TalosFastCollision](classrobot__dart_1_1robots_1_1TalosFastCollision.md),  [robot\_dart::robots::TalosLight](classrobot__dart_1_1robots_1_1TalosLight.md)












## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::unordered\_map&lt; std::string, std::shared\_ptr&lt; [**sensor::Torque**](classrobot__dart_1_1sensor_1_1Torque.md) &gt; &gt; | [**torque\_map\_t**](#typedef-torque_map_t)  <br> |








































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Talos**](#function-talos) (size\_t frequency=1000, const std::string & urdf="talos/talos.urdf", const std::vector&lt; std::pair&lt; std::string, std::string &gt; &gt; & packages={{"talos\_description", "talos/talos\_description"}}) <br> |
|  const [**sensor::ForceTorque**](classrobot__dart_1_1sensor_1_1ForceTorque.md) & | [**ft\_foot\_left**](#function-ft_foot_left) () const<br> |
|  const [**sensor::ForceTorque**](classrobot__dart_1_1sensor_1_1ForceTorque.md) & | [**ft\_foot\_right**](#function-ft_foot_right) () const<br> |
|  const [**sensor::ForceTorque**](classrobot__dart_1_1sensor_1_1ForceTorque.md) & | [**ft\_wrist\_left**](#function-ft_wrist_left) () const<br> |
|  const [**sensor::ForceTorque**](classrobot__dart_1_1sensor_1_1ForceTorque.md) & | [**ft\_wrist\_right**](#function-ft_wrist_right) () const<br> |
|  const [**sensor::IMU**](classrobot__dart_1_1sensor_1_1IMU.md) & | [**imu**](#function-imu) () const<br> |
| virtual void | [**reset**](#function-reset) () override<br> |
|  const torque\_map\_t & | [**torques**](#function-torques) () const<br> |


## Public Functions inherited from robot_dart::Robot

See [robot\_dart::Robot](classrobot__dart_1_1Robot.md)

| Type | Name |
| ---: | :--- |
|   | [**Robot**](classrobot__dart_1_1Robot.md#function-robot-13) (const std::string & model\_file, const std::vector&lt; std::pair&lt; std::string, std::string &gt; &gt; & packages, const std::string & robot\_name="robot", bool is\_urdf\_string=false, bool cast\_shadows=true) <br> |
|   | [**Robot**](classrobot__dart_1_1Robot.md#function-robot-23) (const std::string & model\_file, const std::string & robot\_name="robot", bool is\_urdf\_string=false, bool cast\_shadows=true) <br> |
|   | [**Robot**](classrobot__dart_1_1Robot.md#function-robot-33) (dart::dynamics::SkeletonPtr skeleton, const std::string & robot\_name="robot", bool cast\_shadows=true) <br> |
|  Eigen::VectorXd | [**acceleration\_lower\_limits**](classrobot__dart_1_1Robot.md#function-acceleration_lower_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**acceleration\_upper\_limits**](classrobot__dart_1_1Robot.md#function-acceleration_upper_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**accelerations**](classrobot__dart_1_1Robot.md#function-accelerations) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  std::vector&lt; std::shared\_ptr&lt; [**control::RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; &gt; | [**active\_controllers**](classrobot__dart_1_1Robot.md#function-active_controllers) () const<br> |
|  std::string | [**actuator\_type**](classrobot__dart_1_1Robot.md#function-actuator_type) (const std::string & joint\_name) const<br> |
|  std::vector&lt; std::string &gt; | [**actuator\_types**](classrobot__dart_1_1Robot.md#function-actuator_types) (const std::vector&lt; std::string &gt; & joint\_names={}) const<br> |
|  void | [**add\_body\_mass**](classrobot__dart_1_1Robot.md#function-add_body_mass-12) (const std::string & body\_name, double mass) <br> |
|  void | [**add\_body\_mass**](classrobot__dart_1_1Robot.md#function-add_body_mass-22) (size\_t body\_index, double mass) <br> |
|  void | [**add\_controller**](classrobot__dart_1_1Robot.md#function-add_controller) (const std::shared\_ptr&lt; [**control::RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; & controller, double weight=1.0) <br> |
|  void | [**add\_external\_force**](classrobot__dart_1_1Robot.md#function-add_external_force-12) (const std::string & body\_name, const Eigen::Vector3d & force, const Eigen::Vector3d & offset=Eigen::Vector3d::Zero(), bool force\_local=false, bool offset\_local=true) <br> |
|  void | [**add\_external\_force**](classrobot__dart_1_1Robot.md#function-add_external_force-22) (size\_t body\_index, const Eigen::Vector3d & force, const Eigen::Vector3d & offset=Eigen::Vector3d::Zero(), bool force\_local=false, bool offset\_local=true) <br> |
|  void | [**add\_external\_torque**](classrobot__dart_1_1Robot.md#function-add_external_torque-12) (const std::string & body\_name, const Eigen::Vector3d & torque, bool local=false) <br> |
|  void | [**add\_external\_torque**](classrobot__dart_1_1Robot.md#function-add_external_torque-22) (size\_t body\_index, const Eigen::Vector3d & torque, bool local=false) <br> |
|  bool | [**adjacent\_colliding**](classrobot__dart_1_1Robot.md#function-adjacent_colliding) () const<br> |
|  Eigen::MatrixXd | [**aug\_mass\_matrix**](classrobot__dart_1_1Robot.md#function-aug_mass_matrix) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::Isometry3d | [**base\_pose**](classrobot__dart_1_1Robot.md#function-base_pose) () const<br> |
|  Eigen::Vector6d | [**base\_pose\_vec**](classrobot__dart_1_1Robot.md#function-base_pose_vec) () const<br> |
|  Eigen::Vector6d | [**body\_acceleration**](classrobot__dart_1_1Robot.md#function-body_acceleration-12) (const std::string & body\_name) const<br> |
|  Eigen::Vector6d | [**body\_acceleration**](classrobot__dart_1_1Robot.md#function-body_acceleration-22) (size\_t body\_index) const<br> |
|  size\_t | [**body\_index**](classrobot__dart_1_1Robot.md#function-body_index) (const std::string & body\_name) const<br> |
|  double | [**body\_mass**](classrobot__dart_1_1Robot.md#function-body_mass-12) (const std::string & body\_name) const<br> |
|  double | [**body\_mass**](classrobot__dart_1_1Robot.md#function-body_mass-22) (size\_t body\_index) const<br> |
|  std::string | [**body\_name**](classrobot__dart_1_1Robot.md#function-body_name) (size\_t body\_index) const<br> |
|  std::vector&lt; std::string &gt; | [**body\_names**](classrobot__dart_1_1Robot.md#function-body_names) () const<br> |
|  dart::dynamics::BodyNode \* | [**body\_node**](classrobot__dart_1_1Robot.md#function-body_node-12) (const std::string & body\_name) <br> |
|  dart::dynamics::BodyNode \* | [**body\_node**](classrobot__dart_1_1Robot.md#function-body_node-22) (size\_t body\_index) <br> |
|  Eigen::Isometry3d | [**body\_pose**](classrobot__dart_1_1Robot.md#function-body_pose-12) (const std::string & body\_name) const<br> |
|  Eigen::Isometry3d | [**body\_pose**](classrobot__dart_1_1Robot.md#function-body_pose-22) (size\_t body\_index) const<br> |
|  Eigen::Vector6d | [**body\_pose\_vec**](classrobot__dart_1_1Robot.md#function-body_pose_vec-12) (const std::string & body\_name) const<br> |
|  Eigen::Vector6d | [**body\_pose\_vec**](classrobot__dart_1_1Robot.md#function-body_pose_vec-22) (size\_t body\_index) const<br> |
|  Eigen::Vector6d | [**body\_velocity**](classrobot__dart_1_1Robot.md#function-body_velocity-12) (const std::string & body\_name) const<br> |
|  Eigen::Vector6d | [**body\_velocity**](classrobot__dart_1_1Robot.md#function-body_velocity-22) (size\_t body\_index) const<br> |
|  bool | [**cast\_shadows**](classrobot__dart_1_1Robot.md#function-cast_shadows) () const<br> |
|  void | [**clear\_controllers**](classrobot__dart_1_1Robot.md#function-clear_controllers) () <br> |
|  void | [**clear\_external\_forces**](classrobot__dart_1_1Robot.md#function-clear_external_forces) () <br> |
|  void | [**clear\_internal\_forces**](classrobot__dart_1_1Robot.md#function-clear_internal_forces) () <br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**clone**](classrobot__dart_1_1Robot.md#function-clone) () const<br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**clone\_ghost**](classrobot__dart_1_1Robot.md#function-clone_ghost) (const std::string & ghost\_name="ghost", const Eigen::Vector4d & ghost\_color={0.3, 0.3, 0.3, 0.7}) const<br> |
|  Eigen::Vector3d | [**com**](classrobot__dart_1_1Robot.md#function-com) () const<br> |
|  Eigen::Vector6d | [**com\_acceleration**](classrobot__dart_1_1Robot.md#function-com_acceleration) () const<br> |
|  Eigen::MatrixXd | [**com\_jacobian**](classrobot__dart_1_1Robot.md#function-com_jacobian) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::MatrixXd | [**com\_jacobian\_deriv**](classrobot__dart_1_1Robot.md#function-com_jacobian_deriv) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::Vector6d | [**com\_velocity**](classrobot__dart_1_1Robot.md#function-com_velocity) () const<br> |
|  Eigen::VectorXd | [**commands**](classrobot__dart_1_1Robot.md#function-commands) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**constraint\_forces**](classrobot__dart_1_1Robot.md#function-constraint_forces) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  std::shared\_ptr&lt; [**control::RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; | [**controller**](classrobot__dart_1_1Robot.md#function-controller) (size\_t index) const<br> |
|  std::vector&lt; std::shared\_ptr&lt; [**control::RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; &gt; | [**controllers**](classrobot__dart_1_1Robot.md#function-controllers) () const<br> |
|  Eigen::VectorXd | [**coriolis\_forces**](classrobot__dart_1_1Robot.md#function-coriolis_forces) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**coriolis\_gravity\_forces**](classrobot__dart_1_1Robot.md#function-coriolis_gravity_forces) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  std::vector&lt; double &gt; | [**coulomb\_coeffs**](classrobot__dart_1_1Robot.md#function-coulomb_coeffs) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  std::vector&lt; double &gt; | [**damping\_coeffs**](classrobot__dart_1_1Robot.md#function-damping_coeffs) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  dart::dynamics::DegreeOfFreedom \* | [**dof**](classrobot__dart_1_1Robot.md#function-dof-12) (const std::string & dof\_name) <br> |
|  dart::dynamics::DegreeOfFreedom \* | [**dof**](classrobot__dart_1_1Robot.md#function-dof-22) (size\_t dof\_index) <br> |
|  size\_t | [**dof\_index**](classrobot__dart_1_1Robot.md#function-dof_index) (const std::string & dof\_name) const<br> |
|  const std::unordered\_map&lt; std::string, size\_t &gt; & | [**dof\_map**](classrobot__dart_1_1Robot.md#function-dof_map) () const<br> |
|  std::string | [**dof\_name**](classrobot__dart_1_1Robot.md#function-dof_name) (size\_t dof\_index) const<br> |
|  std::vector&lt; std::string &gt; | [**dof\_names**](classrobot__dart_1_1Robot.md#function-dof_names) (bool filter\_mimics=false, bool filter\_locked=false, bool filter\_passive=false) const<br> |
|  const std::vector&lt; std::pair&lt; dart::dynamics::BodyNode \*, double &gt; &gt; & | [**drawing\_axes**](classrobot__dart_1_1Robot.md#function-drawing_axes) () const<br> |
|  Eigen::Vector6d | [**external\_forces**](classrobot__dart_1_1Robot.md#function-external_forces-12) (const std::string & body\_name) const<br> |
|  Eigen::Vector6d | [**external\_forces**](classrobot__dart_1_1Robot.md#function-external_forces-22) (size\_t body\_index) const<br> |
|  void | [**fix\_to\_world**](classrobot__dart_1_1Robot.md#function-fix_to_world) () <br> |
|  bool | [**fixed**](classrobot__dart_1_1Robot.md#function-fixed) () const<br> |
|  Eigen::VectorXd | [**force\_lower\_limits**](classrobot__dart_1_1Robot.md#function-force_lower_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  void | [**force\_position\_bounds**](classrobot__dart_1_1Robot.md#function-force_position_bounds) () <br> |
|  std::pair&lt; Eigen::Vector6d, Eigen::Vector6d &gt; | [**force\_torque**](classrobot__dart_1_1Robot.md#function-force_torque) (size\_t joint\_index) const<br> |
|  Eigen::VectorXd | [**force\_upper\_limits**](classrobot__dart_1_1Robot.md#function-force_upper_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**forces**](classrobot__dart_1_1Robot.md#function-forces) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  bool | [**free**](classrobot__dart_1_1Robot.md#function-free) () const<br> |
|  void | [**free\_from\_world**](classrobot__dart_1_1Robot.md#function-free_from_world) (const Eigen::Vector6d & pose=Eigen::Vector6d::Zero()) <br> |
|  double | [**friction\_coeff**](classrobot__dart_1_1Robot.md#function-friction_coeff-12) (const std::string & body\_name) <br> |
|  double | [**friction\_coeff**](classrobot__dart_1_1Robot.md#function-friction_coeff-22) (size\_t body\_index) <br> |
|  Eigen::Vector3d | [**friction\_dir**](classrobot__dart_1_1Robot.md#function-friction_dir-12) (const std::string & body\_name) <br> |
|  Eigen::Vector3d | [**friction\_dir**](classrobot__dart_1_1Robot.md#function-friction_dir-22) (size\_t body\_index) <br> |
|  bool | [**ghost**](classrobot__dart_1_1Robot.md#function-ghost) () const<br> |
|  Eigen::VectorXd | [**gravity\_forces**](classrobot__dart_1_1Robot.md#function-gravity_forces) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::MatrixXd | [**inv\_aug\_mass\_matrix**](classrobot__dart_1_1Robot.md#function-inv_aug_mass_matrix) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::MatrixXd | [**inv\_mass\_matrix**](classrobot__dart_1_1Robot.md#function-inv_mass_matrix) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::MatrixXd | [**jacobian**](classrobot__dart_1_1Robot.md#function-jacobian) (const std::string & body\_name, const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::MatrixXd | [**jacobian\_deriv**](classrobot__dart_1_1Robot.md#function-jacobian_deriv) (const std::string & body\_name, const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  dart::dynamics::Joint \* | [**joint**](classrobot__dart_1_1Robot.md#function-joint-12) (const std::string & joint\_name) <br> |
|  dart::dynamics::Joint \* | [**joint**](classrobot__dart_1_1Robot.md#function-joint-22) (size\_t joint\_index) <br> |
|  size\_t | [**joint\_index**](classrobot__dart_1_1Robot.md#function-joint_index) (const std::string & joint\_name) const<br> |
|  const std::unordered\_map&lt; std::string, size\_t &gt; & | [**joint\_map**](classrobot__dart_1_1Robot.md#function-joint_map) () const<br> |
|  std::string | [**joint\_name**](classrobot__dart_1_1Robot.md#function-joint_name) (size\_t joint\_index) const<br> |
|  std::vector&lt; std::string &gt; | [**joint\_names**](classrobot__dart_1_1Robot.md#function-joint_names) () const<br> |
|  std::vector&lt; std::string &gt; | [**locked\_dof\_names**](classrobot__dart_1_1Robot.md#function-locked_dof_names) () const<br> |
|  Eigen::MatrixXd | [**mass\_matrix**](classrobot__dart_1_1Robot.md#function-mass_matrix) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  std::vector&lt; std::string &gt; | [**mimic\_dof\_names**](classrobot__dart_1_1Robot.md#function-mimic_dof_names) () const<br> |
|  const std::string & | [**model\_filename**](classrobot__dart_1_1Robot.md#function-model_filename) () const<br> |
|  const std::vector&lt; std::pair&lt; std::string, std::string &gt; &gt; & | [**model\_packages**](classrobot__dart_1_1Robot.md#function-model_packages) () const<br> |
|  const std::string & | [**name**](classrobot__dart_1_1Robot.md#function-name) () const<br> |
|  size\_t | [**num\_bodies**](classrobot__dart_1_1Robot.md#function-num_bodies) () const<br> |
|  size\_t | [**num\_controllers**](classrobot__dart_1_1Robot.md#function-num_controllers) () const<br> |
|  size\_t | [**num\_dofs**](classrobot__dart_1_1Robot.md#function-num_dofs) () const<br> |
|  size\_t | [**num\_joints**](classrobot__dart_1_1Robot.md#function-num_joints) () const<br> |
|  std::vector&lt; std::string &gt; | [**passive\_dof\_names**](classrobot__dart_1_1Robot.md#function-passive_dof_names) () const<br> |
|  std::vector&lt; bool &gt; | [**position\_enforced**](classrobot__dart_1_1Robot.md#function-position_enforced) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**position\_lower\_limits**](classrobot__dart_1_1Robot.md#function-position_lower_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**position\_upper\_limits**](classrobot__dart_1_1Robot.md#function-position_upper_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**positions**](classrobot__dart_1_1Robot.md#function-positions) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  void | [**reinit\_controllers**](classrobot__dart_1_1Robot.md#function-reinit_controllers) () <br> |
|  void | [**remove\_all\_drawing\_axis**](classrobot__dart_1_1Robot.md#function-remove_all_drawing_axis) () <br> |
|  void | [**remove\_controller**](classrobot__dart_1_1Robot.md#function-remove_controller-12) (const std::shared\_ptr&lt; [**control::RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; & controller) <br> |
|  void | [**remove\_controller**](classrobot__dart_1_1Robot.md#function-remove_controller-22) (size\_t index) <br> |
| virtual void | [**reset**](classrobot__dart_1_1Robot.md#function-reset) () <br> |
|  void | [**reset\_commands**](classrobot__dart_1_1Robot.md#function-reset_commands) () <br> |
|  double | [**restitution\_coeff**](classrobot__dart_1_1Robot.md#function-restitution_coeff-12) (const std::string & body\_name) <br> |
|  double | [**restitution\_coeff**](classrobot__dart_1_1Robot.md#function-restitution_coeff-22) (size\_t body\_index) <br> |
|  double | [**secondary\_friction\_coeff**](classrobot__dart_1_1Robot.md#function-secondary_friction_coeff-12) (const std::string & body\_name) <br> |
|  double | [**secondary\_friction\_coeff**](classrobot__dart_1_1Robot.md#function-secondary_friction_coeff-22) (size\_t body\_index) <br> |
|  bool | [**self\_colliding**](classrobot__dart_1_1Robot.md#function-self_colliding) () const<br> |
|  void | [**set\_acceleration\_lower\_limits**](classrobot__dart_1_1Robot.md#function-set_acceleration_lower_limits) (const Eigen::VectorXd & accelerations, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_acceleration\_upper\_limits**](classrobot__dart_1_1Robot.md#function-set_acceleration_upper_limits) (const Eigen::VectorXd & accelerations, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_accelerations**](classrobot__dart_1_1Robot.md#function-set_accelerations) (const Eigen::VectorXd & accelerations, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_actuator\_type**](classrobot__dart_1_1Robot.md#function-set_actuator_type) (const std::string & type, const std::string & joint\_name, bool override\_mimic=false, bool override\_base=false) <br> |
|  void | [**set\_actuator\_types**](classrobot__dart_1_1Robot.md#function-set_actuator_types) (const std::string & type, const std::vector&lt; std::string &gt; & joint\_names={}, bool override\_mimic=false, bool override\_base=false) <br> |
|  void | [**set\_base\_pose**](classrobot__dart_1_1Robot.md#function-set_base_pose-12) (const Eigen::Isometry3d & tf) <br> |
|  void | [**set\_base\_pose**](classrobot__dart_1_1Robot.md#function-set_base_pose-22) (const Eigen::Vector6d & pose) <br>_set base pose: pose is a 6D vector (first 3D orientation in angle-axis and last 3D translation)_  |
|  void | [**set\_body\_mass**](classrobot__dart_1_1Robot.md#function-set_body_mass-12) (const std::string & body\_name, double mass) <br> |
|  void | [**set\_body\_mass**](classrobot__dart_1_1Robot.md#function-set_body_mass-22) (size\_t body\_index, double mass) <br> |
|  void | [**set\_body\_name**](classrobot__dart_1_1Robot.md#function-set_body_name) (size\_t body\_index, const std::string & body\_name) <br> |
|  void | [**set\_cast\_shadows**](classrobot__dart_1_1Robot.md#function-set_cast_shadows) (bool cast\_shadows=true) <br> |
|  void | [**set\_color\_mode**](classrobot__dart_1_1Robot.md#function-set_color_mode-12) (const std::string & color\_mode) <br> |
|  void | [**set\_color\_mode**](classrobot__dart_1_1Robot.md#function-set_color_mode-22) (const std::string & color\_mode, const std::string & body\_name) <br> |
|  void | [**set\_commands**](classrobot__dart_1_1Robot.md#function-set_commands) (const Eigen::VectorXd & commands, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_coulomb\_coeffs**](classrobot__dart_1_1Robot.md#function-set_coulomb_coeffs-12) (const std::vector&lt; double &gt; & cfrictions, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_coulomb\_coeffs**](classrobot__dart_1_1Robot.md#function-set_coulomb_coeffs-22) (double cfriction, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_damping\_coeffs**](classrobot__dart_1_1Robot.md#function-set_damping_coeffs-12) (const std::vector&lt; double &gt; & damps, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_damping\_coeffs**](classrobot__dart_1_1Robot.md#function-set_damping_coeffs-22) (double damp, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_draw\_axis**](classrobot__dart_1_1Robot.md#function-set_draw_axis) (const std::string & body\_name, double size=0.25) <br> |
|  void | [**set\_external\_force**](classrobot__dart_1_1Robot.md#function-set_external_force-12) (const std::string & body\_name, const Eigen::Vector3d & force, const Eigen::Vector3d & offset=Eigen::Vector3d::Zero(), bool force\_local=false, bool offset\_local=true) <br> |
|  void | [**set\_external\_force**](classrobot__dart_1_1Robot.md#function-set_external_force-22) (size\_t body\_index, const Eigen::Vector3d & force, const Eigen::Vector3d & offset=Eigen::Vector3d::Zero(), bool force\_local=false, bool offset\_local=true) <br> |
|  void | [**set\_external\_torque**](classrobot__dart_1_1Robot.md#function-set_external_torque-12) (const std::string & body\_name, const Eigen::Vector3d & torque, bool local=false) <br> |
|  void | [**set\_external\_torque**](classrobot__dart_1_1Robot.md#function-set_external_torque-22) (size\_t body\_index, const Eigen::Vector3d & torque, bool local=false) <br> |
|  void | [**set\_force\_lower\_limits**](classrobot__dart_1_1Robot.md#function-set_force_lower_limits) (const Eigen::VectorXd & forces, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_force\_upper\_limits**](classrobot__dart_1_1Robot.md#function-set_force_upper_limits) (const Eigen::VectorXd & forces, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_forces**](classrobot__dart_1_1Robot.md#function-set_forces) (const Eigen::VectorXd & forces, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_friction\_coeff**](classrobot__dart_1_1Robot.md#function-set_friction_coeff-12) (const std::string & body\_name, double value) <br> |
|  void | [**set\_friction\_coeff**](classrobot__dart_1_1Robot.md#function-set_friction_coeff-22) (size\_t body\_index, double value) <br> |
|  void | [**set\_friction\_coeffs**](classrobot__dart_1_1Robot.md#function-set_friction_coeffs) (double value) <br> |
|  void | [**set\_friction\_dir**](classrobot__dart_1_1Robot.md#function-set_friction_dir-12) (const std::string & body\_name, const Eigen::Vector3d & direction) <br> |
|  void | [**set\_friction\_dir**](classrobot__dart_1_1Robot.md#function-set_friction_dir-22) (size\_t body\_index, const Eigen::Vector3d & direction) <br> |
|  void | [**set\_ghost**](classrobot__dart_1_1Robot.md#function-set_ghost) (bool ghost=true) <br> |
|  void | [**set\_joint\_name**](classrobot__dart_1_1Robot.md#function-set_joint_name) (size\_t joint\_index, const std::string & joint\_name) <br> |
|  void | [**set\_mimic**](classrobot__dart_1_1Robot.md#function-set_mimic) (const std::string & joint\_name, const std::string & mimic\_joint\_name, double multiplier=1., double offset=0.) <br> |
|  void | [**set\_position\_enforced**](classrobot__dart_1_1Robot.md#function-set_position_enforced-12) (const std::vector&lt; bool &gt; & enforced, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_position\_enforced**](classrobot__dart_1_1Robot.md#function-set_position_enforced-22) (bool enforced, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_position\_lower\_limits**](classrobot__dart_1_1Robot.md#function-set_position_lower_limits) (const Eigen::VectorXd & positions, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_position\_upper\_limits**](classrobot__dart_1_1Robot.md#function-set_position_upper_limits) (const Eigen::VectorXd & positions, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_positions**](classrobot__dart_1_1Robot.md#function-set_positions) (const Eigen::VectorXd & positions, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_restitution\_coeff**](classrobot__dart_1_1Robot.md#function-set_restitution_coeff-12) (const std::string & body\_name, double value) <br> |
|  void | [**set\_restitution\_coeff**](classrobot__dart_1_1Robot.md#function-set_restitution_coeff-22) (size\_t body\_index, double value) <br> |
|  void | [**set\_restitution\_coeffs**](classrobot__dart_1_1Robot.md#function-set_restitution_coeffs) (double value) <br> |
|  void | [**set\_secondary\_friction\_coeff**](classrobot__dart_1_1Robot.md#function-set_secondary_friction_coeff-12) (const std::string & body\_name, double value) <br> |
|  void | [**set\_secondary\_friction\_coeff**](classrobot__dart_1_1Robot.md#function-set_secondary_friction_coeff-22) (size\_t body\_index, double value) <br> |
|  void | [**set\_secondary\_friction\_coeffs**](classrobot__dart_1_1Robot.md#function-set_secondary_friction_coeffs) (double value) <br> |
|  void | [**set\_self\_collision**](classrobot__dart_1_1Robot.md#function-set_self_collision) (bool enable\_self\_collisions=true, bool enable\_adjacent\_collisions=false) <br> |
|  void | [**set\_spring\_stiffnesses**](classrobot__dart_1_1Robot.md#function-set_spring_stiffnesses-12) (const std::vector&lt; double &gt; & stiffnesses, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_spring\_stiffnesses**](classrobot__dart_1_1Robot.md#function-set_spring_stiffnesses-22) (double stiffness, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_velocities**](classrobot__dart_1_1Robot.md#function-set_velocities) (const Eigen::VectorXd & velocities, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_velocity\_lower\_limits**](classrobot__dart_1_1Robot.md#function-set_velocity_lower_limits) (const Eigen::VectorXd & velocities, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  void | [**set\_velocity\_upper\_limits**](classrobot__dart_1_1Robot.md#function-set_velocity_upper_limits) (const Eigen::VectorXd & velocities, const std::vector&lt; std::string &gt; & dof\_names={}) <br> |
|  dart::dynamics::SkeletonPtr | [**skeleton**](classrobot__dart_1_1Robot.md#function-skeleton) () <br> |
|  std::vector&lt; double &gt; | [**spring\_stiffnesses**](classrobot__dart_1_1Robot.md#function-spring_stiffnesses) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  void | [**update**](classrobot__dart_1_1Robot.md#function-update) (double t) <br> |
|  void | [**update\_joint\_dof\_maps**](classrobot__dart_1_1Robot.md#function-update_joint_dof_maps) () <br> |
|  Eigen::VectorXd | [**vec\_dof**](classrobot__dart_1_1Robot.md#function-vec_dof) (const Eigen::VectorXd & vec, const std::vector&lt; std::string &gt; & dof\_names) const<br> |
|  Eigen::VectorXd | [**velocities**](classrobot__dart_1_1Robot.md#function-velocities) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**velocity\_lower\_limits**](classrobot__dart_1_1Robot.md#function-velocity_lower_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
|  Eigen::VectorXd | [**velocity\_upper\_limits**](classrobot__dart_1_1Robot.md#function-velocity_upper_limits) (const std::vector&lt; std::string &gt; & dof\_names={}) const<br> |
| virtual  | [**~Robot**](classrobot__dart_1_1Robot.md#function-robot) () <br> |




## Public Static Functions inherited from robot_dart::Robot

See [robot\_dart::Robot](classrobot__dart_1_1Robot.md)

| Type | Name |
| ---: | :--- |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**create\_box**](classrobot__dart_1_1Robot.md#function-create_box-12) (const Eigen::Vector3d & dims, const Eigen::Isometry3d & tf, const std::string & type="free", double mass=1.0, const Eigen::Vector4d & color=dart::Color::Red(1.0), const std::string & box\_name="box") <br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**create\_box**](classrobot__dart_1_1Robot.md#function-create_box-22) (const Eigen::Vector3d & dims, const Eigen::Vector6d & pose=Eigen::Vector6d::Zero(), const std::string & type="free", double mass=1.0, const Eigen::Vector4d & color=dart::Color::Red(1.0), const std::string & box\_name="box") <br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**create\_ellipsoid**](classrobot__dart_1_1Robot.md#function-create_ellipsoid-12) (const Eigen::Vector3d & dims, const Eigen::Isometry3d & tf, const std::string & type="free", double mass=1.0, const Eigen::Vector4d & color=dart::Color::Red(1.0), const std::string & ellipsoid\_name="ellipsoid") <br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**create\_ellipsoid**](classrobot__dart_1_1Robot.md#function-create_ellipsoid-22) (const Eigen::Vector3d & dims, const Eigen::Vector6d & pose=Eigen::Vector6d::Zero(), const std::string & type="free", double mass=1.0, const Eigen::Vector4d & color=dart::Color::Red(1.0), const std::string & ellipsoid\_name="ellipsoid") <br> |










## Protected Attributes

| Type | Name |
| ---: | :--- |
|  size\_t | [**\_frequency**](#variable-_frequency)  <br> |
|  std::shared\_ptr&lt; [**sensor::ForceTorque**](classrobot__dart_1_1sensor_1_1ForceTorque.md) &gt; | [**\_ft\_foot\_left**](#variable-_ft_foot_left)  <br> |
|  std::shared\_ptr&lt; [**sensor::ForceTorque**](classrobot__dart_1_1sensor_1_1ForceTorque.md) &gt; | [**\_ft\_foot\_right**](#variable-_ft_foot_right)  <br> |
|  std::shared\_ptr&lt; [**sensor::ForceTorque**](classrobot__dart_1_1sensor_1_1ForceTorque.md) &gt; | [**\_ft\_wrist\_left**](#variable-_ft_wrist_left)  <br> |
|  std::shared\_ptr&lt; [**sensor::ForceTorque**](classrobot__dart_1_1sensor_1_1ForceTorque.md) &gt; | [**\_ft\_wrist\_right**](#variable-_ft_wrist_right)  <br> |
|  std::shared\_ptr&lt; [**sensor::IMU**](classrobot__dart_1_1sensor_1_1IMU.md) &gt; | [**\_imu**](#variable-_imu)  <br> |
|  torque\_map\_t | [**\_torques**](#variable-_torques)  <br> |


## Protected Attributes inherited from robot_dart::Robot

See [robot\_dart::Robot](classrobot__dart_1_1Robot.md)

| Type | Name |
| ---: | :--- |
|  std::vector&lt; std::pair&lt; dart::dynamics::BodyNode \*, double &gt; &gt; | [**\_axis\_shapes**](classrobot__dart_1_1Robot.md#variable-_axis_shapes)  <br> |
|  bool | [**\_cast\_shadows**](classrobot__dart_1_1Robot.md#variable-_cast_shadows)  <br> |
|  std::vector&lt; std::shared\_ptr&lt; [**control::RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; &gt; | [**\_controllers**](classrobot__dart_1_1Robot.md#variable-_controllers)  <br> |
|  std::unordered\_map&lt; std::string, size\_t &gt; | [**\_dof\_map**](classrobot__dart_1_1Robot.md#variable-_dof_map)  <br> |
|  bool | [**\_is\_ghost**](classrobot__dart_1_1Robot.md#variable-_is_ghost)  <br> |
|  std::unordered\_map&lt; std::string, size\_t &gt; | [**\_joint\_map**](classrobot__dart_1_1Robot.md#variable-_joint_map)  <br> |
|  std::string | [**\_model\_filename**](classrobot__dart_1_1Robot.md#variable-_model_filename)  <br> |
|  std::vector&lt; std::pair&lt; std::string, std::string &gt; &gt; | [**\_packages**](classrobot__dart_1_1Robot.md#variable-_packages)  <br> |
|  std::string | [**\_robot\_name**](classrobot__dart_1_1Robot.md#variable-_robot_name)  <br> |
|  dart::dynamics::SkeletonPtr | [**\_skeleton**](classrobot__dart_1_1Robot.md#variable-_skeleton)  <br> |






























## Protected Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**\_post\_addition**](#function-_post_addition) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \*) override<br>_Function called by_ [_**RobotDARTSimu**_](classrobot__dart_1_1RobotDARTSimu.md) _object when adding the robot to the world._ |
| virtual void | [**\_post\_removal**](#function-_post_removal) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \*) override<br>_Function called by_ [_**RobotDARTSimu**_](classrobot__dart_1_1RobotDARTSimu.md) _object when removing the robot to the world._ |


## Protected Functions inherited from robot_dart::Robot

See [robot\_dart::Robot](classrobot__dart_1_1Robot.md)

| Type | Name |
| ---: | :--- |
|  dart::dynamics::Joint::ActuatorType | [**\_actuator\_type**](classrobot__dart_1_1Robot.md#function-_actuator_type) (size\_t joint\_index) const<br> |
|  std::vector&lt; dart::dynamics::Joint::ActuatorType &gt; | [**\_actuator\_types**](classrobot__dart_1_1Robot.md#function-_actuator_types) () const<br> |
|  std::string | [**\_get\_path**](classrobot__dart_1_1Robot.md#function-_get_path) (const std::string & filename) const<br> |
|  Eigen::MatrixXd | [**\_jacobian**](classrobot__dart_1_1Robot.md#function-_jacobian) (const Eigen::MatrixXd & full\_jacobian, const std::vector&lt; std::string &gt; & dof\_names) const<br> |
|  dart::dynamics::SkeletonPtr | [**\_load\_model**](classrobot__dart_1_1Robot.md#function-_load_model) (const std::string & filename, const std::vector&lt; std::pair&lt; std::string, std::string &gt; &gt; & packages=std::vector&lt; std::pair&lt; std::string, std::string &gt; &gt;(), bool is\_urdf\_string=false) <br> |
|  Eigen::MatrixXd | [**\_mass\_matrix**](classrobot__dart_1_1Robot.md#function-_mass_matrix) (const Eigen::MatrixXd & full\_mass\_matrix, const std::vector&lt; std::string &gt; & dof\_names) const<br> |
| virtual void | [**\_post\_addition**](classrobot__dart_1_1Robot.md#function-_post_addition) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \*) <br>_Function called by_ [_**RobotDARTSimu**_](classrobot__dart_1_1RobotDARTSimu.md) _object when adding the robot to the world._ |
| virtual void | [**\_post\_removal**](classrobot__dart_1_1Robot.md#function-_post_removal) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \*) <br>_Function called by_ [_**RobotDARTSimu**_](classrobot__dart_1_1RobotDARTSimu.md) _object when removing the robot to the world._ |
|  void | [**\_set\_actuator\_type**](classrobot__dart_1_1Robot.md#function-_set_actuator_type) (size\_t joint\_index, dart::dynamics::Joint::ActuatorType type, bool override\_mimic=false, bool override\_base=false) <br> |
|  void | [**\_set\_actuator\_types**](classrobot__dart_1_1Robot.md#function-_set_actuator_types-12) (const std::vector&lt; dart::dynamics::Joint::ActuatorType &gt; & types, bool override\_mimic=false, bool override\_base=false) <br> |
|  void | [**\_set\_actuator\_types**](classrobot__dart_1_1Robot.md#function-_set_actuator_types-22) (dart::dynamics::Joint::ActuatorType type, bool override\_mimic=false, bool override\_base=false) <br> |
|  void | [**\_set\_color\_mode**](classrobot__dart_1_1Robot.md#function-_set_color_mode-12) (dart::dynamics::MeshShape::ColorMode color\_mode, dart::dynamics::SkeletonPtr skel) <br> |
|  void | [**\_set\_color\_mode**](classrobot__dart_1_1Robot.md#function-_set_color_mode-22) (dart::dynamics::MeshShape::ColorMode color\_mode, dart::dynamics::ShapeNode \* sn) <br> |






## Public Types Documentation




### typedef torque\_map\_t 

```C++
using robot_dart::robots::Talos::torque_map_t =  std::unordered_map<std::string, std::shared_ptr<sensor::Torque>>;
```




<hr>
## Public Functions Documentation




### function Talos 

```C++
robot_dart::robots::Talos::Talos (
    size_t frequency=1000,
    const std::string & urdf="talos/talos.urdf",
    const std::vector< std::pair< std::string, std::string > > & packages={{"talos_description", "talos/talos_description"}}
) 
```




<hr>



### function ft\_foot\_left 

```C++
inline const sensor::ForceTorque & robot_dart::robots::Talos::ft_foot_left () const
```




<hr>



### function ft\_foot\_right 

```C++
inline const sensor::ForceTorque & robot_dart::robots::Talos::ft_foot_right () const
```




<hr>



### function ft\_wrist\_left 

```C++
inline const sensor::ForceTorque & robot_dart::robots::Talos::ft_wrist_left () const
```




<hr>



### function ft\_wrist\_right 

```C++
inline const sensor::ForceTorque & robot_dart::robots::Talos::ft_wrist_right () const
```




<hr>



### function imu 

```C++
inline const sensor::IMU & robot_dart::robots::Talos::imu () const
```




<hr>



### function reset 

```C++
virtual void robot_dart::robots::Talos::reset () override
```



Implements [*robot\_dart::Robot::reset*](classrobot__dart_1_1Robot.md#function-reset)


<hr>



### function torques 

```C++
inline const torque_map_t & robot_dart::robots::Talos::torques () const
```




<hr>
## Protected Attributes Documentation




### variable \_frequency 

```C++
size_t robot_dart::robots::Talos::_frequency;
```




<hr>



### variable \_ft\_foot\_left 

```C++
std::shared_ptr<sensor::ForceTorque> robot_dart::robots::Talos::_ft_foot_left;
```




<hr>



### variable \_ft\_foot\_right 

```C++
std::shared_ptr<sensor::ForceTorque> robot_dart::robots::Talos::_ft_foot_right;
```




<hr>



### variable \_ft\_wrist\_left 

```C++
std::shared_ptr<sensor::ForceTorque> robot_dart::robots::Talos::_ft_wrist_left;
```




<hr>



### variable \_ft\_wrist\_right 

```C++
std::shared_ptr<sensor::ForceTorque> robot_dart::robots::Talos::_ft_wrist_right;
```




<hr>



### variable \_imu 

```C++
std::shared_ptr<sensor::IMU> robot_dart::robots::Talos::_imu;
```




<hr>



### variable \_torques 

```C++
torque_map_t robot_dart::robots::Talos::_torques;
```




<hr>
## Protected Functions Documentation




### function \_post\_addition 

```C++
virtual void robot_dart::robots::Talos::_post_addition (
    RobotDARTSimu *
) override
```



Implements [*robot\_dart::Robot::\_post\_addition*](classrobot__dart_1_1Robot.md#function-_post_addition)


<hr>



### function \_post\_removal 

```C++
virtual void robot_dart::robots::Talos::_post_removal (
    RobotDARTSimu *
) override
```



Implements [*robot\_dart::Robot::\_post\_removal*](classrobot__dart_1_1Robot.md#function-_post_removal)


<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/robots/talos.hpp`

