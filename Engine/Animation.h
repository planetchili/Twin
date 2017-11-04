#pragma once

#include "Surface.h"
#include "Graphics.h"
#include <vector>

class Animation
{
public:
	Animation( int x,int y,int width,int height,int count,const Surface* sprite,float holdTime );
	template<class T>
	void Draw( const Vei2& pos,const RectI& clip,Graphics& gfx,T effect,bool mirrored = false ) const
	{
		gfx.DrawSprite( pos.x,pos.y,frames[iCurFrame],*sprite,effect,mirrored );
	}
	void Update( float dt );
	void Reset();
private:
	void Advance();
private:
	const Surface* sprite;
	std::vector<RectI> frames;
	int iCurFrame = 0;
	float holdTime;
	float curFrameTime = 0.0f;
};