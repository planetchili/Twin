#pragma once
#include "Shia.h"
#include <random>
#include "SpriteElement.h"

class Shia::Poopin : public Shia::Behavior
{
public:
	Poopin( float duration,float period,float magnitude,int count,float poopin_shake_factor );
	Behavior* Update( Shia& shia,class World& world,float dt ) override;
	void Activate( Shia& shia,const class World& world ) override;
private:
	// how long the behavior lasts
	float duration;
	// controls rate of vibration
	float period;
	// vibration timer
	float v_time = 0.0f;
	// shake factor for poopin
	float poopin_shake_factor;
	// state timer
	float s_time = 0.0f;
	// base position
	Vec2 base;
	// poop times
	std::vector<float> ptimes;
	// next scheduled poop
	std::vector<float>::const_iterator iNextPoop;
	// poop offset
	OffsetElement oe;
	// random generation shiz for vibration
	std::normal_distribution<float> shake_dist;
	std::normal_distribution<float> angle_dist = std::normal_distribution<float>( 0.0f,PI / 20.0f );
	std::normal_distribution<float> speed_dist = std::normal_distribution<float>( 500.0f,20.0f );
	std::mt19937 rng = std::mt19937( std::random_device{}() );
};