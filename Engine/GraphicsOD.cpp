#define GOD_GRAPHICS
#include "Graphics.h"
#include "SpriteEffect.h"

void Graphics::BeginFrame( Color fillColor )
{
	// clear the sysbuffer
	sysBuffer.Fill( fillColor );
}

void Graphics::PutPixel( int x,int y,Color c )
{
	assert( x >= 0 );
	assert( x < int( Graphics::ScreenWidth ) );
	assert( y >= 0 );
	assert( y < int( Graphics::ScreenHeight ) );
	sysBuffer.PutPixel( x,y,c );
}

Color Graphics::GetPixel( int x,int y ) const
{
	assert( x >= 0 );
	assert( x < int( Graphics::ScreenWidth ) );
	assert( y >= 0 );
	assert( y < int( Graphics::ScreenHeight ) );
	return sysBuffer.GetPixel( x,y );
}

void dummy( Graphics& gfx )
{
	gfx.DrawSprite( 0,0,{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0 },SpriteEffect::Copy{} );
	gfx.DrawSprite( 0,0,{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0 },SpriteEffect::Chroma{ Colors::Black } );
	gfx.DrawSprite( 0,0,{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0 },SpriteEffect::Substitution{ Colors::Black,Colors::Black } );
	gfx.DrawSprite( 0,0,{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0 },SpriteEffect::Ghost{ Colors::Black } );
	gfx.DrawSprite( 0,0,{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0 },SpriteEffect::DissolveHalfTint{ Colors::Black,Colors::Black,0.0f } );
	SpriteEffect::AlphaBlendBaked{}.DrawSprite( gfx,{ 0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0 } );
	SpriteEffect::AlphaBakedSubstitution{ Colors::Black }.DrawSprite( gfx,{ 0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0 } );
}