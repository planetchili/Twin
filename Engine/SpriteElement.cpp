#include "SpriteElement.h"
#include "Surface.h"
#include "Graphics.h"
#include "SpriteEffect.h"
#include "Rect.h"
#include <cassert>

void SurfaceSpriteElement::Draw( const Vec2& pos,const RectI& clip,Graphics& gfx,Effect effect,bool mirrored ) const
{
	const Vec2 offset_pos = pos + GetOffset( mirrored );
	switch( effect )
	{
	case Effect::None:
		gfx.DrawSprite( (int)offset_pos.x,(int)offset_pos.y,pSurf->GetRect(),
			gfx.GetScreenRect(),*pSurf,SpriteEffect::AlphaBlendBaked{},mirrored
		);
		break;
	case Effect::ColorSub:
		gfx.DrawSprite( (int)offset_pos.x,(int)offset_pos.y,pSurf->GetRect(),
			gfx.GetScreenRect(),*pSurf,SpriteEffect::SubstitutionAlpha{ effect_color },mirrored
		);
		break;
	default:
		assert( "Bad effect type!" && false );
	}
}

void AnimationSpriteElement::Draw( const Vec2& pos,const RectI& clip,Graphics& gfx,Effect effect,bool mirrored ) const
{
	const Vec2 offset_pos = pos + GetOffset( mirrored );
	switch( effect )
	{
	case Effect::None:
		animation.Draw( Vei2( offset_pos ),clip,gfx,SpriteEffect::AlphaBlendBaked{},mirrored );
		break;
	case Effect::ColorSub:
		animation.Draw( Vei2( offset_pos ),clip,gfx,SpriteEffect::SubstitutionAlpha{ effect_color },mirrored );
		break;
	default:
		assert( "Bad effect type!" && false );
	}
}

void AnimationSpriteElement::Reset()
{
	animation.Reset();
}

void AnimationSpriteElement::Update( float dt )
{
	animation.Update( dt );
}

void CompositeSpriteElement::Draw( const Vec2& pos,const RectI& clip,Graphics& gfx,Effect effect,bool mirrored ) const
{
	for( const auto pe : elementPtrs )
	{
		pe->Draw( pos,clip,gfx,effect,mirrored );
	}
}

void CompositeSpriteElement::SetEffectColor( Color c )
{
	for( const auto pe : elementPtrs )
	{
		pe->SetEffectColor( c );
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

void OffsetCompositeSpriteElement::Draw( const Vec2& pos,const RectI& clip,Graphics& gfx,Effect effect,bool mirrored ) const
{
	CompositeSpriteElement::Draw( pos + GetOffset( mirrored ),clip,gfx,effect,mirrored );
}
