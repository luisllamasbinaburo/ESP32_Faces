/*
  ESP32_Faces - M5StickC
  The face reacts to how you hold the M5StickC, using its accelerometer:
   - Lying flat:        sad
   - Tilted one way:    angry
   - Tilted the other:  furious
   - Upright:           normal / happy
  Press button A to blink.

  For the M5StickC Plus use <M5StickCPlus.h>, a 240x135 sprite and eye size 60.

  Libraries: ESP32_Faces, M5StickC
*/

#include <M5StickC.h>
#include <ESP32_Faces.h>
#include <canvas/TFTSpriteCanvas.h>

#define WIDTH  160
#define HEIGHT 80
#define EYE    40

TFT_eSprite sprite = TFT_eSprite(&M5.Lcd);
TFTSpriteCanvas<TFT_eSprite> canvas(sprite);

Face face(canvas, WIDTH, HEIGHT, EYE);

void setup(void)
{
	M5.begin();
	M5.Lcd.setRotation(1); // Landscape. Use 3 if the face is upside down
	M5.IMU.Init();

	sprite.setColorDepth(8);
	sprite.createSprite(WIDTH, HEIGHT);

	face.SetColor(TFT_CYAN, TFT_BLACK);
	face.Expression.GoTo_Normal();

	face.Behavior.Clear();
	face.Behavior.SetEmotion(eEmotions::Normal, 1.0);
}

void loop()
{
	M5.update();

	float accX = 0;
	float accY = 0;
	float accZ = 0;
	M5.IMU.getAccelData(&accX, &accY, &accZ);

	face.Behavior.Clear();
	if (accZ > 0.8 || accZ < -0.8)
	{
		face.Behavior.SetEmotion(eEmotions::Sad, 1.0);
	}
	else if (accY > 0.8)
	{
		face.Behavior.SetEmotion(eEmotions::Angry, 1.0);
	}
	else if (accY < -0.8)
	{
		face.Behavior.SetEmotion(eEmotions::Furious, 1.0);
	}
	else
	{
		face.Behavior.SetEmotion(eEmotions::Normal, 2.0);
		face.Behavior.SetEmotion(eEmotions::Happy, 1.0);
	}

	if (M5.BtnA.wasPressed())
	{
		face.DoBlink();
	}

	face.Update();
}
