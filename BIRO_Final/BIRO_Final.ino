#include <Arduino.h>
#include "BIRO_IMU_mod.h"

IMU imu;
void setup() 
{
  Serial.begin(BAUD_RATE); //Output Serial
  Serial.print("hardware UART initilization complete\r\n");
  pinMode(39, OUTPUT);      //A
  digitalWrite(39,0);

  Serial.print("software UART initilization complete\r\n");
  Serial.print("begin decode\r\n");
  
  imu.INIT();
}

void loop()
{
  imu.READ();

}
