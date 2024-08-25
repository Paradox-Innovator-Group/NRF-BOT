#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeOut = 0xF9E8F0F0E1AA;   //IMPORTANT: The same as in the receiver 0xF9E8F0F0E1LL

RF24 radio(9, 10); // select CE,CSN pin

struct PacketData 
{
  byte lxAxisValue;
  byte lyAxisValue;
//  byte rxAxisValue;
//  byte ryAxisValue;
//  byte lPotValue;  
//  byte rPotValue;    
  byte switch1Value;
  byte switch2Value;
  byte switch3Value;
  byte switch4Value;  
  byte switch5Value;
  byte switch6Value;
  byte switch7Value;
};
PacketData data;

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
  radio.stopListening(); //start the radio Transmitter 

  pinMode(A0,INPUT);
  pinMode(A1,INPUT); 
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);  
  pinMode(6,INPUT);
  pinMode(7,INPUT);
}

//This function is used to map 0-1023 joystick value to 0-254. hence 127 is the center value which we send.
//It also adjust the deadband in joystick.
//Jotstick values range from 0-1023. But its center value is not always 511. It is little different.
//So we need to add some deadband to center value. in our case 500-530. Any value in this deadband range is mapped to center 127.
int mapAndAdjustJoystickDeadBandValues(int value, bool reverse)
{
  if (value >= 530)
  {
    value = map(value, 530, 1023, 127, 254);
  }
  else if (value <= 500)
  {
    value = map(value, 500, 0, 127, 0);  
  }
  else
  {
    value = 127;
  }

  if (reverse)
  {
    value = 254 - value;
  }
  return value;
}

void loop()
{
  data.lxAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A1), false);
  data.lyAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A0), false);
  data.switch1Value   = digitalRead(2);
  data.switch2Value   = digitalRead(3);
  data.switch3Value   = digitalRead(4);
  data.switch4Value   = digitalRead(5);
  data.switch5Value   = digitalRead(6);
  data.switch6Value   = digitalRead(8);
  data.switch7Value   = digitalRead(7);
        
  radio.write(&data, sizeof(PacketData));
}
