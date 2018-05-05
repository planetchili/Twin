#pragma once
#include "Shia.h"
#include "Vec2.h"

class Shia::Awaken : public Shia::Behavior
{
public:
	Awaken( std::mt19937& rng );
	Behavior* Update( Shia& shia,class World& world,float dt ) override;
	void Activate( Shia& shia,const class World& world ) override;
private:
	float duration = 2.0f;
	float s_time = 0.0f;
	float mean_n_poos = 5.0f;
	float std_dev = 2.0f;
	bool activated = false;
	std::mt19937& rng;
};