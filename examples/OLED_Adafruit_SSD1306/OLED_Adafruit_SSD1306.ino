/*
  ESP32_Faces - OLED with Adafruit SSD1306
  Animated eyes on a 128x64 SSD1306 I2C OLED display, using the Adafruit libraries.

  Wiring (ESP32): OLED SDA -> GPIO 21, OLED SCL -> GPIO 22, VCC -> 3.3V, GND -> GND

  Libraries: ESP32_Faces, Adafruit SSD1306, Adafruit GFX
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32_Faces.h>
#include <canvas/AdafruitGFXCanvas.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
AdafruitMonoCanvas<Adafruit_SSD1306> canvas(display);

Face face(canvas, 128, 64, 40);

void setup()
{
	Serial.begin(115200);

	// Most modules use the 0x3C I2C address, some use 0x3D
	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
	{
		Serial.println(F("SSD1306 not found"));
		for (;;) delay(100);
	}
	Wire.setClock(400000);

	// Let the face pick emotions randomly, every 2 seconds
	face.Behavior.Clear();
	face.Behavior.SetEmotion(eEmotions::Normal, 1.0);
	face.Behavior.SetEmotion(eEmotions::Happy, 1.0);
	face.Behavior.SetEmotion(eEmotions::Glee, 0.5);
	face.Behavior.SetEmotion(eEmotions::Sleepy, 0.3);
	face.Behavior.Timer.SetIntervalMillis(2000);
}

void loop()
{
	face.Update();
}
