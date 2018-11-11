#include "HeartMeter.h"
#include "Graphics.h"
#include "Codex.h"
#include "SpriteEffect.h"
#include "Chili.h"

HeartMeter::HeartMeter( const Vec2 pos,const Chili& chili )
	:
	pos( pos ),
	pHeart( Codex<Surface>::Retrieve( L"Images\\heart.bmp" ) ),
	chili( chili )
{}

void HeartMeter::Draw( Graphics& gfx ) const
{
	float c = chili.GetHealth();
	int x = pos.x;
	// draw full hearts
	while( c >= 1.0f )
	{
		gfx.DrawSprite( x,pos.y,*pHeart,SpriteEffect::Chroma{ Colors::Magenta } );
		c -= 1.0f;
		x += offset;
	}
	// draw half heart if necessary
	if( c > 0.0f )
	{
		auto srcRect = pHeart->GetRect();
		srcRect.right -= srcRect.GetWidth() / 2;
		gfx.DrawSprite( x,pos.y,srcRect,Graphics::GetFringeRect(),*pHeart,SpriteEffect::Chroma{ Colors::Magenta } );
	}
}
