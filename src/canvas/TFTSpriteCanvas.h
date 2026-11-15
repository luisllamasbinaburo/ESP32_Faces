/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#ifndef _TFTSPRITECANVAS_h
#define _TFTSPRITECANVAS_h

#include "../FaceCanvas.h"

/**
 * Adapter for sprite (off-screen buffer) based libraries:
 * TFT_eSPI (TFT_eSprite), M5StickC / M5Stack (TFT_eSprite), M5GFX (M5Canvas), LovyanGFX (LGFX_Sprite).
 *
 * Drawing into a sprite and pushing it once per frame avoids flicker.
 *
 *   TFT_eSprite sprite(&tft);
 *   TFTSpriteCanvas<TFT_eSprite> canvas(sprite);
 *   ...
 *   sprite.createSprite(160, 80);
 */
template <class TSprite>
class TFTSpriteCanvas : public FaceCanvas
{
public:
	TFTSpriteCanvas(TSprite& sprite, int32_t x = 0, int32_t y = 0) : _sprite(sprite), _x(x), _y(y) {}

	void clear(uint16_t color) override { _sprite.fillSprite(color); }

	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override
	{
		_sprite.fillRect(x, y, w, h, color);
	}

	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) override
	{
		_sprite.fillTriangle(x0, y0, x1, y1, x2, y2, color);
	}

	void flush() override { _sprite.pushSprite(_x, _y); }

	// Change where the sprite is pushed on the physical screen
	void setPosition(int32_t x, int32_t y) { _x = x; _y = y; }

private:
	TSprite& _sprite;
	int32_t _x;
	int32_t _y;
};

#endif
