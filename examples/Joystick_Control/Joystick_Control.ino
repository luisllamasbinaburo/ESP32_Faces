/*
  ESP32_Faces - Manual control with a joystick
  Move the joystick to make the eyes look around, press it to blink.
  Three potentiometers set how often the face gets angry, sad or surprised.

  Wiring (ESP32):
    OLED SSD1306 128x64 I2C:  SDA -> GPIO 21, SCL -> GPIO 22
    Joystick:                 VRx -> GPIO 34, VRy -> GPIO 35, SW -> GPIO 16
    Potentiometers (wipers):  GPIO 32 (angry), GPIO 33 (sad), GPIO 39 (surprised)

  Libraries: ESP32_Faces, U8g2
*/

#include <Wire.h>
#include <U8g2lib.h>
#include <ESP32_Faces.h>
#include <canvas/U8g2Canvas.h>

const int joystickXPin = 34;
const int joystickYPin = 35;
const int blinkPin = 16;
const int moodPins[] = { 32, 33, 39 };
const eEmotions moods[] = { eEmotions::Angry, eEmotions::Sad, eEmotions::Surprised };
const int ADC_MAX = 4095; // ESP32. Use 1023 on Arduino / ESP8266

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
U8g2Canvas<U8G2> canvas(u8g2);
Face face(canvas, 128, 64, 40);

// Returns -1.0 to 1.0, with a small dead zone in the centre
float readAxis(int pin)
{
	float value = analogRead(pin) * 2.0f / ADC_MAX - 1.0f;
	return fabs(value) < 0.1f ? 0.0f : value;
}

void setup()
{
	pinMode(blinkPin, INPUT_PULLUP);

	u8g2.begin();
	u8g2.setBusClock(400000);

	face.RandomLook = false;   // We move the eyes...
	face.RandomBlink = true;   // ...but they still blink on their own
	face.RandomBehavior = true;
	face.Behavior.Timer.SetIntervalMillis(1000);
}

void loop()
{
	// Update where the eyes look every 100 ms
	static unsigned long lastMove = 0;
	if (millis() - lastMove > 100)
	{
		lastMove = millis();
		face.LookAt(readAxis(joystickXPin), -readAxis(joystickYPin));
	}

	// Blink when the joystick is pressed
	static bool lastPressed = false;
	bool pressed = digitalRead(blinkPin) == LOW;
	if (pressed && !lastPressed) face.DoBlink();
	lastPressed = pressed;

	// Each potentiometer sets the weight (0 to 2) of one emotion. Normal always has weight 1
	face.Behavior.SetEmotion(eEmotions::Normal, 1.0);
	for (int i = 0; i < 3; i++)
	{
		face.Behavior.SetEmotion(moods[i], analogRead(moodPins[i]) * 2.0f / ADC_MAX);
	}

	face.Update();
}
