#pragma once

#include "Vec2.h"
#include <algorithm>

template<typename T>
class Rect_
{
public:
	Rect_( T left_in,T right_in,T top_in,T bottom_in )
		:
		left( left_in ),
		right( right_in ),
		top( top_in ),
		bottom( bottom_in )
	{}
	Rect_( const Vec2_<T>& topLeft,const Vec2_<T>& bottomRight )
		:
		Rect_( topLeft.x,bottomRight.x,topLeft.y,bottomRight.y )
	{}
	Rect_( const Vec2_<T>& topLeft,T width,T height )
		:
		Rect_( topLeft,topLeft + Vec2_<T>( width,height ) )
	{}
	// rect-to-rect converting ctor
	template<typename S>
	explicit Rect_( const Rect_<S>& src )
		:
		left( (T)src.left ),
		right( (T)src.right ),
		top( (T)src.top ),
		bottom( (T)src.bottom )
	{}
	bool IsOverlappingWith( const Rect_& other ) const
	{
		return right > other.left && left < other.right
			&& bottom > other.top && top < other.bottom;
	}
	bool IsContainedBy( const Rect_& other ) const
	{
		return left >= other.left && right <= other.right &&
			top >= other.top && bottom <= other.bottom;
	}
	bool Contains( const Vec2_<T>& point ) const
	{
		return point.x >= left && point.x < right && point.y >= top && point.y < bottom;
	}
	// degenerate means top is below or equal bottom / left is right of or equal right
	bool IsDegenerate() const
	{
		return right <= left || bottom <= top;
	}
	Rect_& ClipTo( const Rect_& clip )
	{
		left = std::max( left,clip.left );
		right = std::min( right,clip.right );
		top = std::max( top,clip.top );
		bottom = std::min( bottom,clip.bottom );
		return *this;
	}
	Rect_ GetClippedTo( const Rect_& clip ) const
	{
		return Rect_( *this ).ClipTo( clip );
	}
	static Rect_ FromCenter( const Vec2_<T>& center,T halfWidth,T halfHeight )
	{
		const Vec2_<T> half( halfWidth,halfHeight );
		return Rect_( center - half,center + half );
	}
	Rect_ GetExpanded( T offset ) const
	{
		return Rect_( left - offset,right + offset,top - offset,bottom + offset );
	}
	Vec2_<T> GetCenter() const
	{
		return Vec2_<T>( (left + right) / (T)2,(top + bottom) / (T)2 );
	}
	T GetWidth() const
	{
		return right - left;
	}
	T GetHeight() const
	{
		return bottom - top;
	}
	Vec2_<T> TopLeft() const
	{
		return{ left,top };
	}
	Vec2_<T> BottomRight() const
	{
		return{ right,bottom };
	}
	Rect_& DisplaceBy( const Vec2_<T>& d )
	{
		left += d.x;
		right += d.x;
		top += d.y;
		bottom += d.y;
		return *this;
	}
	Rect_ GetDisplacedBy( const Vec2_<T>& d ) const
	{
		return Rect_( *this ).DisplaceBy( d );
	}
public:
	T left;
	T right;
	T top;
	T bottom;
};

typedef Rect_<int> RectI;
typedef Rect_<float> RectF;