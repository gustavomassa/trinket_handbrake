#include <Arduino.h>
#include "TrinketJoystick.h"

const int ledPin = 1; // built in red LED
const int potPin = 3; // PB3

// the setup routine runs once when you press reset:
void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(potPin, INPUT);

  /*   // Sinalize setup
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
  digitalWrite(ledPin, HIGH); */

  TrinketJoystick.begin(); // start the USB device engine and enumerate
}

// the loop routine runs over and over again forever:
void loop()
{
  // ATtiny85: 10 bit ADC
  int throttle = analogRead(potPin);
  int throttleMapped = map(throttle, 0, 1023, 0, 255);
  TrinketJoystick.setThrottle(throttleMapped);
  //TrinketJoystick.mouseMove(0, 0, 0b0000000000000000);
  delay(5);
}
