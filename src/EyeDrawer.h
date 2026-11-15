/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#ifndef _EYEDRAWER_h
#define _EYEDRAWER_h

#include <Arduino.h>

#include "EyeConfig.h"
#include "FaceCanvas.h"


enum CornerType
{
	T_R, T_L, B_L, B_R
};


/**
 * Draws one eye from its (already animated) EyeConfig.
 *
 * Note on coordinates: the algorithm below was originally written for a portrait
 * screen (M5StickC) and works in a transposed space where "x" runs vertically and
 * "y" runs horizontally. The private helpers at the end transpose back, so the
 * face is drawn in normal landscape coordinates on any FaceCanvas.
 */
class EyeDrawer
{

public:

	static void Draw(FaceCanvas& buffer, int16_t centerX, int16_t centerY, EyeConfig* config, uint16_t color, uint16_t background)
	{
		int32_t delta_y_top = config->Height * config->Slope_Top / 2.0;
		int32_t delta_y_bottom = config->Height * config->Slope_Bottom / 2.0;

		auto totalHeight = config->Height + delta_y_top - delta_y_bottom;
		if (config->Radius_Bottom > 0 && config->Radius_Top > 0 && totalHeight - 1 < config->Radius_Bottom + config->Radius_Top)
		{
			int32_t corrected_radius_top = (float)config->Radius_Top * (totalHeight - 1) / (config->Radius_Bottom + config->Radius_Top);
			int32_t corrected_radius_bottom = (float)config->Radius_Bottom * (totalHeight - 1) / (config->Radius_Bottom + config->Radius_Top);
			config->Radius_Top = corrected_radius_top;
			config->Radius_Bottom = corrected_radius_bottom;
		}

		int32_t TLc_x = centerY + config->OffsetY - config->Height / 2 + config->Radius_Top + delta_y_top;
		int32_t TLc_y = centerX + config->OffsetX + config->Width / 2 - config->Radius_Top;
		int32_t TL0_y = TLc_y + config->Radius_Top;
		int32_t TL1_x = TLc_x - config->Radius_Top;
		int32_t TL1_y = TLc_y;

		int32_t TRc_x = centerY + config->OffsetY - config->Height / 2 + config->Radius_Top - delta_y_top;
		int32_t TRc_y = centerX + config->OffsetX - config->Width / 2 + config->Radius_Top;
		int32_t TR0_y = TRc_y - config->Radius_Top;
		int32_t TR1_x = TRc_x - config->Radius_Top;
		int32_t TR1_y = TRc_y;


		int32_t BLc_x = centerY + config->OffsetY + config->Height / 2 - config->Radius_Bottom + delta_y_bottom;
		int32_t BLc_y = centerX + config->OffsetX + config->Width / 2 - config->Radius_Bottom;
		int32_t BL0_y = BLc_y + config->Radius_Bottom;
		int32_t BL1_x = BLc_x + config->Radius_Bottom;
		int32_t BL1_y = BLc_y;

		int32_t BRc_x = centerY + config->OffsetY + config->Height / 2 - config->Radius_Bottom - delta_y_bottom;
		int32_t BRc_y = centerX + config->OffsetX - config->Width / 2 + config->Radius_Bottom;
		int32_t BR0_y = BRc_y - config->Radius_Bottom;
		int32_t BR1_x = BRc_x + config->Radius_Bottom;
		int32_t BR1_y = BRc_y;


		int32_t iTLc_x = TL1_x;
		int32_t iTLc_y = TL1_y - config->Inverse_Radius_Top;
		int32_t iTL0_x = iTLc_x + config->Inverse_Radius_Top;
		int32_t iTL0_y = iTLc_y;

		int32_t iTRc_x = TR1_x;
		int32_t iTRc_y = TR1_y + config->Inverse_Radius_Top;
		int32_t iTR0_x = iTRc_x + config->Inverse_Radius_Top;
		int32_t iTR0_y = iTRc_y;

		int32_t iBLc_x = BL1_x;
		int32_t iBLc_y = BL1_y - config->Inverse_Radius_Bottom;
		int32_t iBL0_x = iBLc_x - config->Inverse_Radius_Bottom;
		int32_t iBL0_y = iBLc_y;

		int32_t iBRc_x = BR1_x;
		int32_t iBRc_y = BR1_y + config->Inverse_Radius_Bottom;
		int32_t iBR0_x = iBRc_x - config->Inverse_Radius_Bottom;
		int32_t iBR0_y = iBRc_y;


		int32_t min_c_x = max(TLc_x, TRc_x);
		int32_t max_c_x = min(BLc_x, BRc_x);
		int32_t min_c_y = max(TRc_y, BRc_y);
		int32_t max_c_y = min(TLc_y, BLc_y);

		int32_t min_e_x = max(TL1_x, TR1_x);
		int32_t max_e_x = min(BL1_x, BR1_x);
		int32_t min_e_y = max(BR0_y, TR0_y);
		int32_t max_e_y = min(TL0_y, BL0_y);

		EyeDrawer::FillRectangle(buffer, min_c_x, min_c_y, max_c_x, max_c_y, color);

		EyeDrawer::FillRectangle(buffer, TRc_x, min_e_y, BRc_x, min_c_y, color);
		EyeDrawer::FillRectangle(buffer, TLc_x, max_c_y, BLc_x, max_e_y, color);
		EyeDrawer::FillRectangle(buffer, min_e_x, TRc_y, min_c_x, TLc_y, color);
		EyeDrawer::FillRectangle(buffer, max_c_x, BRc_y, max_e_x, BLc_y, color);

		if (config->Slope_Top > 0)
		{
			EyeDrawer::FillRectangularTriangle(buffer, TL1_x, TL1_y, TR1_x, TR1_y, background);
				EyeDrawer::FillRectangularTriangle(buffer, TR1_x, TR1_y, TL1_x, TL1_y, color);
		}
		else if (config->Slope_Top < 0)
		{
			EyeDrawer::FillRectangularTriangle(buffer, TR1_x, TR1_y, TL1_x, TL1_y, background);
			EyeDrawer::FillRectangularTriangle(buffer, TL1_x, TL1_y, TR1_x, TR1_y, color);
		}

		if (config->Slope_Bottom > 0)
		{
			EyeDrawer::FillRectangularTriangle(buffer, BR1_x, BR1_y, BL1_x, BL1_y, background);
			EyeDrawer::FillRectangularTriangle(buffer, BL1_x, BL1_y, BR1_x, BR1_y, color);
		}
		else if (config->Slope_Bottom < 0)
		{
			EyeDrawer::FillRectangularTriangle(buffer, BL1_x, BL1_y, BR1_x, BR1_y, background);
			EyeDrawer::FillRectangularTriangle(buffer, BR1_x, BR1_y, BL1_x, BL1_y, color);
		}


		if (config->Radius_Top > 0)
		{
			EyeDrawer::FillEllipseCorner(buffer, T_L, TLc_x, TLc_y, config->Radius_Top, config->Radius_Top, color);
			EyeDrawer::FillEllipseCorner(buffer, T_R, TRc_x, TRc_y, config->Radius_Top, config->Radius_Top, color);
		}

		if (config->Radius_Bottom > 0)
		{
			EyeDrawer::FillEllipseCorner(buffer, B_L, BLc_x, BLc_y, config->Radius_Bottom, config->Radius_Bottom, color);
			EyeDrawer::FillEllipseCorner(buffer, B_R, BRc_x, BRc_y, config->Radius_Bottom, config->Radius_Bottom, color);
		}

		if (config->Inverse_Radius_Top > 0)
		{
			EyeDrawer::FillRectangle(buffer, min(iTLc_x, iTRc_x) - 1, iTRc_y, min(iTL0_x, iTR0_x) + 1, iTLc_y, background);

			if (config->Slope_Top > 0)
			{

				EyeDrawer::FillRectangularTriangle(buffer, iTL0_x, iTL0_y, iTR0_x, iTR0_y, background);
			}
			else if (config->Slope_Top < 0)
			{
				EyeDrawer::FillRectangularTriangle(buffer, iTR0_x, iTR0_y, iTL0_x, iTL0_y, background);
			}

			EyeDrawer::FillEllipseCorner(buffer, B_L, iTLc_x - 1, iTLc_y, config->Inverse_Radius_Top, config->Inverse_Radius_Top, background);
			EyeDrawer::FillEllipseCorner(buffer, B_R, iTRc_x - 1, iTRc_y, config->Inverse_Radius_Top, config->Inverse_Radius_Top, background);
		}

		if (config->Inverse_Radius_Bottom > 0)
		{
			EyeDrawer::FillEllipseCorner(buffer, T_L, iBLc_x + 1, iBLc_y, config->Inverse_Radius_Bottom, config->Inverse_Radius_Bottom, background);
			EyeDrawer::FillEllipseCorner(buffer, T_R, iBRc_x + 1, iBRc_y, config->Inverse_Radius_Bottom, config->Inverse_Radius_Bottom, background);

			EyeDrawer::FillRectangle(buffer, max(iBL0_x, iBR0_x) + 1, iBRc_y - 1, max(iBLc_x, iBRc_x) + 1, iBLc_y + 1, background);

			if (config->Slope_Bottom > 0)
			{
				EyeDrawer::FillRectangularTriangle(buffer, iBR0_x, iBR0_y, iBL0_x, iBL0_y, background);
			}
			else if (config->Slope_Bottom < 0)
			{
				EyeDrawer::FillRectangularTriangle(buffer, iBL0_x, iBL0_y, iBR0_x, iBR0_y, background);
			}
		}

	}

private:
	static void FillEllipseCorner(FaceCanvas& buffer, CornerType corner, int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color)
	{
		if (rx < 2) return;
		if (ry < 2) return;
		int32_t x, y;
		int32_t rx2 = rx * rx;
		int32_t ry2 = ry * ry;
		int32_t fx2 = 4 * rx2;
		int32_t fy2 = 4 * ry2;
		int32_t s;

		if (corner == T_R)
		{
			for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y; x++) {

				DrawLine(buffer, x0 - x, y0 - y, x, color);

				if (s >= 0) {
					s += fx2 * (1 - y);
					y--;
				}
				s += ry2 * ((4 * x) + 6);
			}

			for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1 - 2 * rx); rx2 * y <= ry2 * x; y++) {

				DrawLine(buffer, x0 - x, y0 - y, x, color);

				if (s >= 0) {
					s += fy2 * (1 - x);
					x--;
				}
				s += rx2 * ((4 * y) + 6);
			}
		}

		else if (corner == B_R)
		{
			for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y; x++) {

				DrawLine(buffer, x0, y0 - y, x, color);

				if (s >= 0) {
					s += fx2 * (1 - y);
					y--;
				}
				s += ry2 * ((4 * x) + 6);
			}

			for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1 - 2 * rx); rx2 * y <= ry2 * x; y++) {

				DrawLine(buffer, x0, y0 - y, x, color);

				if (s >= 0) {
					s += fy2 * (1 - x);
					x--;
				}
				s += rx2 * ((4 * y) + 6);
			}
		}

		else if (corner == T_L)
		{
			for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y; x++) {

				DrawLine(buffer, x0 - x, y0 + y - 1, x, color);

				if (s >= 0) {
					s += fx2 * (1 - y);
					y--;
				}
				s += ry2 * ((4 * x) + 6);
			}

			for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1 - 2 * rx); rx2 * y <= ry2 * x; y++) {

				DrawLine(buffer, x0 - x, y0 + y - 1, x, color);

				if (s >= 0) {
					s += fy2 * (1 - x);
					x--;
				}
				s += rx2 * ((4 * y) + 6);
			}
		}

		else if (corner == B_L)
		{
			for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y; x++) {

				DrawLine(buffer, x0, y0 + y - 1, x, color);

				if (s >= 0) {
					s += fx2 * (1 - y);
					y--;
				}
				s += ry2 * ((4 * x) + 6);
			}

			for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1 - 2 * rx); rx2 * y <= ry2 * x; y++) {

				DrawLine(buffer, x0, y0 + y - 1, x, color);

				if (s >= 0) {
					s += fy2 * (1 - x);
					x--;
				}
				s += rx2 * ((4 * y) + 6);
			}

		}
	}

	// --- Helpers. They receive transposed coordinates and draw them in screen space ---

	// A run of "w" pixels along the transposed x axis (a vertical run on screen)
	static void DrawLine(FaceCanvas& buffer, int32_t x, int32_t y, int32_t w, uint16_t color)
	{
		if (w < 1) return;
		buffer.fillRect(y, x, 1, w, color);
	}

	// Rectangle from (x0, y0) to (x1, y1). Empty or inverted rectangles are skipped
	static void FillRectangle(FaceCanvas& buffer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color)
	{
		if (x1 - x0 < 1 || y1 - y0 < 1) return;
		buffer.fillRect(y0, x0, y1 - y0, x1 - x0, color);
	}

	// Right triangle with vertices (x0, y0), (x1, y1) and (x1, y0)
	static void FillRectangularTriangle(FaceCanvas& buffer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color)
	{
		buffer.fillTriangle(y0, x0, y1, x1, y0, x1, color);
	}
};


#endif
