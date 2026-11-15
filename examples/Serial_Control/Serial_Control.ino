/*
  ESP32_Faces - Control from the Serial Monitor
  Type commands in the Serial Monitor (115200 baud, "Newline" line ending):

    happy, sad, angry...            Go to that emotion (any of the 18 names)
    random                          Back to automatic random emotions and looks
    left, right, up, down, front    Look somewhere
    blink                           Blink
    list                            Show all emotions

  Libraries: ESP32_Faces, U8g2
*/

#include <Wire.h>
#include <U8g2lib.h>
#include <ESP32_Faces.h>
#include <canvas/U8g2Canvas.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
U8g2Canvas<U8G2> canvas(u8g2);
Face face(canvas, 128, 64, 40);

void listEmotions()
{
	Serial.print(F("Emotions:"));
	for (int e = 0; e < eEmotions::EMOTIONS_COUNT; e++)
	{
		Serial.print(' ');
		Serial.print(GetEmotionName((eEmotions)e));
	}
	Serial.println();
}

void look(void (Face::*direction)())
{
	face.RandomLook = false;
	(face.*direction)();
}

void handleCommand(String command)
{
	command.trim();
	command.toLowerCase();
	if (command.length() == 0) return;

	if (command == "blink") face.DoBlink();
	else if (command == "left") look(&Face::LookLeft);
	else if (command == "right") look(&Face::LookRight);
	else if (command == "up") look(&Face::LookTop);
	else if (command == "down") look(&Face::LookBottom);
	else if (command == "front") look(&Face::LookFront);
	else if (command == "list") listEmotions();
	else if (command == "random")
	{
		face.RandomBehavior = true;
		face.RandomLook = true;
	}
	else
	{
		for (int e = 0; e < eEmotions::EMOTIONS_COUNT; e++)
		{
			String name = GetEmotionName((eEmotions)e);
			name.toLowerCase();
			if (command == name)
			{
				face.RandomBehavior = false;
				face.GoToEmotion((eEmotions)e);
				Serial.print(F("-> "));
				Serial.println(GetEmotionName((eEmotions)e));
				return;
			}
		}
		Serial.print(F("Unknown command: "));
		Serial.println(command);
		listEmotions();
	}
}

void setup()
{
	Serial.begin(115200);
	u8g2.begin();
	u8g2.setBusClock(400000);

	for (int e = 0; e < eEmotions::EMOTIONS_COUNT; e++)
	{
		face.Behavior.SetEmotion((eEmotions)e, 1.0);
	}
	face.Behavior.Timer.SetIntervalMillis(2000);

	Serial.println(F("ESP32_Faces. Type an emotion name, random, blink, left, right, up, down, front or list"));
}

void loop()
{
	if (Serial.available())
	{
		handleCommand(Serial.readStringUntil('\n'));
	}
	face.Update();
}
