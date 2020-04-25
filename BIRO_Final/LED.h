#include <variant.h>
#ifndef LED_H
#define LED_H
class LED
{
  public:
    LED ();
    void SET(uint8_t Pin1,uint8_t Pin2,uint8_t Pin3 ,uint8_t Pin4, uint8_t Number);
    // Takes in four digital pins and an unsigned 1 byte number
    // Writes the bits of the 1 byte number to the digital pins 
};
#endif
