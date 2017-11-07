#pragma once
#include "Shia.h"

class Shia::Chill: public Shia::Behavior
{
public:
	Chill( float duration = std::numeric_limits<float>::max() );
	Behavior* Update( Shia& shia,class World& world,float dt ) override;
	void Activate( Shia& shia,const class World& world ) override;
private:
	float duration;
	float s_time = 0.0f;
};
