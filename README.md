# Gait_Analyzer
This project entails using two wireless imu sensors as an angle measurement device.
This angle is plotted against time using Mathlab Simulink.
This angle is displayed on an LED Screen.
C++ Source code for operating HI221/HI221GW imu Sensore module and node with an arduino due MCU.
C++ Source code for managining input to cd4543be for displaying output of the IMU sensors on the 7 segment LED.

The are two Header Files
BIRO_IMU_mod.h
LED.h

There is a MATLAB simuliink file that generates the angle versus time graph.
Reader.slx

BIRO_IMU_mod.h manages the imu sensors.
LED.h manages the LED Display.

BIRO_Final.ino  -> Main program file for the MCU(Arduino DUE)
