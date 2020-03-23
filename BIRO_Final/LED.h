#include <variant.h>
#ifndef LED_H
#define LED_H
class LED
{
  public:
    LED ();
    void SET(uint8_t Pin1,uint8_t Pin2,uint8_t Pin3 ,uint8_t Pin4, uint8_t Number);
};
#endif
