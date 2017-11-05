#pragma once

#include "Surface.h"
#include "Graphics.h"
#include <vector>

namespace SpriteEffect
{
	class Driver;
}

class Animation
{
private:
	struct Frame
	{
		Frame( const RectI& rect,float holdTime )
			:
			rect( rect ),
			holdTime( holdTime )
		{}
		RectI rect;
		float holdTime;
	};
public:
	Animation( int x,int y,int width,int height,int count,const Surface* sprite,float holdTime );
	Animation( int x,int y,int width,int height,int count,const Surface* sprite,std::vector<float> holdTimes );
	template<class T>
	void Draw( const Vei2& pos,const RectI& clip,Graphics& gfx,T effect,bool mirrored = false ) const
	{
		gfx.DrawSprite( pos.x,pos.y,frames[iCurFrame].rect,*sprite,effect,mirrored );
	}
	void Draw( const Vei2& pos,const RectI& clip,Graphics& gfx,const SpriteEffect::Driver& effect,bool mirrored = false ) const;
	void Update( float dt );
	void Reset();
private:
	void Advance();
private:
	const Surface* sprite;
	std::vector<Frame> frames;
	int iCurFrame = 0;
	float curFrameTime = 0.0f;
};