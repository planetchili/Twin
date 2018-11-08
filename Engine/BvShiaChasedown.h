#pragma once
#include "Shia.h"

// faster when further
class Shia::Chasedown : public Shia::Behavior
{
public:
	Chasedown( float duration );
	Behavior* Update( Shia& shia,class World& world,float dt ) override;
private:
	float chase_speed = 85.0f;
	float chase_dist_sq = 6400.0f;
	float k = 0.01f;
	float max_speed = 600.0f;
	float duration;
	float time = 0.0f;
};
