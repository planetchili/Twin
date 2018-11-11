#pragma once
#include "Surface.h"
#include "Vec2.h"

class HeartMeter
{
public:
	HeartMeter( const Vec2 pos,const class Chili& chili );
	void Draw( class Graphics& ) const;
private:
	static constexpr int offset = 20;
	float count = 69.0f;
	Vei2 pos;
	const Surface* pHeart = nullptr;
	const class Chili& chili;
};