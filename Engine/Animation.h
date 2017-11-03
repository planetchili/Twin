#pragma once

#include "Surface.h"
#include "Graphics.h"
#include <vector>

class Animation
{
public:
	Animation( int x,int y,int width,int height,int count,const Surface* sprite,float holdTime,Color chroma = Colors::Magenta );
	template<class T>
	void Draw( const Vei2& pos,Graphics& gfx,T effect,bool mirrored ) const
	{
		gfx.DrawSprite( pos.x,pos.y,frames[iCurFrame],*sprite,effect,mirrored );
	}
	void Draw( const Vei2& pos,Graphics& gfx,bool mirrored = false ) const;
	// this version of draw replaces all opaque pixels with specified color
	void DrawColor( const Vei2& pos,Graphics& gfx,Color c,bool mirrored = false ) const;
	void Update( float dt );
private:
	void Advance();
private:
	Color chroma;
	const Surface* sprite;
	std::vector<RectI> frames;
	int iCurFrame = 0;
	float holdTime;
	float curFrameTime = 0.0f;
};