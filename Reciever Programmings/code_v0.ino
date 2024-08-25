#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include<Servo.h>
#include <NewPing.h>

#define TRIGGER_PIN  7  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     6  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

#define in1 A0
#define in2 A1
#define in3 A2
#define in4 A3
#define enA A4
#define enB A5
#define buzzer 2
#define HeadLight 4
#define AmbientLight 3
#define SIGNAL_TIMEOUT 500  // This is signal timeout in milli seconds. We will reset the data if no signal

const uint64_t pipeIn = 0xF9E8F0F0E1AA;
RF24 radio(9, 10);
unsigned long lastRecvTime = 0;
Servo servo;
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
};
PacketData receiverData;
void setup()
{ Serial.begin(9600);
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, pipeIn);
  radio.startListening(); //start the radio receiver
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(HeadLight, OUTPUT);
  pinMode(AmbientLight, OUTPUT);
  servo.attach(8);
  servo.write(90);
}

void loop()
{
  Serial.println("LOOP");
  // Check if RF is connected and packet is available
  if ( radio.available())
  {
    radio.read(&receiverData, sizeof(PacketData));
    //    Serial.print("LX :");
    //    Serial.println(receiverData.lxAxisValue);
    //    Serial.print("ry :");
    //    Serial.println(receiverData.ryAxisValue);
    int distance = sonar.ping_cm();
    if (receiverData.lxAxisValue > 130 )
    {
      if (distance >= 20)
      {
        Serial.println("FORWARD");
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        int pwm = map(receiverData.lxAxisValue, 130, 255, 150, 255);
        analogWrite(enA, pwm);
        analogWrite(enB, pwm);
        delay(10);
      }
      else if (distance < 20)
      {

        int pwm = 255;
        int wait = 500;
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        analogWrite(enA, pwm);
        analogWrite(enB, pwm);

        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
        analogWrite(enA, pwm);
        analogWrite(enB, pwm);
        delay(1000);
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        analogWrite(enA, LOW);
        analogWrite(enB, LOW);
        servo.write(0);
        int left = sonar.ping_cm();
        servo.write(180);
        int right = sonar.ping_cm();
        servo.write(90);
        if (left > right)
        {
          //move left
          Serial.println("Left");
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
          analogWrite(enA, pwm);
          analogWrite(enB, pwm);
          delay(wait);
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW);
          analogWrite(enA, LOW);
          analogWrite(enB, LOW);

        }
        else if (right > left)
        {
          //move right
          Serial.println("RIGHT");
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, HIGH);
          analogWrite(enA, pwm);
          analogWrite(enB, pwm);
          delay(wait);
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW);
          analogWrite(enA, LOW);
          analogWrite(enB, LOW);
        }
      }

    }
    else if (receiverData.lxAxisValue < 120)
    { Serial.println("BACKWARD");
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      int pwm = map(receiverData.lyAxisValue, 120, 0, 150, 255);
      analogWrite(enA, pwm);
      analogWrite(enB, pwm);
      delay(10);
    }
    else if (receiverData.lyAxisValue  < 120 )
    { Serial.println("Left");
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      int pwm = map(receiverData.lyAxisValue, 130, 255, 150, 255);
      analogWrite(enA, pwm);
      analogWrite(enB, pwm);
      delay(10);
    }
    else if (receiverData.lyAxisValue > 130)
    { Serial.println("RIGHT");
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      int pwm = map(receiverData.lyAxisValue, 120, 0, 150, 255);
      analogWrite(enA, pwm);
      analogWrite(enB, pwm);
      delay(10);
    }
    else if (!receiverData.switch1Value)
    {
      if (distance >= 20)
      {
        Serial.println("FORWARD");
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        int pwm = 255;
        analogWrite(enA, pwm);
        analogWrite(enB, pwm);
        delay(10);
      }
      else if (distance < 20)
      {

        int pwm = 255;
        int wait = 500;
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        analogWrite(enA, pwm);
        analogWrite(enB, pwm);

        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
        analogWrite(enA, pwm);
        analogWrite(enB, pwm);
        delay(1000);
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        analogWrite(enA, LOW);
        analogWrite(enB, LOW);
        servo.write(0);
        int left = sonar.ping_cm();
        servo.write(180);
        int right = sonar.ping_cm();
        servo.write(90);
        if (left > right)
        {
          //move left
          Serial.println("Left");
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
          analogWrite(enA, pwm);
          analogWrite(enB, pwm);
          delay(wait);
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW);
          analogWrite(enA, LOW);
          analogWrite(enB, LOW);

        }
        else if (right > left)
        {
          //move right
          Serial.println("RIGHT");
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, HIGH);
          analogWrite(enA, pwm);
          analogWrite(enB, pwm);
          delay(wait);
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW);
          analogWrite(enA, LOW);
          analogWrite(enB, LOW);
        }
      }
    }
    else if (!receiverData.switch3Value)
    {
      Serial.println("BACKWARD");
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      int pwm = 255;
      analogWrite(enA, pwm);
      analogWrite(enB, pwm);
      delay(10);
    }
    else if (!receiverData.switch2Value)
    {
      Serial.println("RIGHT");
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      int pwm = 255;
      analogWrite(enA, pwm);
      analogWrite(enB, pwm);
      delay(10);
    }
    else if (!receiverData.switch4Value)
    {
      Serial.println("Left");
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      int pwm = 255;
      analogWrite(enA, pwm);
      analogWrite(enB, pwm);
      delay(10);
    }
    //    else if (!receiverData.switch5Value)
    //    {
    //      Serial.println("Obstacle avoidance");
    //      obstacle();
    //    }
    if (!receiverData.switch6Value)
    {
      digitalWrite(buzzer, HIGH);
    }
    else
    {
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      analogWrite(enA, LOW);
      analogWrite(enB, LOW);
    }

  }
  else
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enA, LOW);
    analogWrite(enB, LOW);
    digitalWrite(buzzer, LOW);
  }

}

