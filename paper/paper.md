---
title: 'RobotDART: A Fast and Modular Robot Simulator for Robotics and Machine Learning Researchers'
tags:
  - Robot simulator
  - Robotics
  - Machine Learning
authors:
  - name: Konstantinos Chatzilygeroudis
    orcid: 0000-0003-3585-1027
    equal-contrib: false
    corresponding: true
    affiliation: "1, 2" # (Multiple affiliations must be quoted)
  - name: Dionis Totsila
    orcid: 0009-0008-0664-2676
    equal-contrib: false
    corresponding: false
    affiliation: "3, 4"
  - name: Jean-Baptiste Mouret
    orcid: 0000-0002-2513-027X
    corresponding: false
    affiliation: 3
affiliations:
 - name: Laboratory of Automation and Robotics (LAR), Department of Electrical & Computer Engineering, University of Patras, Greece
   index: 1
 - name: Computational Intelligence Lab (CILab), Department of Mathematics, University of Patras, Greece
   index: 2
 - name: Université de Lorraine, CNRS, Inria, LORIA, F-54000 Nancy, France
   index: 3
 - name: Computer Engineering and Informatics Department (CEID), University of Patras, Greece
   index: 4
date: 2 February 2024
bibliography: paper.bib

---

# Summary

Robot simulation plays a pivotal role in robotics and machine learning research, offering a cost-effective and safe means to develop, validate, and benchmark algorithms in various scenarios. With the growing complexity of robotic systems and the increasing demand for data-driven approaches in machine learning, there is a pressing need for versatile and efficient robot simulators that cater to the diverse requirements of researchers. In response to this demand, we introduce RobotDART, a high-performance and modular robot simulator designed to empower researchers in robotics and machine learning with a powerful and flexible simulation environment.

# Motivation
Simulators play a crucial role in robotics by providing a safe environment for testing without the risk of damaging real robots. Within the robotics community, existing simulators primarily prioritize seamless deployment on physical robots, which is evident in their design philosophy. One of the best examples is Gazebo \cite{}, which is heavily inspired by ROS and it can leverages it as an abstraction level so that users can seamlessly transition between simulated and real robots. Because of this design choice, Gazebo: (1) is mostly asynchronous, which means that the simulator operates on its own clock and time-step analogous to real-world robots, (2) introduces a significant overhead, including network communication and serialization to topics/services/messages, and (3) is not inheritly designed for running multiple simulations concurrently. At a lower abstraction level, the robotics community has developed various physics libraries such as Mujoco \cite{}, Bullet \cite{}, and ODE \cite{}. However, these libraries typically lack sensor abstractions, particularly for cameras, and do not come with pre-validated robot models, necessitating users to program their own simulators.

With the emergence of reinforcement learning for robot control and more generally robot learning, there arises a need for simulators that can address novel requirements not adequately addressed by current simulators or "low-level" physics libraries. Specifically, simulators designed for robot learning must encompass the following features:

- beyond real-time simulation speed: Given the necessity to execute thousands or even millions of simulations, both controllers and simulators stand to benefit from operating at speeds faster than real-time;
- overhead-free and fast reset: with the imperative to conduct numerous simulations efficiently, minimizing time introduced by the communication overheads and the initialization processes becomes paramount;
- synchronous operation: ensuring reproducible learning runs and effectively managing faster-than-reality simulations necessitates synchronization of simulation and controller time-steps;
- thread safety: modern computers with their many of cores can be leveraged to execute many simulations in parallel but the simulator needs to be designed in accordance;
- camera and depth sensor simulation: deep reinforcement learning has demonstrated promising results in learning vision-based policies through end-to-end learning; however, this demands the simulation of both depth and color cameras;
- ease of use, flexibility and modularity: the field of machine learning is characterized by rapid development, making it challenging to anticipate the diverse needs of researchers; for instance, techniques like privileged reinforcement learning \cite{} and domain randomization \cite{} necessitate direct access to various low-level components of the simulator to enable experimentation and innovation.

Most of these features are not implemented in the mainstreams robot simulators like Gazebo. Even if some of these features are implemented, they require too much work from the researcher. For example, to effectively use Gazebo for multi-core parallel simulations, we need to create multiple Gazebo servers with different ip addresses which need to be unique in the LAN and propagated to the correct thread; as a result, the researcher spends a lot of time in boilerplate code to handle this instead of focusing in their main research avenue. RobotDART aims at filling this gap by providing a flexible simulator for data-driven robotics while being open-source and as simple as possible.

# Design and Implementation

RobotDART is built upon the Dynamic Animation and Robotics Toolkit (DART)\cite{}, a robust open-source physics engine known for its accuracy and efficiency in simulating rigid body dynamics and articulated systems. By leveraging DART's capabilities, RobotDART provides a solid foundation for simulating a wide range of robotic systems with realistic dynamics and interactions with the environment.

One of the key strengths of RobotDART lies in its modular architecture, which allows researchers to easily customize and extend the simulator according to their specific research needs. RobotDART provides a collection of pre-built robot models, sensors, and environments, covering a diverse range of scenarios commonly encountered in robotics research. Researchers can seamlessly integrate their own robot models, sensors, and environments into RobotDART, thanks to its flexible library design and intuitive Python API.

Unlike other simulation frameworks, like Gazebo, RobotDART runs headless by default, and there is the possibility of rendering the scene (e.g., through a camera sensor) without opening a graphics window. All RobotDART's code is thread-safe (including graphics and camera sensors), and thus enables its users to use their code in parallel jobs in multicore computers. RobotDART is intended to be used by Robotics and Machine Learning researchers who want to write controllers or test learning algorithms without the delays and overhead that usually comes with other simulators (e.g., Gazebo, Coppelia-sim).

**Main features:**

- High-performance simulation engine based on the DART physics engine.
- Support for various robot platforms, including manipulators, mobile robots, and humanoid robots.
- Extensive library of sensors, such as cameras, lidars, and inertial measurement units (IMUs).
- Modular environment system for creating custom simulation scenarios.
- Intuitive Python API for easy integration with machine learning frameworks such as TensorFlow and PyTorch.
- Modular graphics API for easily creating custom render pipelines
- Thread-safe code that enables parallelization without worries

# Use Cases in Scientific Works/Projects

This library is used in these scientific contributions: @chatzilygeroudis2017black, etc.

# Conclusion

RobotDART is a versatile and efficient robot simulator tailored to the needs of robotics and machine learning researchers. By combining the accuracy of the DART physics engine with a modular and extensible design, RobotDART provides researchers with a powerful tool for prototyping, testing, and validating algorithms in a simulated environment. We believe that RobotDART will facilitate advancements in robotics and machine learning research by providing a flexible and accessible platform for exploring new ideas and pushing the boundaries of innovation.

<!--
# Figures
Figures can be included like this:
![Caption for example figure.\label{fig:example}](flowchart.png)
and referenced from text using \autoref{fig:example}.

Figure sizes can be customized by adding an optional second parameter:
![Caption for example figure.](flowchart.png){ width=10% }
-->
# Acknowledgements

The was supported by the Hellenic Foundation for Research and Innovation (H.F.R.I.) under the ``3rd Call for H.F.R.I. Research Projects to support Post-Doctoral Researchers'' (Project Acronym: NOSALRO, Project Number: 7541).

# References
