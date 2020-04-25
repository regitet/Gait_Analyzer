#include "BIRO_IMU_mod.h"
void IMU::INIT()
{
  SERIAL_WL.begin(BAUD_RATE);
}
void IMU::READ()
{
  ready_to_read = 0;
  if(SERIAL_WL.available()>0)
  {
    ch = SERIAL_WL.read();
    Packet_Decode(ch);
  }
}
void IMU::Packet_Decode(uint8_t c)
{
    static uint16_t CRCReceived = 0; /* CRC value received from a frame */
    static uint16_t CRCCalculated = 0;  /* CRC value caluated from a frame */
    static uint8_t status = kStatus_Idle;  /* state machine */
    static uint8_t crc_header[4] = {0x5A, 0xA5, 0x00, 0x00}; 

    switch(status)
    {
        case kStatus_Idle:
             if(c == 0x5A)
                 status = kStatus_Cmd;
             break; 
        case kStatus_Cmd:
             if(c == 0xA5)
                 status = kStatus_LenLow;
             break;
        case kStatus_LenLow:
             payload_len = c;
             crc_header[2] = c;
             status = kStatus_LenHigh;
             break;
        case kStatus_LenHigh:
             payload_len |= (c<<8);
             crc_header[3] = c;
             status = kStatus_CRCLow;
             break;  
        case kStatus_CRCLow:
             CRCReceived = c;
             status = kStatus_CRCHigh;
             break;                                       
        case kStatus_CRCHigh:
             CRCReceived |= (c<<8);
             ofs = 0;
             CRCCalculated = 0;
             status = kStatus_Data;
             break; 
        case kStatus_Data:
             buf[ofs++] = c;
            if(ofs >= payload_len)
            {
                /* calculate CRC */
                crc16_update(&CRCCalculated, crc_header, 4);
                crc16_update(&CRCCalculated, buf, ofs);
                
                /* CRC match */
                if(CRCCalculated == CRCReceived)
                {
                      /* Received a frame of data successfully and start displaying */
                      ready_to_read = 1;
                      DISPLAY_BUFFER(crc_header);
                                        
                }
                status = kStatus_Idle;
            }
            break;
        default:
              status = kStatus_Idle;
            break;             
                                   
                   
    }// end of switch
  
}

/* CRC16 */
void IMU::crc16_update(uint16_t *currectCrc, const uint8_t *src, uint32_t lengthInBytes)
{
    uint32_t crc = *currectCrc;
    uint32_t j;
    for (j=0; j < lengthInBytes; ++j)
    {
        uint32_t i;
        uint32_t byte = src[j];
        crc ^= byte << 8;
        for (i = 0; i < 8; ++i)
        {
            uint32_t temp = crc << 1;
            if (crc & 0x8000)
            {
                temp ^= 0x1021;
            }
            crc = temp;
        }
    } 
    *currectCrc = crc;
}
void IMU::DISPLAY_BUFFER(uint8_t *crc_header)
{
  if(ready_to_read == 1)
  {
    int offset = 0;
    if(buf[0] == 0x72)
    { 
      float Pitch_1,Pitch_2;
      int n = 2;

      Pitch_1 = ((float)(int16_t)(buf[offset+3] + (buf[offset+4]<<8)))/100;
      offset  = offset +6;
      Pitch_2 = ((float)(int16_t)(buf[offset+3] + (buf[offset+4]<<8)))/100;
    
      float knee_angle = Angle(Pitch_1,Pitch_2); 
      int   Display    = knee_angle*10;
      int*  d          = digits(Display);
      A.SET(30,32,34,36,d[3]);        //Writing bits in d[3] to pins
      B.SET(22,24,26,28,d[2]);
      C.SET(31,33,35,37,d[1]);
      D.SET(23,25,27,29,d[0]);        //Writing bits in d[0] to pins
      
      
      uint32_t Theta;  
      memcpy(&Theta, &knee_angle, sizeof(knee_angle));
   
      byte myVar_0_Byte =  Theta>>24; // get the high byte
      byte myVar_1_Byte =  Theta>>16; // get the low byte
      byte myVar_2_Byte =  Theta>>8;  // get the high byte
      byte myVar_3_Byte =  Theta;     // get the low byte
      
      // x86 compatible machines are little-endian so we send the low byte first
      Serial.write(myVar_1_Byte);
      Serial.write(myVar_0_Byte);
      Serial.write(myVar_3_Byte); 
      Serial.write(myVar_2_Byte);
    
    }
    if( buf[0] == 0x71) 
    {
      Serial.print("W  X  Y  Z \r\n"); 
      float W1,W2;
      float X1,X2;
      float Y1,Y2;
      float Z1,Z2;
      int n = 3;
      memcpy(&W1,&buf[1+offset],4);  
      memcpy(&X1,&buf[1+offset+4],4);  
      memcpy(&Y1,&buf[1+offset+8],4); 
      memcpy(&Z1,&buf[1+offset+12],4); 
      
      offset = offset +16;
      memcpy(&W2,&buf[1+offset],4);  
      memcpy(&X2,&buf[1+offset+4],4);  
      memcpy(&Y2,&buf[1+offset+8],4); 
      memcpy(&Z2,&buf[1+offset+12],4);

      Serial.print("W\n");
      Serial.print(W1,n);
      Serial.print("\n");
      Serial.print(X1,n);
      Serial.print("\n");
      Serial.print(Y1,n);
      Serial.print("\n");     
      Serial.print(Z1,n);   
      Serial.print("\n");      
      Serial.print(W2,n);
      Serial.print("\n");
      Serial.print(X2,n);
      Serial.print("\n");
      Serial.print(Y2,n);
      Serial.print("\n");     
      Serial.print(Z2,n); 
      Serial.print("\n"); 
    } 
  }
}
float IMU:: Angle(float& R1, float& R2)
{
      float out;    
      float Y = abs(R1);
      float X = abs(R2);
      if (((R1 >= 0.0) && (R2 >= 0.0)) || ((R1 < 0.0) && (R2 < 0.0)))
      {
          if (Y > X)
          {
            out = Y - X;
          } 
          else 
          {
            out = X - Y;
          }
      } 
      else 
      {
          out = Y + X;
      }  
      return out;
}
int* IMU::digits(int Number)
{
      static int d[4] = {0};
      for(int i=0; i<5; i++)
      {
          d[i] = Number%10;
          Number = Number/10;
      }
      return d;
}
/*
void IMU::SET(uint8_t Pin1,uint8_t Pin2,uint8_t Pin3 ,uint8_t Pin4, uint8_t Number)
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
*/
