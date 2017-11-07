#include "BvShiaEaseInto.h"
#include "World.h"

Shia::EaseInto::EaseInto( const Vec2& target,float speed )
	:
	target( target ),
	spd( speed )
{}

Shia::Behavior* Shia::EaseInto::Update( Shia& shia,World& world,float dt )
{
	if( (target - shia.GetPos()).GetLengthSq() < 4.0f )
	{
		if( HasSuccessors() )
		{
			return PassTorch();
		}
	}

	// do logic processing
	const Vec2 toVector = target - shia.GetPos();
	const float dist = toVector.GetLength();
	shia.SetDirection( toVector / dist );
	shia.speed = k * (startDistance * dist - sq( dist ));

	return nullptr;
}

void Shia::EaseInto::Activate( Shia& shia,const World& world )
{
	startDistance = (target - shia.GetPos()).GetLength();
	k = 4.0f * spd / sq( startDistance );
	shia.sprite.SetMode( SpriteMode::Beam );
	shia.sprite.Reset();
}
