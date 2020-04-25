#include <Arduino.h>
#include "BIRO_IMU_mod.h"

IMU imu;                      //Initializes IMU setup
void setup() 
{
  Serial.begin(BAUD_RATE);    //Sets Baud rate 115200
  Serial.print("hardware UART initilization complete\r\n");
  
  pinMode(39, OUTPUT);        //Initializes Digital Pin 39
  digitalWrite(39,0);         //Sets Pin 39 to Low
                              //Used to control decimal point on LED
                              
  Serial.print("software UART initilization complete\r\n");
  Serial.print("begin decode\r\n");
  
  imu.INIT();                 //Initializes IMU object
}

void loop()
{
  imu.READ();                //Starts Reading Data from Sensors

}
