#include "SpriteElement.h"
#include "Surface.h"
#include "Graphics.h"
#include "SpriteEffect.h"
#include "Rect.h"
#include <cassert>

void SurfaceSpriteElement::Draw( const Vec2& pos,const RectI& clip,Graphics& gfx,
	const SpriteEffect::Driver& effect,bool mirrored ) const
{
	effect.DrawSprite( gfx,Vei2( pos + GetOffset( mirrored ) ),
		pSurf->GetRect(),clip,*pSurf,mirrored );
}

void AnimationSpriteElement::Draw( const Vec2& pos,const RectI& clip,Graphics& gfx,
	const SpriteEffect::Driver& effect,bool mirrored ) const
{
	animation.Draw( Vei2( pos + GetOffset( mirrored ) ),clip,gfx,
		effect,mirrored );
}

void AnimationSpriteElement::Reset()
{
	animation.Reset();
}

void AnimationSpriteElement::Update( float dt )
{
	animation.Update( dt );
}

void CompositeSpriteElement::Draw( const Vec2& pos,const RectI& clip,Graphics& gfx,const SpriteEffect::Driver& effect,bool mirrored ) const
{
	for( const auto pe : elementPtrs )
	{
		pe->Draw( pos,clip,gfx,effect,mirrored );
	}
}

void CompositeSpriteElement::Reset()
{
	for( const auto pe : elementPtrs )
	{
		pe->Reset();
	}
}

void CompositeSpriteElement::Update( float dt )
{
	for( const auto pe : elementPtrs )
	{
		pe->Update( dt );
	}
}

CompositeSpriteElement::~CompositeSpriteElement()
{
	for( auto pe : elementPtrs )
	{
		delete pe;
	}
}

void OffsetCompositeSpriteElement::Draw( const Vec2& pos,const RectI& clip,Graphics& gfx,const SpriteEffect::Driver& effect,bool mirrored ) const
{
	CompositeSpriteElement::Draw( pos + GetOffset( mirrored ),clip,gfx,effect,mirrored );
}
