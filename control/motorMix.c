/*
Author: Leslie Nix
Date Created: 29 October 2020
Date Updated: 3 November 2020
Status: Not working; needs output, #includes, and whatnot

Motor Mixing Algorithm
*/

/*****************************************************
System Model

Configuration: 'X'
Forward Direction: x-axis (NED coordinates)

m0 - Front Right // CCW
m1 - Front Left  // CW
m2 - Back Right  // CW
m3 - Back Left   // CCW
*****************************************************/

void compute_motor_commands(float thrust_cmd, float yaw_cmd, float pitch_cmd, float roll_cmd)
{
  float m0_cmd = thrust_cmd + yaw_cmd + pitch_cmd + roll_cmd;
  float m1_cmd = thrust_cmd - yaw_cmd + pitch_cmd - roll_cmd;
  float m2_cmd = thrust_cmd - yaw_cmd - pitch_cmd + roll_cmd;
  float m3_cmd = thrust_cmd + yaw_cmd - pitch_cmd - roll_cmd;

  // Output those commands to a place

}
