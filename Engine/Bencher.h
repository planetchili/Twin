#pragma once
#include <limits>
#include <string>
#include "FrameTimer.h"

class Bencher
{
public:
	void Start();
	bool End();
	operator std::wstring() const;

private:
	FrameTimer ft;
	int count = 0;
	int nSamples = 50;
	float min = std::numeric_limits<float>::max();
	float max = 0.0f;
	float mean = 0.0f;
	float prevMin = std::numeric_limits<float>::signaling_NaN();
	float prevMax = std::numeric_limits<float>::signaling_NaN();
	float prevMean = std::numeric_limits<float>::signaling_NaN();
};