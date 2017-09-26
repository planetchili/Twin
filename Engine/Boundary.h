#pragma once

#include "Rect.h"

class Boundary
{
public:
	Boundary( const RectF& boundary_rect )
		:
		rect( boundary_rect )
	{}
	template<class Entity>
	void Adjust( Entity& e ) const
	{
		const RectF entity_rect = e.GetHitbox();
		// simultaneous left and right collision doesn't make sense
		// will not be considered
		if( entity_rect.left < rect.left )
		{
			e.DisplaceBy( { rect.left - entity_rect.left,0.0f } );
		}
		else if( entity_rect.right > rect.right )
		{
			e.DisplaceBy( { rect.right - entity_rect.right,0.0f } );
		}
		if( entity_rect.top < rect.top )
		{
			e.DisplaceBy( { 0.0f,rect.top - entity_rect.top } );
		}
		else if( entity_rect.bottom > rect.bottom )
		{
			e.DisplaceBy( { 0.0f,rect.bottom - entity_rect.bottom } );
		}
	}
	const RectF& GetRect() const
	{
		return rect;
	}
private:
	RectF rect;
};