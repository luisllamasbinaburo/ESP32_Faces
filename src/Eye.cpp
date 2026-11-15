/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#include "Eye.h"
#include "Face.h"

Eye::Eye(Face& face) : _face(face)
{
	memset(&Config, 0, sizeof(Config));

	ChainOperators();
	Variation1.Animation._t0 = 200;
	Variation1.Animation._t1 = 200;
	Variation1.Animation._t2 = 200;
	Variation1.Animation._t3 = 200;
	Variation1.Animation._t4 = 0;
	Variation1.Animation.Interval = 800;

	Variation2.Animation._t0 = 0;
	Variation2.Animation._t1 = 200;
	Variation2.Animation._t2 = 200;
	Variation2.Animation._t3 = 200;
	Variation2.Animation._t4 = 200;
	Variation2.Animation.Interval = 800;
}

void Eye::ChainOperators()
{
	Transition.Origin = &Config;
	Transformation.Input = &Config;
	Variation1.Input = &(Transformation.Output);
	Variation2.Input = &(Variation1.Output);
	BlinkTransformation.Input = &(Variation2.Output);
	FinalConfig = &(BlinkTransformation.Output);
}

void Eye::Update()
{
	Variation1.Scale = Scale;
	Variation2.Scale = Scale;
	BlinkTransformation.BlinkWidth = 60 * Scale;

	Transition.Update();
	Transformation.Update();
	Variation1.Update();
	Variation2.Update();
	BlinkTransformation.Update();
}

void Eye::Draw(FaceCanvas& canvas, uint16_t background)
{
	Update();
	EyeDrawer::Draw(canvas, CenterX, CenterY, FinalConfig, Foreground, background);
}

void Eye::CopyPreset(const EyeConfig& preset, EyeConfig& destin)
{
	destin.OffsetX = (IsMirrored ? -preset.OffsetX : preset.OffsetX) * Scale;
	destin.OffsetY = -preset.OffsetY * Scale;
	destin.Height = preset.Height * Scale;
	destin.Width = preset.Width * Scale;
	destin.Slope_Top = IsMirrored ? preset.Slope_Top : -preset.Slope_Top;
	destin.Slope_Bottom = IsMirrored ? preset.Slope_Bottom : -preset.Slope_Bottom;
	destin.Radius_Top = preset.Radius_Top * Scale;
	destin.Radius_Bottom = preset.Radius_Bottom * Scale;
	destin.Inverse_Radius_Top = preset.Inverse_Radius_Top * Scale;
	destin.Inverse_Radius_Bottom = preset.Inverse_Radius_Bottom * Scale;
	destin.Inverse_Offset_Top = preset.Inverse_Offset_Top * Scale;
	destin.Inverse_Offset_Bottom = preset.Inverse_Offset_Bottom * Scale;
}

void Eye::ApplyPreset(const EyeConfig config)
{
	CopyPreset(config, Config);
	CopyPreset(config, Transition.Destin);
	Transition.Animation.Restart();
}

void Eye::TransitionTo(const EyeConfig config)
{
	CopyPreset(config, Transition.Destin);
	Transition.Animation.Restart();
}
