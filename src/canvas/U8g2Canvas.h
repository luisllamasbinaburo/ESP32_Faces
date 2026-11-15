/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#ifndef _U8G2CANVAS_h
#define _U8G2CANVAS_h

#include "../FaceCanvas.h"

/**
 * Adapter for monochrome displays driven by U8g2 (SSD1306, SH1106, SSD1309, ST7920...).
 * Use a full frame buffer constructor (the ones ending in _F_), e.g.:
 *
 *   U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
 *   U8g2Canvas<U8G2> canvas(u8g2);
 *
 * Any non-zero color is drawn as "on".
 */
template <class TU8g2>
class U8g2Canvas : public FaceCanvas
{
public:
	U8g2Canvas(TU8g2& u8g2) : _u8g2(u8g2) {}

	void clear(uint16_t color) override
	{
		_u8g2.clearBuffer();
		if (color != 0)
		{
			_u8g2.setDrawColor(1);
			_u8g2.drawBox(0, 0, _u8g2.getDisplayWidth(), _u8g2.getDisplayHeight());
		}
	}

	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override
	{
		// U8g2 uses unsigned coordinates, so negative values must be clipped here
		if (!clipRect(x, y, w, h, _u8g2.getDisplayWidth(), _u8g2.getDisplayHeight())) return;
		_u8g2.setDrawColor(color != 0 ? 1 : 0);
		_u8g2.drawBox(x, y, w, h);
	}

	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) override
	{
		_u8g2.setDrawColor(color != 0 ? 1 : 0);
		_u8g2.drawTriangle(x0, y0, x1, y1, x2, y2);
	}

	void flush() override
	{
		_u8g2.setDrawColor(1);
		_u8g2.sendBuffer();
	}

private:
	TU8g2& _u8g2;
};

#endif
