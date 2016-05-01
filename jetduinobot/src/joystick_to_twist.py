#!/usr/bin/env python
# BEGIN ALL
import rospy
import pygame
import math
from std_msgs.msg import String
from geometry_msgs.msg import Twist

# Initialize pygame
pygame.init()

# Initialize the joysticks
pygame.joystick.init()

g_twist_pub = None
g_target_twist = None
g_last_twist = None
g_last_send_time = None
g_vel_scales = [0.1, 0.1] # default to very slow
g_vel_ramps = [1, 1] # units: meters per second^2

def read_targets():
  # EVENT PROCESSING STEP
  for event in pygame.event.get():
      if event.type == pygame.QUIT:
          done = True

      # Possible joystick actions: JOYAXISMOTION JOYBALLMOTION JOYBUTTONDOWN
      # JOYBUTTONUP JOYHATMOTION
      if event.type == pygame.JOYBUTTONDOWN:
          print("Joystick button pressed.")
      if event.type == pygame.JOYBUTTONUP:
          print("Joystick button released.")

  joystick = pygame.joystick.Joystick(0)
  joystick.init()

  forward = -joystick.get_axis(1) * g_vel_scales[1]    
  rotate = -joystick.get_axis(0) * g_vel_scales[0]
  print("[%f, %f]" % (forward, rotate))

  g_target_twist.linear.x  = forward
  g_target_twist.angular.z = rotate

def ramped_vel(v_prev, v_target, t_prev, t_now, ramp_rate):
  # compute maximum velocity step
  step = ramp_rate * (t_now - t_prev).to_sec()
  sign = 1.0 if (v_target > v_prev) else -1.0
  error = math.fabs(v_target - v_prev)
  if error < step: # we can get there within this timestep-we're done.
    return v_target
  else:
    return v_prev + sign * step  # take a step toward the target

def ramped_twist(prev, target, t_prev, t_now, ramps):
  tw = Twist()
  tw.angular.z = ramped_vel(prev.angular.z, target.angular.z, t_prev,
                            t_now, ramps[0])
  tw.linear.x = ramped_vel(prev.linear.x, target.linear.x, t_prev,
                           t_now, ramps[1])
  return tw

def send_twist():
  global g_last_twist_send_time, g_target_twist, g_last_twist,\
         g_vel_scales, g_vel_ramps, g_twist_pub
  t_now = rospy.Time.now()
  g_last_twist = ramped_twist(g_last_twist, g_target_twist,
                              g_last_twist_send_time, t_now, g_vel_ramps)
  g_last_twist_send_time = t_now
  g_twist_pub.publish(g_last_twist)

def fetch_param(name, default):
  if rospy.has_param(name):
    return rospy.get_param(name)
  else:
    print "parameter [%s] not defined. Defaulting to %.3f" % (name, default)
    return default


if __name__ == '__main__':
  rospy.init_node('joystick_to_twist')
  g_last_twist_send_time = rospy.Time.now()
  g_twist_pub = rospy.Publisher('cmd_vel', Twist, queue_size=1)
  g_target_twist = Twist() # initializes to zero
  g_last_twist = Twist()

  g_vel_scales[0] = fetch_param('~angular_scale', 0.1)
  g_vel_scales[1] = fetch_param('~linear_scale', 0.1)
  g_vel_ramps[0] = fetch_param('~angular_accel', 1.0)
  g_vel_ramps[1] = fetch_param('~linear_accel', 1.0)

  # BEGIN RATE
  rate = rospy.Rate(20)

  while not rospy.is_shutdown():
    read_targets()
    send_twist()  
    rate.sleep()

  # END RATE
# END ALL