//void obstacle()
//{
//
//  int pwm = 255;
//  int wait = 300;
//  digitalWrite(in1, HIGH);
//  digitalWrite(in2, LOW);
//  digitalWrite(in3, HIGH);
//  digitalWrite(in4, LOW);
//  analogWrite(enA, pwm);
//  analogWrite(enB, pwm);
//  int distance = sonar.ping_cm();
//  if (distance < 20)
//  {
//    digitalWrite(in1, LOW);
//    digitalWrite(in2, LOW);
//    digitalWrite(in3, LOW);
//    digitalWrite(in4, LOW);
//    analogWrite(enA, pwm);
//    analogWrite(enB, pwm);
//
//    digitalWrite(in1, LOW);
//    digitalWrite(in2, HIGH);
//    digitalWrite(in3, LOW);
//    digitalWrite(in4, HIGH);
//    analogWrite(enA, pwm);
//    analogWrite(enB, pwm);
//    delay(1000);
//    digitalWrite(in1, LOW);
//    digitalWrite(in2, LOW);
//    digitalWrite(in3, LOW);
//    digitalWrite(in4, LOW);
//    analogWrite(enA, LOW);
//    analogWrite(enB, LOW);
//    servo.write(0);
//    int left = sonar.ping_cm();
//    servo.write(180);
//    int right = sonar.ping_cm();
//    servo.write(90);
//    if (left > right)
//    {
//      //move left
//      Serial.println("Left");
//      digitalWrite(in1, LOW);
//      digitalWrite(in2, HIGH);
//      digitalWrite(in3, HIGH);
//      digitalWrite(in4, LOW);
//      analogWrite(enA, pwm);
//      analogWrite(enB, pwm);
//      delay(wait);
//      digitalWrite(in1, LOW);
//      digitalWrite(in2, LOW);
//      digitalWrite(in3, LOW);
//      digitalWrite(in4, LOW);
//      analogWrite(enA, LOW);
//      analogWrite(enB, LOW);
//
//    }
//    else if(right > left)
//    {
//      //move right
//      Serial.println("RIGHT");
//      digitalWrite(in1, HIGH);
//      digitalWrite(in2, LOW);
//      digitalWrite(in3, LOW);
//      digitalWrite(in4, HIGH);
//      analogWrite(enA, pwm);
//      analogWrite(enB, pwm);
//      delay(wait);
//      digitalWrite(in1, LOW);
//      digitalWrite(in2, LOW);
//      digitalWrite(in3, LOW);
//      digitalWrite(in4, LOW);
//      analogWrite(enA, LOW);
//      analogWrite(enB, LOW);
//    }
//  }
//  obstacle();
//}
