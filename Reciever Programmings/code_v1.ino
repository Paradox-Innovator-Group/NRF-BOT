#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include<Servo.h>

#define Red 5
#define Green 4
#define Blue 3
#define in1 A0
#define in2 A1
#define in3 A2
#define in4 A3
#define enA A4
#define enB A5
#define buzzer 2
#define HeadLight 4
#define AmbientLight 3
#define TRIG_PIN  7  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     6  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200
Servo servo;

RF24 radio(9, 10);
const byte address[6] = "00002";
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();
  radio.setPALevel(RF24_PA_MIN);
  //  pinMode(2,OUTPUT);
  //  pinMode(3,OUTPUT);
  //  pinMode(10,OUTPUT);
  pinMode(2, OUTPUT);//red
  pinMode(3, OUTPUT);//green
  pinMode(4, OUTPUT);//blue

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(HeadLight, OUTPUT);
  pinMode(AmbientLight, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO_PIN, INPUT);
  servo.attach(8);
  servo.write(90);
}

struct datapack {
  int x;
  int y;
  int a;
  int b;
  int c;
  int d;
  int e;
  int f;
  int k;
};
datapack data;
int distanceCm = 0;
int distanceInch = 0;
void loop() {
  int distance = dis();
  Serial.print("distance :");
  Serial.println(distance);
  while (radio.available())
  {
    radio.read(&data, sizeof(data));
    //    Serial.print(data.x);
    //    Serial.print("\t");
    //    Serial.print(data.y);
    //    Serial.println();

    if (data.a == 0)
    {
      if (distance >= 20)
      {
        analogWrite(Red, 255);
      analogWrite(Green, 255);
      analogWrite(Blue, 255);
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
      else
      {
        Serial.println("Obstacle");

        int pwm = 150;
        int wait = 100;
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        analogWrite(enA, LOW);
        analogWrite(enB, LOW);
        servo.write(0);
        delay(500);
        int left = dis();
        delay(10);
        servo.write(180);
        delay(500);
        int right = dis();
        delay(10);
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

    else if (data.c == 0)
    {
      analogWrite(Red, 255);
      analogWrite(Green, 0);
      analogWrite(Blue, 0);
      int pwm = 255;
      analogWrite(enA, pwm);
      analogWrite(enB, pwm);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);

      Serial.println("backward");
    }
    else if (data.b == 0)
    {
      Serial.println("Right_turn");
      analogWrite(Red, 0);
      analogWrite(Green, 255);
      analogWrite(Blue, 0);
      int pwm = 255;
      analogWrite(enA, pwm);
      analogWrite(enB, pwm);
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      delay(10);
    }
    else if (data.d == 0)
    {
      Serial.println("Left_turn");
      analogWrite(Red, 0);
      analogWrite(Green, 0);
      analogWrite(Blue, 255);
      int pwm = 255;
      analogWrite(enA, pwm);
      analogWrite(enB, pwm);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      delay(10);
    }
    else if (data.y > 580)
    {
      Serial.println("Right variac");
      int pwm = map(data.x, 570, 1023, 150, 255);;
      analogWrite(enA, pwm);
      analogWrite(enB, pwm);
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      delay(10);
    }
    else if (data.y < 500)
    {
      Serial.println("Left variac");
      int pwm = map(data.x, 0, 570,  150, 255);;
      analogWrite(enA, pwm);
      analogWrite(enB, pwm);
      delay(10);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      delay(10);
    }
    else if (data.x < 500)
    {
      Serial.println("back variac");
      int pwm = map(data.x, 0, 570,  150, 255);;
      analogWrite(enA, pwm);
      analogWrite(enB, pwm);
      delay(10);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      delay(10);
    }
    else if (data.x > 580)
    {
      Serial.println("Front variac");
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
      else
      {
        Serial.println("Obstacle");

        int pwm = 150;
        int wait = 100;
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        analogWrite(enA, LOW);
        analogWrite(enB, LOW);
        servo.write(0);
        delay(500);
        int left = dis();
        delay(10);
        servo.write(180);
        delay(500);
        int right = dis();
        delay(10);
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
    if (data.k == 0)
    {
      Serial.println("Buzzer");
      digitalWrite(buzzer, HIGH);
    }
    if (data.f == 0)
    {
      Serial.println("Lights");
      analogWrite(Red, 180);
      analogWrite(Green, 80);
      analogWrite(Blue, 100);
    }
    else
    {
      //      Serial.println("Stop");
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      digitalWrite(buzzer, LOW);
      analogWrite(Red, 0);
      analogWrite(Green, 0);
      analogWrite(Blue, 0);
    }
    }
}

int dis() {
  long duration;


  // Clears the TRIG_PIN
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Sets the TRIG_PIN on HIGH state for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Reads the ECHO_PIN, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance
  distanceCm = duration * 0.034 / 2; // Distance in cm
  distanceInch = duration * 0.0133 / 2; // Distance in inches

  // Prints the distance in the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.print(" cm");
  Serial.print(" | ");
  Serial.print(distanceInch);
  Serial.println(" in");
  return distanceCm;
}
