/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#ifndef _FACE_h
#define _FACE_h

#include <Arduino.h>

#include "FaceCanvas.h"
#include "Animations.h"
#include "EyePresets.h"
#include "Eye.h"
#include "FaceEmotions.hpp"
#include "FaceExpression.h"
#include "FaceBehavior.h"
#include "LookAssistant.h"
#include "BlinkAssistant.h"

class Face
{
protected:
	FaceCanvas& _canvas;

	void Draw();

public:
	/**
	 * @param canvas        Where to draw (see FaceCanvas.h and the adapters in canvas/)
	 * @param screenWidth   Width of the drawing area, in pixels
	 * @param screenHeight  Height of the drawing area, in pixels
	 * @param eyeSize       Size of each eye, in pixels. Presets are designed for 40 px and scale with it
	 */
	Face(FaceCanvas& canvas, uint16_t screenWidth, uint16_t screenHeight, uint16_t eyeSize = 40);

	uint16_t Width;
	uint16_t Height;

	uint16_t CenterX;
	uint16_t CenterY;

	uint16_t EyeSize;
	uint16_t EyeInterDistance = 4;

	uint16_t Background = FACE_BLACK;

	Eye LeftEye;
	Eye RightEye;

	BlinkAssistant Blink;
	LookAssistant Look;
	FaceExpression Expression;
	FaceBehavior Behavior;

	// Call it as often as possible from loop(). Updates the automatic behaviours and draws a frame
	void Update();

	// Automatic behaviours. Disable them to control the face manually
	bool RandomBehavior = true;
	bool RandomLook = true;
	bool RandomBlink = true;

	// Change the eye (and optionally background) color. RGB565, any non-zero value on monochrome screens
	void SetColor(uint16_t eyeColor, uint16_t background = FACE_BLACK);

	// Transition to an emotion right now (to change emotions randomly, see Behavior.SetEmotion)
	void GoToEmotion(eEmotions emotion);

	void DoBlink();

	// x: -1.0 (left) to 1.0 (right), y: -1.0 (down) to 1.0 (up)
	void LookAt(float x, float y);
	void LookLeft();
	void LookRight();
	void LookFront();
	void LookTop();
	void LookBottom();

	// Keeps animating the face for some time (blocking)
	void Wait(unsigned long milliseconds);
};


#endif
