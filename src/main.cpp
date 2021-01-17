#include <Arduino.h>
#include "TrinketJoystick.h"

const int ledPin = 1; // built in red LED
const int potPin = 3; // PB3
volatile int minThrottle = 0;
volatile int maxThrottle = 0;
volatile int throttleMapped = 0;

int calibrate()
{
  int value = 0;
  for (uint8_t i = 0; i < 100; i++)
  {
    value = analogRead(potPin);
    delay(10);
  }

  return value;
}

void blink()
{
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
}

// the setup routine runs once when you press reset:
void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(potPin, INPUT);

  TrinketJoystick.begin(); // start the USB device engine and enumerate

  // Start Calibration process
  blink();
  minThrottle = calibrate();
  maxThrottle = (1023 - minThrottle);
  blink();
}

// the loop routine runs over and over again forever:
void loop()
{
  // ATtiny85: 10 bit ADC
  int throttle = analogRead(potPin);

  if (throttle > minThrottle)
  {
    throttle = minThrottle;
  }

  throttleMapped = map(throttle, minThrottle, maxThrottle, 0, 255);
  TrinketJoystick.setThrottle(throttleMapped);
  delay(5);
}
