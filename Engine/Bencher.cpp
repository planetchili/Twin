#include "Bencher.h"
#include <string>
#include <sstream>
#include <algorithm>

void Bencher::Start()
{
	ft.Mark();
}

bool Bencher::End()
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

Bencher::operator std::wstring() const
{
	std::wostringstream oss;
	oss << L"Sample size: " << nSamples << std::endl
		<< L"Mean: " << prevMean * 1000.0f << L"ms" << std::endl
		<< L"Min: " << prevMin * 1000.0f << L"ms" << std::endl
		<< L"Max: " << prevMax * 1000.0f << L"ms" << std::endl;
	return oss.str();
}
