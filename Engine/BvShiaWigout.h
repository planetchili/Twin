#pragma once
#include "Shia.h"
#include <random>

class Shia::Wigout : public Shia::Behavior
{
public:
	Wigout( float duration,float period,float magnitude );
	Behavior* Update( Shia& shia,class World& world,float dt ) override;
	void Activate( Shia& shia,const class World& world ) override;
private:
	// how long the behavior lasts
	float duration;
	// controls rate of vibration
	float period;
	// vibration timer
	float v_time = 0.0f;
	// state timer
	float s_time = 0.0f;
	// base position
	Vec2 base;
	// random generation shiz for vibration
	std::normal_distribution<float> dist;
	std::mt19937 rng = std::mt19937( std::random_device{}() );
};