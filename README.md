# jetduinoBot
The jetduinoBot stack provides all the basic drivers for running and using a JetduinoBot.

The JetduinoBot is a ROS robot using a NVIDIA Jetson TK1 and the Jetduino robotics interface
board on a DF robot 4WD base. The JetduinoBot is very customizable, and the the urdf file is
setup so that you can add different sensor stacks on top of it. The default stack uses a 
pan/tilt servo mechanism that holds a Zed 3D stereo camera. All servo motors
will have position feedback. 

The goal of the JetduinoBot is to have it autonmously navigate around an indoor environment
using SLAM, while also navigating around obstacles. It will also have the ability to locate and
identify obstacles in the floor using visual object recognition with a DNN (deep neural network).
It will be able to sort obstacles into different categories based on vision. Some example 
categories include "small toy car", "medium toy car", "general toy", "clothes", "miscellaneous". 
Further, it will also be able to recognize landmarks to help with SLAM localization. It will
be able to memorize the location o34f landmarks and autonomously navigate to them. This includes
things like it charging station, and storage bins.

## jetduinoBot Description
The jetduinobot_description folder contains the meshes and urdf files that define the robot model.
The urdf was based on how the turtlebot is structured. It is modular so you can start with the
base and add on different sensors. The default version has a Zed camera on it. Here are the 
steps to view the robot model in rviz and use GUI controls to check the joints.

###Steps to display JetduinoBot in rviz with gui control
Run these commands in terminal windows.

**Terminal 1**
- roscore

**Terminal 2**
- cd jetduinoBot
- source devel/setup.bash
- roslaunch urdf_tutorial xacrodisplay.launch model:=src/jetduinoBot/jetduinobot_description/urdf/jetduinobot.urdf.xacro gui:=True

###Steps to display JetduinoBot in gazebo and rviz
Run these commands in terminal windows. Need to have a joystick plugged into the computer to be able to
control it.

**Terminal 1**
- roscore

**Terminal 2**
- cd jetduinoBot
- source devel/setup.bash
- roslaunch src/jetduinoBot/jetduinobot_description/launch/minimal_sim.launch

**Terminal 3**
- ./joystick_to_twist.py _linear_scale:=2.5 _angular_scale:=2.0 _linear_accel:=2.0 _angular_accel:=2.0
