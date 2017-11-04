#include "Animation.h"
#include "SpriteEffect.h"

Animation::Animation( int x,int y,int width,int height,int count,
					  const Surface* sprite,float holdTime )
	:
	sprite( sprite )
{
	frames.reserve( count );
	for( int i = 0; i < count; i++ )
	{
		frames.emplace_back( RectI{ x + i * width,x + (i + 1) * width,y,y + height },holdTime );
	}
}

Animation::Animation( int x,int y,int width,int height,int count,const Surface* sprite,std::vector<float> holdTimes )
	:
	sprite( sprite )
{
	assert( holdTimes.size() == count );
	frames.reserve( count );
	for( int i = 0; i < count; i++ )
	{
		frames.emplace_back( RectI{ x + i * width,x + (i + 1) * width,y,y + height },holdTimes[i] );
	}
}

void Animation::Update( float dt )
{
	curFrameTime += dt;
	while( curFrameTime >= frames[iCurFrame].holdTime )
	{
		curFrameTime -= frames[iCurFrame].holdTime;
		Advance();
	}
}

void Animation::Reset()
{
	iCurFrame = 0;
	curFrameTime = 0.0f;
}

void Animation::Advance()
{
	if( ++iCurFrame >= frames.size() )
	{
		iCurFrame = 0;
	}
}
