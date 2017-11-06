#include "Entity.h"

Entity::Entity( const Vec2& pos,float speed,float hitwidth,float hitheight )
	:
	pos( pos ),
	speed( speed ),
	hitbox_halfwidth( hitwidth * 0.5f ),
	hitbox_halfheight( hitheight * 0.5f )
{}

const Vec2 & Entity::GetPos() const
{
	return pos;
}

RectF Entity::GetHitbox() const
{
	return RectF::FromCenter( pos,hitbox_halfwidth,hitbox_halfheight );
}

bool Entity::IsDead() const
{
	return isDead;
}

bool Entity::IsReadyForRemoval() const
{
	return isReadyForRemoval;
}

void Entity::DisplaceBy( const Vec2& d )
{
	pos += d;
}

void Entity::SetDirection( const Vec2& dir )
{
	// x vel determines direction
	if( dir.x < 0.0f )
	{
		facingRight = false;
	}
	else if( dir.x > 0.0f )
	{
		facingRight = true;
	}
	// else if x stationary keep current direction

	vel = dir * speed;
}

void Entity::SetDead()
{
	isDead = true;
}

void Entity::SetReadyForRemoval()
{
	isReadyForRemoval = true;
}
