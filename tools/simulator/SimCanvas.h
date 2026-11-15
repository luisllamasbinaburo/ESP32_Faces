// FaceCanvas that draws into a RAM frame buffer, mimicking TFT_eSPI clipping and triangle filling.
#ifndef SIM_CANVAS_h
#define SIM_CANVAS_h

#include <vector>
#include "FaceCanvas.h"

class SimCanvas : public FaceCanvas
{
public:
	SimCanvas(int16_t width, int16_t height) : Width(width), Height(height), Pixels(width * height, 0) {}

	int16_t Width;
	int16_t Height;
	std::vector<uint16_t> Pixels;
	int Frames = 0;

	void clear(uint16_t color) override { std::fill(Pixels.begin(), Pixels.end(), color); }

	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override
	{
		if (!clipRect(x, y, w, h, Width, Height)) return;
		for (int j = y; j < y + h; j++)
			for (int i = x; i < x + w; i++)
				Pixels[j * Width + i] = color;
	}

	// Same scan line algorithm as TFT_eSPI / Adafruit_GFX
	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) override
	{
		int32_t a, b, y, last;
		if (y0 > y1) { std::swap(y0, y1); std::swap(x0, x1); }
		if (y1 > y2) { std::swap(y2, y1); std::swap(x2, x1); }
		if (y0 > y1) { std::swap(y0, y1); std::swap(x0, x1); }

		if (y0 == y2)
		{
			a = b = x0;
			if (x1 < a) a = x1; else if (x1 > b) b = x1;
			if (x2 < a) a = x2; else if (x2 > b) b = x2;
			hline(a, y0, b - a + 1, color);
			return;
		}

		int32_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0, dx12 = x2 - x1, dy12 = y2 - y1;
		int32_t sa = 0, sb = 0;

		last = (y1 == y2) ? y1 : y1 - 1;
		for (y = y0; y <= last; y++)
		{
			a = x0 + sa / dy01;
			b = x0 + sb / dy02;
			sa += dx01;
			sb += dx02;
			if (a > b) std::swap(a, b);
			hline(a, y, b - a + 1, color);
		}

		sa = dx12 * (y - y1);
		sb = dx02 * (y - y0);
		for (; y <= y2; y++)
		{
			a = x1 + sa / dy12;
			b = x0 + sb / dy02;
			sa += dx12;
			sb += dx02;
			if (a > b) std::swap(a, b);
			hline(a, y, b - a + 1, color);
		}
	}

	void flush() override { Frames++; }

private:
	void hline(int32_t x, int32_t y, int32_t w, uint16_t color) { fillRect(x, y, w, 1, color); }
};

#endif
