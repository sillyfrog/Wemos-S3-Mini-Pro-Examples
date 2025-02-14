#include <Arduino.h>

#define DELAY 500

void setup()
{
  Serial.begin(460800);
#ifdef RGB_POWER
  // Turn on power the RGB LED
  pinMode(RGB_POWER, OUTPUT);
  digitalWrite(RGB_POWER, HIGH);
#endif
}

// the loop function runs over and over again forever
void loop()
{
  // Using digitalWrite
  digitalWrite(RGB_BUILTIN, HIGH); // Turn the RGB LED white
  delay(DELAY);
  digitalWrite(RGB_BUILTIN, LOW); // Turn the RGB LED off
  delay(DELAY);

  // Using RGB values
  // NOTE: The Red and Green are swapped on the RGB LED (tested on a v1.1.0 board)
  rgbLedWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0); // Red
  delay(DELAY);
  rgbLedWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0); // Green
  delay(DELAY);
  rgbLedWrite(RGB_BUILTIN, 0, 0, RGB_BRIGHTNESS); // Blue
  delay(DELAY);
  rgbLedWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
  delay(DELAY);
  Serial.println("Looping...");
}
