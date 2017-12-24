#include "BvShiaEaseInto.h"
#include "World.h"

Shia::EaseInto::EaseInto( const Vec2& target,float speed )
	:
	spd( speed )
{
	this->target = target;
	OutputDebugString( L"ease into create\n" );
	OutputDebugString( (std::to_wstring( target.x ) + L"\n").c_str() );
}

Shia::Behavior* Shia::EaseInto::Update( Shia& shia,World& world,float dt )
{
	OutputDebugString( L"ease into update\n" );
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
	shia.sprite.SetMode( SpriteMode::Standing );
	shia.sprite.Reset();
}
