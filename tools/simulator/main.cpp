// ESP32_Faces simulator
// Runs the real library code on a PC and records animated GIFs of the face.
// Used to generate the images of the README, and handy to design new presets without hardware.
//
//   face_sim [output_folder]

#include <cstdio>
#include <string>
#include <vector>
#include <functional>

#include "Face.h"
#include "SimCanvas.h"
#include "GifWriter.h"

unsigned long g_simMillis = 1000;

static uint32_t Rgb565To888(uint16_t c)
{
	uint32_t r = (c >> 11) & 0x1F, g = (c >> 5) & 0x3F, b = c & 0x1F;
	return ((r * 255 / 31) << 16) | ((g * 255 / 63) << 8) | (b * 255 / 31);
}

// Records what a Face draws on a SimCanvas into a GIF
class Recorder
{
public:
	static const int FrameMs = 30;

	Recorder(const std::string& path, Face& face, SimCanvas& canvas, int scale, uint16_t eye, uint16_t background)
		: _face(face), _canvas(canvas), _eye(eye), _background(background)
	{
		face.SetColor(eye, background);
		if (!_gif.Open(path.c_str(), canvas.Width, canvas.Height, scale, { Rgb565To888(background), Rgb565To888(eye) }))
			printf("Cannot write %s\n", path.c_str());
		else
			printf("Writing %s\n", path.c_str());
	}

	// Lets the face run on its own for some time, recording every frame
	void Run(unsigned long ms)
	{
		for (unsigned long t = 0; t < ms; t += FrameMs)
		{
			_face.Update();
			Capture();
			SimAdvance(FrameMs);
		}
	}

	void Capture()
	{
		std::vector<uint8_t> indices(_canvas.Pixels.size());
		for (size_t i = 0; i < indices.size(); i++) indices[i] = _canvas.Pixels[i] == _background ? 0 : 1;
		_gif.AddFrame(indices, FrameMs / 10);
	}

	void Close() { _gif.Close(); }

private:
	Face& _face;
	SimCanvas& _canvas;
	uint16_t _eye;
	uint16_t _background;
	GifWriter _gif;
};

// One GIF per emotion: normal -> emotion (with a blink) -> normal, loops seamlessly
static void RecordEmotion(const std::string& folder, eEmotions emotion)
{
	SimCanvas canvas(128, 64);
	Face face(canvas, 128, 64, 40);
	face.RandomBehavior = false;
	face.RandomLook = false;
	face.RandomBlink = false;
	face.GoToEmotion(eEmotions::Normal);
	SimAdvance(1000);
	face.Update();

	std::string name = GetEmotionName(emotion);
	for (auto& ch : name) ch = (char)tolower(ch);
	Recorder rec(folder + "/emotion_" + name + ".gif", face, canvas, 2, FACE_CYAN, FACE_BLACK);

	rec.Run(500);
	face.GoToEmotion(emotion);
	rec.Run(1400);
	face.DoBlink();
	rec.Run(1100);
	face.GoToEmotion(eEmotions::Normal);
	rec.Run(700);
	rec.Close();
}

// The hero animation of the README
static void RecordDemo(const std::string& folder, int width, int height, int eyeSize, int scale, uint16_t eye, const char* file)
{
	SimCanvas canvas(width, height);
	Face face(canvas, width, height, eyeSize);
	face.RandomBehavior = false;
	face.RandomLook = false;
	face.RandomBlink = false;
	face.GoToEmotion(eEmotions::Normal);
	SimAdvance(1000);
	face.Update();

	Recorder rec(folder + "/" + file, face, canvas, scale, eye, FACE_BLACK);

	struct Step { std::function<void()> action; unsigned long ms; };
	std::vector<Step> script = {
		{ [&] {}, 600 },
		{ [&] { face.DoBlink(); }, 500 },
		{ [&] { face.LookAt(-0.8f, 0.3f); }, 700 },
		{ [&] { face.LookAt(0.8f, 0.3f); }, 700 },
		{ [&] { face.LookFront(); }, 300 },
		{ [&] { face.GoToEmotion(eEmotions::Happy); }, 1200 },
		{ [&] { face.GoToEmotion(eEmotions::Glee); }, 1000 },
		{ [&] { face.GoToEmotion(eEmotions::Surprised); }, 900 },
		{ [&] { face.GoToEmotion(eEmotions::Suspicious); face.LookAt(-0.6f, 0); }, 1200 },
		{ [&] { face.LookAt(0.6f, 0); }, 900 },
		{ [&] { face.LookFront(); face.GoToEmotion(eEmotions::Angry); }, 1200 },
		{ [&] { face.GoToEmotion(eEmotions::Furious); }, 900 },
		{ [&] { face.GoToEmotion(eEmotions::Sad); face.LookAt(0, -0.6f); }, 1400 },
		{ [&] { face.GoToEmotion(eEmotions::Worried); face.LookFront(); }, 1100 },
		{ [&] { face.GoToEmotion(eEmotions::Sleepy); face.LookAt(0, -0.3f); }, 1400 },
		{ [&] { face.GoToEmotion(eEmotions::Awe); face.LookFront(); }, 1100 },
		{ [&] { face.GoToEmotion(eEmotions::Normal); }, 500 },
		{ [&] { face.DoBlink(); }, 600 },
	};

	for (auto& step : script)
	{
		step.action();
		rec.Run(step.ms);
	}
	rec.Close();
}

// Let the automatic behaviour run for a long (simulated) time as a smoke test
static bool StressTest()
{
	SimCanvas canvas(128, 64);
	Face face(canvas, 128, 64, 40);
	for (int e = 0; e < EMOTIONS_COUNT; e++) face.Behavior.SetEmotion((eEmotions)e, 1.0f);

	int counts[EMOTIONS_COUNT] = {};
	for (int i = 0; i < 20000; i++)
	{
		face.Update();
		SimAdvance(30);
		counts[face.Behavior.CurrentEmotion]++;
	}
	int used = 0;
	for (int e = 0; e < EMOTIONS_COUNT; e++) if (counts[e] > 0) used++;
	printf("Stress test: %d frames drawn, %d/%d emotions shown\n", canvas.Frames, used, (int)EMOTIONS_COUNT);
	return used == EMOTIONS_COUNT;
}

int main(int argc, char** argv)
{
	std::string folder = argc > 1 ? argv[1] : ".";
	srand(42);

	if (!StressTest()) { printf("Stress test FAILED\n"); return 1; }

	RecordDemo(folder, 160, 80, 40, 3, FACE_CYAN, "demo.gif");
	RecordDemo(folder, 128, 64, 40, 3, FACE_WHITE, "demo_oled.gif");

	for (int e = 0; e < EMOTIONS_COUNT; e++) RecordEmotion(folder, (eEmotions)e);

	return 0;
}
