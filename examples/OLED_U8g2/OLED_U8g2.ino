/*
  ESP32_Faces - OLED with U8g2
  Animated eyes on a 128x64 SSD1306 I2C OLED display.

  Wiring (ESP32):   OLED SDA -> GPIO 21, OLED SCL -> GPIO 22, VCC -> 3.3V, GND -> GND
  Wiring (ESP8266): OLED SDA -> D2,      OLED SCL -> D1

  Libraries: ESP32_Faces, U8g2
*/

#include <Wire.h>
#include <U8g2lib.h>
#include <ESP32_Faces.h>
#include <canvas/U8g2Canvas.h>

// A full buffer (_F_) constructor is required. For a SH1106 use U8G2_SH1106_128X64_NONAME_F_HW_I2C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
U8g2Canvas<U8G2> canvas(u8g2);

// Canvas, screen width, screen height, eye size
Face face(canvas, 128, 64, 40);

void setup()
{
	u8g2.begin();
	u8g2.setBusClock(400000); // Faster I2C = smoother animation

	// Relative weights of the emotions the face will pick randomly
	face.Behavior.Clear();
	face.Behavior.SetEmotion(eEmotions::Normal, 1.0);
	face.Behavior.SetEmotion(eEmotions::Happy, 0.5);
	face.Behavior.SetEmotion(eEmotions::Surprised, 0.2);
	face.Behavior.SetEmotion(eEmotions::Suspicious, 0.2);

	// Pick a new emotion (it may be the same one) every 3 seconds
	face.Behavior.Timer.SetIntervalMillis(3000);
}

void loop()
{
	face.Update();
}
