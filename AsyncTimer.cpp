/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version. 

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses 
****************************************************/

#include "AsyncTimer.h"

AsyncTimer::AsyncTimer(unsigned long microsInterval) : AsyncTimer(microsInterval, false, nullptr)
{
}

AsyncTimer::AsyncTimer(unsigned long microsInterval, AsyncTimerCallback onFinish) : AsyncTimer(microsInterval, false, onFinish)
{

}

AsyncTimer::AsyncTimer(unsigned long microsInterval, bool autoReset) : AsyncTimer(microsInterval, false, nullptr)
{
}

AsyncTimer::AsyncTimer(unsigned long microsInterval, bool autoReset, AsyncTimerCallback onFinish)
{
	Interval = microsInterval;
	AutoReset = autoReset;
	OnFinish = onFinish;
}

void AsyncTimer::Start()
{
	Reset();
	_isActive = true;
}

void AsyncTimer::Reset()
{
	_startTime = micros();
}

void AsyncTimer::Stop()
{
	_isActive = false;
}

bool AsyncTimer::Update()
{
	if (_isActive == false) return false;

	_isExpired = false;
	if (static_cast<unsigned long>(micros() - _startTime) >= Interval)
	{
		_isExpired = true;
		if (OnFinish != nullptr) OnFinish();
		Reset();
	}
	return _isExpired;
}

void AsyncTimer::SetIntervalMillis(unsigned long interval)
{
	Interval = 1000 * interval;
}

void AsyncTimer::SetIntervalMicros(unsigned long interval)
{
	Interval = interval;
}

unsigned long AsyncTimer::GetStartTime()
{
	return _startTime;
}

unsigned long AsyncTimer::GetElapsedTime()
{
	return micros() - _startTime;
}

unsigned long AsyncTimer::GetRemainingTime()
{
	return Interval - micros() + _startTime;
}

bool AsyncTimer::IsActive() const
{
	return _isActive;
}

bool AsyncTimer::IsExpired() const
{
	return _isExpired;
}