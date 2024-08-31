#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); //CE ,CSN
const byte address[6] = "00002";

void setup() {

  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);

}//end of setup void
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

void loop() {
  Serial.println("A1 :");
  Serial.println(analogRead(A0));
  data.x    = analogRead(A1);
  data.y    = analogRead(A0);
  data.a   = digitalRead(2);
  data.b   = digitalRead(3);
  data.c   = digitalRead(4);
  data.d   = digitalRead(5);
  data.e   = digitalRead(6);
  data.f   = digitalRead(7);
  data.k   = digitalRead(8);
  radio.write(&data, sizeof(data));


}//end of loop void
