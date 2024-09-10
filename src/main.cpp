#include <Arduino.h>

#define Button1 23
#define Button2 22
#define Button3 21
#define Button4 19
#define Trig 2
#define Echo 4

// Ultrasonic Sensor
long duration;
int distance;

// Button Status
byte button1Status;
byte button2Status;
byte button3Status;
byte button4Status;

// Detect Mechanism
int detectionThreshold = 20;
bool detected = false;
bool waitingForDetection = false;
unsigned long detectionStartTime = 0;        // Time when waiting for detection starts
const unsigned long detectionTimeout = 5000; // 5 seconds timeout
unsigned long detectionTime = 0;

// Items counter
int kabel = 0;
int baterai = 0;
int adaptor = 0;
int lampu = 0;

// To keep track of which button was pressed
int currentMaterial = -1;

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

void declareButtons()
{
  button1Status = digitalRead(Button1); // LOW = Pressed
  button2Status = digitalRead(Button2); // LOW = Pressed
  button3Status = digitalRead(Button3); // HIGH = Pressed
  button4Status = digitalRead(Button4); // LOW = Pressed
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
}

void detectMechanism(int &materials)
{
  if (distance < detectionThreshold)
  {
    detected = true;
    detectionTime = millis();
    Serial.println("Detected");

    // Increment material count
    materials++;
    Serial.println("Item count incremented.");

    // Reset detection state
    waitingForDetection = false;
    detected = false;
    currentMaterial = -1; // Reset current material to prevent further increments
  }
}

void loop()
{
  declareButtons();
  readDistance();

  // If not currently waiting for detection, check for button presses
  if (!waitingForDetection)
  {
    if (button1Status == LOW)
    {
      Serial.println("Button1Pressed - Waiting for detection");
      waitingForDetection = true;
      currentMaterial = 1;           // Kabel
      detectionStartTime = millis(); // Start waiting for detection
    }
    else if (button2Status == LOW)
    {
      Serial.println("Button2Pressed - Waiting for detection");
      waitingForDetection = true;
      currentMaterial = 2;           // Baterai
      detectionStartTime = millis(); // Start waiting for detection
    }
    else if (button3Status == HIGH) // Assuming this is a normally closed button
    {
      Serial.println("Button3Pressed - Waiting for detection");
      waitingForDetection = true;
      currentMaterial = 3;           // Adaptor
      detectionStartTime = millis(); // Start waiting for detection
    }
    else if (button4Status == LOW)
    {
      Serial.println("Button4Pressed - Waiting for detection");
      waitingForDetection = true;
      currentMaterial = 4;           // Lampu
      detectionStartTime = millis(); // Start waiting for detection
    }
  }

  // If waiting for detection, check if the timeout has expired
  if (waitingForDetection)
  {
    // Check if timeout has passed
    if (millis() - detectionStartTime > detectionTimeout)
    {
      Serial.println("Detection timeout - Returning to normal state");
      waitingForDetection = false;
      currentMaterial = -1; // Reset current material
    }
    else
    {
      // If within timeout, check for detection
      switch (currentMaterial)
      {
      case 1:
        detectMechanism(kabel);
        break;
      case 2:
        detectMechanism(baterai);
        break;
      case 3:
        detectMechanism(adaptor);
        break;
      case 4:
        detectMechanism(lampu);
        break;
      }
    }
  }

  // Output current item counts and distance for debugging
  Serial.println("Kabel: " + String(kabel));
  Serial.println("Baterai: " + String(baterai));
  Serial.println("Adaptor: " + String(adaptor));
  Serial.println("Lampu: " + String(lampu));
  Serial.println("Distance: " + String(distance));

  delay(500);
}
