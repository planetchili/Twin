#pragma once
#include "Vec2.h"
#include "Rect.h"
#include "SpriteElement.h"
#include <vector>

namespace SpriteEffect
{
	class Driver;
}

// the sprite graphics control class
template<typename Mode>
class SpriteControl
{
public:
	SpriteControl( Mode initial = (Mode)0 )
		:
		curMode( initial ),
		elementPtrs( (int)Mode::Count )
	{}
	// rule of 3 beware...
	~SpriteControl()
	{
		for( auto p : elementPtrs )
		{
			delete p;
		}
	}
	// actually, just delete that shit
	SpriteControl( const SpriteControl& ) = delete;
	SpriteControl& operator=( const SpriteControl& ) = delete;
	void SetMode( Mode newMode )
	{
		curMode = newMode;
	}
	void Reset()
	{
		GetCurrentElement().Reset();
	}
	void Update( float dt )
	{
		GetCurrentElement().Update( dt );
	}
	void Draw( const Vec2& pos,const RectI& clip,class Graphics& gfx,const SpriteEffect::Driver& effect,bool mirrored ) const
	{
		GetCurrentElement().Draw( pos,clip,gfx,effect,mirrored );
	}
private:
	class SpriteElement& GetCurrentElement()
	{
		return *elementPtrs[(int)curMode];
	}
	const class SpriteElement& GetCurrentElement() const
	{
		return const_cast<SpriteControl*>(this)->GetCurrentElement();
	}
private:
	Mode curMode;
protected:
	std::vector<class SpriteElement*> elementPtrs;
};

