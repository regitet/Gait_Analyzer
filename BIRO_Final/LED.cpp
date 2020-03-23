#include "LED.h"
LED::LED()
{

}
void LED::SET(uint8_t Pin1,uint8_t Pin2,uint8_t Pin3 ,uint8_t Pin4, uint8_t Number)
{
      pinMode(Pin1, OUTPUT);      //A
      digitalWrite(Pin1, bitRead(Number, 0) );
    
      pinMode(Pin2, OUTPUT);      //B
      digitalWrite(Pin2, bitRead(Number, 3) );
    
      pinMode(Pin3, OUTPUT);      //C
      digitalWrite(Pin3, bitRead(Number, 1) );
    
      pinMode(Pin4, OUTPUT);      //D
      digitalWrite(Pin4, bitRead(Number, 2));
}