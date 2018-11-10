#pragma once
#include <random>

class FakeCamera
{
public:
	void Generate()
	{
		if( on )
		{
			xoff = (int)d( rng );
			yoff = (int)d( rng );
		}
		else
		{
			xoff = 0;
			yoff = 0;
		}
	}
	void TurnOn()
	{
		on = true;
	}
	void TurnOff()
	{
		on = false;
	}
	int GetXOffset() const
	{
		return xoff;
	}
	int GetYOffset() const
	{
		return yoff;
	}
private:
	bool on = false;
	int xoff = 0;
	int yoff = 0;
	std::mt19937 rng;
	std::normal_distribution<float> d = std::normal_distribution<float>{ 0.0f,3.0f };
};