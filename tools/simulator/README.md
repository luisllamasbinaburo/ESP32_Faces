# ESP32_Faces simulator

Compiles the real library code for your PC and records the face as animated GIFs.
It is how the images of the main README are generated, and it is handy to design
new presets or expressions without flashing a board every time.

## Build and run

You need CMake and a C++20 compiler (Visual Studio, GCC or Clang).

```bash
cmake -S tools/simulator -B tools/simulator/build
cmake --build tools/simulator/build --config Release
```

Then run it with the output folder as argument. From the repository root:

```bash
tools/simulator/build/Release/face_sim docs/images   # Visual Studio
tools/simulator/build/face_sim docs/images           # GCC / Clang
```

It first runs a stress test (20,000 frames of random behaviour) and then writes:

- `demo.gif`, `demo_oled.gif`: the scripted demo of the README
- `emotion_<name>.gif`: one GIF per emotion

The grid with every emotion (`all_emotions.png`) is made from the GIFs with ImageMagick:

```bash
cd docs/images
for n in normal happy glee surprised awe angry furious annoyed frustrated sad worried scared skeptic suspicious unimpressed focused squint sleepy; do
  magick "emotion_$n.gif[55]" -set label "${n^}" "/tmp/$n.png"
done
magick montage /tmp/{normal,happy,glee,surprised,awe,angry,furious,annoyed,frustrated,sad,worried,scared,skeptic,suspicious,unimpressed,focused,squint,sleepy}.png \
  -tile 6x -geometry +6+6 -background "#0d1117" -fill "#c9d1d9" -pointsize 18 all_emotions.png
```

## How it works

- `arduino/Arduino.h` is a tiny stand-in for the Arduino API. Time is simulated: `millis()` only
  advances when the simulator says so, so recordings are deterministic.
- `SimCanvas.h` is a `FaceCanvas` that draws into memory, with the same clipping and triangle
  filling as TFT_eSPI and Adafruit_GFX.
- `GifWriter.h` is a minimal animated GIF encoder.
- `main.cpp` drives a `Face` exactly as a sketch would, and records each frame.
