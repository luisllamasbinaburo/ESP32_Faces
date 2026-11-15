/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#include "Face.h"
#include "FaceBehavior.h"
#include "FaceEmotions.hpp"

FaceBehavior::FaceBehavior(Face& face) : _face(face), Timer(500)
{
	Timer.Start();
	Clear();
	Emotions[(int)eEmotions::Normal] = 2.0;
	Emotions[(int)eEmotions::Happy] = 1.0;
}

void FaceBehavior::SetEmotion(eEmotions emotion, float value)
{
	if (emotion < 0 || emotion >= eEmotions::EMOTIONS_COUNT) return;
	Emotions[emotion] = value < 0 ? 0 : value;
}

float FaceBehavior::GetEmotion(eEmotions emotion)
{
	if (emotion < 0 || emotion >= eEmotions::EMOTIONS_COUNT) return 0;
	return Emotions[emotion];
}

void FaceBehavior::Clear()
{
	for (int emotion = 0; emotion < eEmotions::EMOTIONS_COUNT; emotion++)
	{
		Emotions[emotion] = 0.0;
	}
}

eEmotions FaceBehavior::GetRandomEmotion()
{
	float sum_of_weight = 0;
	for (int emotion = 0; emotion < eEmotions::EMOTIONS_COUNT; emotion++)
	{
		sum_of_weight += Emotions[emotion];
	}

	if (sum_of_weight <= 0)
	{
		return eEmotions::Normal;
	}

	// Roulette wheel selection: pick a random point in [0, sum_of_weight)
	// and return the emotion whose slice contains it
	float rand = random(0, 10000) / 10000.0f * sum_of_weight;

	float acc = 0;
	for (int emotion = 0; emotion < eEmotions::EMOTIONS_COUNT; emotion++)
	{
		if (Emotions[emotion] <= 0) continue;
		acc += Emotions[emotion];

		if (rand <= acc)
		{
			return (eEmotions)emotion;
		}
	}

	return eEmotions::Normal;
}


void FaceBehavior::Update()
{
	Timer.Update();

	if (Timer.IsExpired())
	{
		Timer.Reset();
		eEmotions newEmotion = GetRandomEmotion();
		if (CurrentEmotion != newEmotion)
		{
			GoToEmotion(newEmotion);
		}
	}
}


void FaceBehavior::GoToEmotion(eEmotions emotion)
{
	CurrentEmotion = emotion;
	_face.Expression.GoTo(emotion);
}
