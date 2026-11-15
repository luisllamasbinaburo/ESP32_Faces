/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#ifndef _FACECANVAS_h
#define _FACECANVAS_h

#include <Arduino.h>

/**
 * Minimal drawing surface used by the library.
 *
 * The face only needs to clear the screen, fill rectangles, fill triangles and
 * push the finished frame. Implement these four methods to support any display,
 * or use one of the ready-made adapters in "canvas/":
 *
 *   - canvas/TFTSpriteCanvas.h    TFT_eSPI, M5StickC, M5GFX, LovyanGFX sprites
 *   - canvas/U8g2Canvas.h         Any U8g2 display (SSD1306, SH1106, ...)
 *   - canvas/AdafruitGFXCanvas.h  Adafruit_SSD1306, Adafruit_SH110X, GFXcanvas16
 *
 * Colors are RGB565. Monochrome adapters treat any non-zero color as "on".
 */
class FaceCanvas
{
public:
	virtual ~FaceCanvas() {}

	virtual void clear(uint16_t color) = 0;
	virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
	virtual void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) = 0;
	virtual void flush() = 0;

	// Clips a rectangle to [0, width) x [0, height). Returns false if nothing is left to draw.
	static bool clipRect(int16_t& x, int16_t& y, int16_t& w, int16_t& h, int16_t width, int16_t height)
	{
		if (x < 0) { w += x; x = 0; }
		if (y < 0) { h += y; y = 0; }
		if (x + w > width) w = width - x;
		if (y + h > height) h = height - y;
		return w > 0 && h > 0;
	}
};

// Some handy RGB565 colors
#define FACE_BLACK   0x0000
#define FACE_WHITE   0xFFFF
#define FACE_CYAN    0x07FF
#define FACE_BLUE    0x041F
#define FACE_GREEN   0x07E0
#define FACE_YELLOW  0xFFE0
#define FACE_ORANGE  0xFC00
#define FACE_RED     0xF800
#define FACE_MAGENTA 0xF81F
#define FACE_PINK    0xFB56

#endif
