#pragma once
#include "Poo.h"
#include <random>

class Poo::Pursue : public Poo::Behavior
{
public:
	Behavior* Update( Poo& poo,class World& world,float dt ) override;
private:
	float speed = 60.0f;
	// how long the behavior lasts
	float duration = 8.0f;
	// square of distance from target needed before settling
	float settle_sq = 3.0f;
	// square of the distace to keep from other poos
	float avoid_sq = 64.0f;
	// state timer
	float s_time = 0.0f;
};