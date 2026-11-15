/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#include "LookAssistant.h"
#include "Face.h"
#include "EyeTransformation.h"

LookAssistant::LookAssistant(Face& face) : _face(face), Timer(4000)
{
	Timer.Start();
}

void LookAssistant::LookAt(float x, float y)
{
	x = constrain(x, -1.0f, 1.0f);
	y = constrain(y, -1.0f, 1.0f);

	// Horizontal travel: 25 px for 40 px eyes, limited so the eyes never leave the screen
	float scale = _face.RightEye.Scale;
	float maxMoveX = (float)_face.Width / 2 - _face.EyeSize - _face.EyeInterDistance;
	float amplitudeX = 25 * scale;
	if (amplitudeX > maxMoveX) amplitudeX = maxMoveX > 0 ? maxMoveX : 0;

	int16_t moveX = amplitudeX * x;
	int16_t moveY = 20 * scale * y;

	// The eye closer to the looking direction gets taller, the other one shorter
	float scaleY_y = 1.0 - (y > 0 ? y : -y) * 0.4;

	transformation.MoveX = moveX;
	transformation.MoveY = moveY;
	transformation.ScaleX = 1.0;
	transformation.ScaleY = (1.0 + x * 0.2) * scaleY_y;
	_face.RightEye.Transformation.SetDestin(transformation);

	transformation.ScaleY = (1.0 - x * 0.2) * scaleY_y;
	_face.LeftEye.Transformation.SetDestin(transformation);

	_face.RightEye.Transformation.Animation.Restart();
	_face.LeftEye.Transformation.Animation.Restart();
}

void LookAssistant::Update()
{
	Timer.Update();

	if (Timer.IsExpired())
	{
		Timer.Reset();
		auto x = random(-50, 50);
		auto y = random(-50, 50);
		LookAt((float)x  / 100, (float)y / 100);

	}

}

