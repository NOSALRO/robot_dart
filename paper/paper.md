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

# Design and Implementation

RobotDART is built upon the Dynamic Animation and Robotics Toolkit (DART), a robust open-source physics engine known for its accuracy and efficiency in simulating rigid body dynamics and articulated systems. By leveraging DART's capabilities, RobotDART provides a solid foundation for simulating a wide range of robotic systems with realistic dynamics and interactions with the environment.

One of the key strengths of RobotDART lies in its modular architecture, which allows researchers to easily customize and extend the simulator according to their specific research needs. RobotDART provides a collection of pre-built robot models, sensors, and environments, covering a diverse range of scenarios commonly encountered in robotics research. Researchers can seamlessly integrate their own robot models, sensors, and environments into RobotDART, thanks to its flexible plugin system and intuitive Python API.

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
