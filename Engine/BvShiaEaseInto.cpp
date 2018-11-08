#include "BvShiaEaseInto.h"
#include "World.h"

Shia::EaseInto::EaseInto( const Vec2& target,float speed )
	:
	spd( speed ),
	target( target )
{}

Shia::Behavior* Shia::EaseInto::Update( Shia& shia,World& world,float dt )
{
	if( (target - shia.GetPos()).GetLengthSq() < 4.0f )
	{
		shia.SetSpeed( 0.0f );
		if( HasSuccessors() )
		{
			return PassTorch();
		}
	}

	// if distance becomes greater than start distance for some reason, clamp speed
	// to some sane constant
	const Vec2 toVector = target - shia.GetPos();
	const float dist = toVector.GetLength();
	if( dist > startDistance )
	{
		shia.speed = std::min( 800.0f,startDistance * 3.0f );
	}
	else
	{
		shia.speed = k * (startDistance * dist - sq( dist ) * 0.95f);
		shia.SetDirection( toVector / dist );
	}

	return nullptr;
}

void Shia::EaseInto::Activate( Shia& shia,const World& world )
{
	startDistance = (target - shia.GetPos()).GetLength();
	k = 4.0f * spd / sq( startDistance );
	shia.sprite.SetMode( SpriteMode::Standing );
	shia.sprite.Reset();
}
