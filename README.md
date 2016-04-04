# jetduinoBot
The jetduinoBot stack provides all the basic drivers for running and using a JetduinoBot.

The JetduinoBot is a ROS robot using a NVIDIA Jetson TK1 and the Jetduino robotics interface
board on a DF robot 4WD base. The JetduinoBot is very customizable, and the the urdf file is
setup so that you can add different sensor stacks on top of it. The default stack uses a 
pan/tilt servo mechanism that holds a minoru stereo camera (http://www.minoru3d.com/) 
and a custom built 360 degree leddar scanner on a tilt servo. Leedar is similar to lidar, but
it uses IR leds instead. By tilting the scanner on the head it will make it possible to
aim the leddar at objects of interest, or get scans of different parts of the room by
moving the head. Tilting the leddar will allow it to generate a point cloud. All servo motors
will have position feedback. 

The goal of the JetduinoBot is to have it autonmously navigate around an indoor environment
using SLAM, while also navigating around obstacles. It will also have the ability to locate and
identify obstacles in the floor using visual object recognition with a DNN (deep neural network).
It will be able to sort obstacles into different categories based on vision. Some example 
categories include "small toy car", "medium toy car", "general toy", "clothes", "miscellaneous". 
Further, it will also be able to recognize landmarks to help with SLAM localization. It will
be able to memorize the location of landmarks and autonomously navigate to them. This includes
things like it charging station, and storage bins.
