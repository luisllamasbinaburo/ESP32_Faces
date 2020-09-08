/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#include <M5StickC.h>

#include "Face.h"

#define WIDTH  80
#define HEIGHT 168
#define EYE 40

TFT_eSprite Buffer = TFT_eSprite(&M5.Lcd);

Face face(Buffer, HEIGHT, WIDTH, EYE);


float accX = 0;
float accY = 0;
float accZ = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;


void setup(void) {
	Serial.begin(115200);
	M5.begin();
	M5.update();

	M5.Lcd.init();

	M5.Lcd.begin();

	Buffer.setColorDepth(8);
	Buffer.createSprite(WIDTH, HEIGHT);

	face.Expression.GoTo_Normal();
	
	face.Behavior.Clear();
	face.Behavior.SetEmotion(eEmotions::Normal, 1.0);

	M5.MPU6886.Init();
}

void loop() {
	M5.MPU6886.getGyroData(&gyroX, &gyroY, &gyroZ);
	M5.MPU6886.getAccelData(&accX, &accY, &accZ);

	if (accY > 0.5)
	{
		face.Behavior.Clear();
		face.Behavior.SetEmotion(eEmotions::Angry, 1.0);
	}
	else if (accY < -0.5)
	{
		face.Behavior.Clear();
		face.Behavior.SetEmotion(eEmotions::Furious, 1.0);
		
	}
	else
	{
		face.Behavior.Clear();
		face.Behavior.SetEmotion(eEmotions::Normal, 1.0);
	}

	face.Update();
	return;
}

