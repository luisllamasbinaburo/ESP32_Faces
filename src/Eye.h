/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#ifndef _EYE_h
#define _EYE_h

#include <Arduino.h>

#include "Animations.h"
#include "EyeConfig.h"
#include "EyeDrawer.h"
#include "EyeTransition.h"
#include "EyeTransformation.h"
#include "EyeVariation.h"
#include "EyeBlink.h"

class Face;

/**
 * One eye. Its shape goes through a chain of operators every frame:
 *
 *   Config -> Transition -> Transformation (look) -> Variation1 -> Variation2 -> Blink -> FinalConfig
 */
class Eye
{
protected:
	Face& _face;

	void Update();
	void ChainOperators();
	void CopyPreset(const EyeConfig& preset, EyeConfig& destin);

public:
	Eye(Face& face);

	uint16_t Foreground = FACE_CYAN;

	int16_t CenterX = 0;
	int16_t CenterY = 0;
	bool IsMirrored = false;

	// Presets are designed for 40 px eyes. Scale = EyeSize / 40
	float Scale = 1.0f;

	EyeConfig Config;
	EyeConfig* FinalConfig;

	EyeTransition Transition;
	EyeTransformation Transformation;
	EyeVariation Variation1;
	EyeVariation Variation2;
	EyeBlink BlinkTransformation;

	void ApplyPreset(const EyeConfig preset);
	void TransitionTo(const EyeConfig preset);

	void Draw(FaceCanvas& canvas, uint16_t background);
};


#endif
