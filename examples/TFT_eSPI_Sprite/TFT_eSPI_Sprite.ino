/*
  ESP32_Faces - Colour TFT with TFT_eSPI
  Animated eyes on any TFT supported by TFT_eSPI (ST7789, ST7735, ILI9341, GC9A01...).
  Example for a LilyGO T-Display (ST7789 135x240) in landscape.

  Configure your display and pins in TFT_eSPI's User_Setup_Select.h as usual.

  Libraries: ESP32_Faces, TFT_eSPI
*/

#include <TFT_eSPI.h>
#include <ESP32_Faces.h>
#include <canvas/TFTSpriteCanvas.h>

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 135

TFT_eSPI tft;
TFT_eSprite sprite(&tft); // Off-screen buffer, avoids flicker
TFTSpriteCanvas<TFT_eSprite> canvas(sprite);

// Presets are designed for 40 px eyes, bigger eyes are scaled automatically
Face face(canvas, SCREEN_WIDTH, SCREEN_HEIGHT, 70);

void setup()
{
	tft.init();
	tft.setRotation(1);
	tft.fillScreen(TFT_BLACK);

	sprite.setColorDepth(8); // 8 bit is enough for two colors and halves the RAM
	sprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);

	face.SetColor(TFT_CYAN, TFT_BLACK);

	// Every emotion with the same weight
	for (int e = 0; e < eEmotions::EMOTIONS_COUNT; e++)
	{
		face.Behavior.SetEmotion((eEmotions)e, 1.0);
	}
	face.Behavior.Timer.SetIntervalMillis(2500);
}

void loop()
{
	face.Update();
}
