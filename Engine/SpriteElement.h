#pragma once

#include "Vec2.h"
#include "Colors.h"
#include "Animation.h"
#include "Rect.h"
#include <vector>

// right now just adding virtual functions for each parameter set for each effect
// later on, will havec virtual sprite effect drivers that instantiate the functors
// in a virtual interface draw function and also carry the functor captures as member data

// class from which all sprite graphics elements will inherit
class SpriteElement
{
public:
	// baked alpha sprites assumed
	enum class Effect
	{
		None,
		ColorSub
	};
public:
	virtual void Draw( const Vec2& pos,const RectI& clip,class Graphics& gfx,Effect effect,bool mirrored ) const = 0;
	virtual void SetEffectColor( Color c ) = 0;
	virtual void Reset() = 0;
	virtual void Update( float dt ) = 0;
	virtual ~SpriteElement() = default;
};

class OffsetElement
{
public:
	OffsetElement( const Vec2& offset_unmirrored,const Vec2& offset_mirrored )
		:
		offset_unmirrored( offset_unmirrored ),
		offset_mirrored( offset_mirrored )
	{}
	const Vec2& GetOffset( bool mirrored ) const
	{
		return mirrored ? offset_mirrored : offset_unmirrored;
	}
private:
	// draw offset from base (mirrored and unmirrored versions)
	Vec2 offset_unmirrored;
	Vec2 offset_mirrored;
};

class CompositeSpriteElement : public SpriteElement
{
public:
	CompositeSpriteElement( std::vector<SpriteElement*> elementPtrs )
		:
		elementPtrs( std::move( elementPtrs ) )
	{}
	void Draw( const Vec2& pos,const RectI& clip,class Graphics& gfx,Effect effect,bool mirrored ) const override;
	void SetEffectColor( Color c ) override;
	void Reset() override;
	void Update( float dt ) override;
	~CompositeSpriteElement() override;
private:
	std::vector<SpriteElement*> elementPtrs;
};

class OffsetCompositeSpriteElement : public CompositeSpriteElement,protected OffsetElement
{
public:
	OffsetCompositeSpriteElement( std::vector<SpriteElement*> elementPtrs,
		const Vec2& offset_unmirrored,const Vec2& offset_mirrored )
		:
		CompositeSpriteElement( std::move( elementPtrs ) ),
		OffsetElement( offset_unmirrored,offset_mirrored )
	{}
	void Draw( const Vec2& pos,const RectI& clip,class Graphics& gfx,Effect effect,bool mirrored ) const override;
};

class ConcreteSpriteElement : public SpriteElement,protected OffsetElement
{
public:
	void SetEffectColor( Color c ) override
	{
		effect_color = c;
	}
protected:
	using OffsetElement::OffsetElement;
protected:
	// effect params
	Color effect_color;
};

class SurfaceSpriteElement : public ConcreteSpriteElement
{
public:
	SurfaceSpriteElement( const Surface* pSurf,const Vec2& offset_unmirrored,
		const Vec2& offset_mirrored )
		:
		ConcreteSpriteElement( offset_unmirrored,offset_mirrored ),
		pSurf( pSurf )
	{}
	void Draw( const Vec2& pos,const RectI& clip,class Graphics& gfx,Effect effect,bool mirrored ) const override;
	void Reset() override {}
	void Update( float dt ) override {}
private:
	const class Surface* pSurf;
};

class AnimationSpriteElement : public ConcreteSpriteElement
{
public:
	AnimationSpriteElement( int x,int y,int width,int height,int count,const Surface* sprite,float holdTime,
		const Vec2& offset_unmirrored,const Vec2& offset_mirrored )
		:
		ConcreteSpriteElement( offset_unmirrored,offset_mirrored ),
		animation( x,y,width,height,count,sprite,holdTime )
	{}
	AnimationSpriteElement( int x,int y,int width,int height,int count,const Surface* sprite,std::vector<float> holdTimes,
		const Vec2& offset_unmirrored,const Vec2& offset_mirrored )
		:
		ConcreteSpriteElement( offset_unmirrored,offset_mirrored ),
		animation( x,y,width,height,count,sprite,std::move( holdTimes ) )
	{}
	void Draw( const Vec2& pos,const RectI& clip,class Graphics& gfx,Effect effect,bool mirrored ) const override;
	void Reset() override;
	void Update( float dt ) override;
private:
	Animation animation;
};