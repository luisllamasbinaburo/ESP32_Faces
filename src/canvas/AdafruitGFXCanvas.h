/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#ifndef _ADAFRUITGFXCANVAS_h
#define _ADAFRUITGFXCANVAS_h

#include <Adafruit_GFX.h>
#include "../FaceCanvas.h"

/**
 * Adapter for buffered monochrome Adafruit displays: Adafruit_SSD1306, Adafruit_SH1106G, Adafruit_SH1107...
 * (anything derived from Adafruit_GFX that has a display() method).
 *
 *   Adafruit_SSD1306 display(128, 64, &Wire, -1);
 *   AdafruitMonoCanvas<Adafruit_SSD1306> canvas(display);
 *
 * Any non-zero color is drawn as "on".
 */
template <class TDisplay>
class AdafruitMonoCanvas : public FaceCanvas
{
public:
	AdafruitMonoCanvas(TDisplay& display) : _display(display) {}

	void clear(uint16_t color) override { _display.fillScreen(toMono(color)); }

	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override
	{
		_display.fillRect(x, y, w, h, toMono(color));
	}

	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) override
	{
		_display.fillTriangle(x0, y0, x1, y1, x2, y2, toMono(color));
	}

	void flush() override { _display.display(); }

private:
	TDisplay& _display;
	static uint16_t toMono(uint16_t color) { return color != 0 ? 1 : 0; }
};

/**
 * Adapter for colour Adafruit TFTs (ST7735, ST7789, ILI9341...).
 * Draws into an off-screen GFXcanvas16 and pushes it in one go, avoiding flicker.
 * Needs width * height * 2 bytes of RAM (160x80 = 25 KB).
 *
 *   Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);
 *   GFXcanvas16 buffer(160, 80);
 *   AdafruitTFTCanvas<Adafruit_ST7735> canvas(tft, buffer);
 */
template <class TDisplay>
class AdafruitTFTCanvas : public FaceCanvas
{
public:
	AdafruitTFTCanvas(TDisplay& display, GFXcanvas16& buffer, int16_t x = 0, int16_t y = 0)
		: _display(display), _buffer(buffer), _x(x), _y(y) {}

	void clear(uint16_t color) override { _buffer.fillScreen(color); }

	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override
	{
		_buffer.fillRect(x, y, w, h, color);
	}

	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) override
	{
		_buffer.fillTriangle(x0, y0, x1, y1, x2, y2, color);
	}

	void flush() override
	{
		_display.drawRGBBitmap(_x, _y, _buffer.getBuffer(), _buffer.width(), _buffer.height());
	}

private:
	TDisplay& _display;
	GFXcanvas16& _buffer;
	int16_t _x;
	int16_t _y;
};

#endif
