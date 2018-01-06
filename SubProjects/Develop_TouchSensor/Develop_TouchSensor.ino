#include <Wire.h>

#define DEVICE_ADDRESS 0x56

#define SETREG_THU 0x33
#define READREG_STAT 0x44


void Set_Threshould(uint16_t th1,uint16_t th2,uint16_t Duration);
void Read_Status(void);

void setup() 
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output

  Set_Threshould(200,200,5);
}

void loop() 
{
  Read_Status();
  delay(1);
}

void I2C_Write_Register(uint8_t data)
{
  Wire.beginTransmission(DEVICE_ADDRESS); 
  Wire.write(data);              // sends one byte
}


void Set_Threshould(uint16_t th1,uint16_t th2,uint16_t Duration)
{
  uint8_t sentByte = 0;
  int8_t check = 0;
  
  I2C_Write_Register(SETREG_THU);
  
  sentByte += Wire.write( th1&0x00FF    );
  sentByte += Wire.write((th1&0xFF00)>>8);
  sentByte += Wire.write( th2&0x00FF    );
  sentByte += Wire.write((th2&0xFF00)>>8);
  sentByte += Wire.write( Duration&0x00FF    );
  sentByte += Wire.write((Duration&0xFF00)>>8);
  
  check = Wire.endTransmission();
  if(check!=0)
  {
    Serial.print("occured error on I2C [");    // stop transmitting  
    Serial.print(check);
    Serial.println("]");
    while(1);
  }
  if(sentByte!=6)
  {
    Serial.print("Invalid number of sent[");
    Serial.print(sentByte);
    Serial.println("]");
    while(1);
  }
  else
  {
    Serial.print("Successd sent!![");
    Serial.print(sentByte);
    Serial.print("]");
    Serial.print("[");
    Serial.print(check);
    Serial.println("]");
  }
}

void Read_Status(void)
{
  I2C_Write_Register(READREG_STAT);
  Wire.endTransmission();
  Wire.requestFrom(DEVICE_ADDRESS,1);    // request 6 bytes from slave device #8

  while (Wire.available()) 
  { // slave may send less than requested
    uint8_t c = Wire.read(); // receive a byte as character
   Serial.print(c,BIN);
  } 
   Serial.println();
}

