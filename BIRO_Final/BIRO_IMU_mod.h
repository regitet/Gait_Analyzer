#include <variant.h>
#include "LED.h"
#define SERIAL_WL      (Serial1)
#define BAUD_RATE      (115200)
#define MAX_PACKET_LEN (256)

class IMU
{
  public:
    void INIT();            // Initiates IMU Sensors
    void READ();            // Read Data from IMU Sensors
    void DISPLAY_BUFFER(uint8_t *crc_header);
    int ready_to_read = 0;  //Flag when Buffer is ready
    
  private: 
    uint32_t ofs;
    uint8_t  buf[MAX_PACKET_LEN + 7] = {0};
    uint16_t payload_len; 
    uint16_t len;

    /* state machine state */
    enum status
    {
        kStatus_Idle,
        kStatus_Cmd,
        kStatus_LenLow,
        kStatus_LenHigh,
        kStatus_CRCLow,
        kStatus_CRCHigh,
        kStatus_Data,
    };
    LED A;
    LED B;
    LED C;
    LED D;
    uint8_t  ch;
    void Packet_Decode(uint8_t c); 
    static void crc16_update(uint16_t *currectCrc, const uint8_t *src, uint32_t lengthInBytes);
    float Angle(float& R1, float& R2);
    int* digits(int Number);

              
};
