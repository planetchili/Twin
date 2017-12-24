#pragma once
#include <limits>
#include <string>
#include <sstream>
#include <algorithm>
#include "FrameTimer.h"

class Bencher
{
public:
	void Start()
	{
		ft.Mark();
	}
	bool End()
	{
		const float dt = ft.Mark();
		min = std::min( dt,min );
		max = std::max( dt,max );
		mean += dt / nSamples;

		if( ++count >= nSamples )
		{
			// store results
			prevMin = min;
			prevMax = max;
			prevMean = mean;
			// reset bench state
			count = 0;
			min = std::numeric_limits<float>::max();
			max = 0.0f;
			mean = 0.0f;
			// signal results update
			return true;
		}
		return false;
	}
	operator std::wstring() const
	{
		std::wostringstream oss;
		oss << L"Sample size: " << nSamples << std::endl
			<< L"Mean: " << prevMean * 1000.0f << L"ms" << std::endl
			<< L"Min: " << prevMin * 1000.0f << L"ms" << std::endl
			<< L"Max: " << prevMax * 1000.0f << L"ms" << std::endl;
		return oss.str();
	}

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