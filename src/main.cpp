#include <Arduino.h>

#define Button1 23
#define Button2 22
#define Button3 21
#define Button4 19
#define Trig 2
#define Echo 4

long duration;
int distance;

void setup()
{
  Serial.begin(115200);
  // Button Pin Mode
  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);
  pinMode(Button3, INPUT_PULLUP);
  pinMode(Button4, INPUT_PULLUP);
  // Ultrasonic Trig and Echo
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
}

void loop()
{
  declareButtons();
}

void declareButtons()
{
  byte button1Status = digitalRead(Button1); // LOW = Pressed
  byte button2Status = digitalRead(Button2); // LOW = Pressed
  byte button3Status = digitalRead(Button3); // HIGH = Pressed
  byte button4Status = digitalRead(Button4); // LOW = Pressed
}

void readDistance()
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  duration = pulseIn(Echo, HIGH);
  distance = duration * 0.034 / 2;
  // return distance;
  Serial.println(distance);
}