#pragma once
#include "Shia.h"
#include "Vec2.h"

class Shia::Fling : public Shia::Behavior
{
public:
	Fling( std::mt19937& rng );
	Behavior* Update( Shia& shia,class World& world,float dt ) override;
	void Activate( Shia& shia,const class World& world ) override;
private:
	float duration = 2.0f;
	float s_time = 0.0f;
	bool activated = false;
	class Poo* pTarget = nullptr;
	std::mt19937& rng;
};