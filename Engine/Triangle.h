#pragma once
#include "Vec2.h"
#include "Rect.h"
#include "ChiliMath.h"

template<typename T>
class Triangle
{
	using ScalarType = decltype(T::x);
public:
	constexpr Triangle() = default;
	constexpr Triangle( const T& v0,const T& v1,const T& v2 )
		:
		v0( v0 ),
		v1( v1 ),
		v2( v2 )
	{}
	float GetArea() const
	{
		return 0.5f * (-v1.y * v2.x + v0.y * (-v1.x + v2.x) + v0.x * (v1.y - v2.y) + v1.x * v2.y);
	}
	bool ContainsPoint( const T& pt,float area ) const
	{
		const auto ai = 1.0f / (2.0f * area);
		const auto t = ai * (v0.x * v1.y - v0.y * v1.x + (v0.y - v1.y) * pt.x + (v1.x - v0.x) * pt.y);
		const auto s = ai * (v0.y * v2.x - v0.x * v2.y + (v2.y - v0.y) * pt.x + (v0.x - v2.x) * pt.y);
		return s > 0.0f && t > 0.0f && 1.0f - s - t > 0.0f;
	}
	bool ContainsPoint( const T& pt ) const
	{
		return ContainsPoint( pt,GetArea() );
	}
	bool Overlaps( const RectF& rect ) const
	{
		const Vec2 pts[] = {
			rect.TopLeft(),{ rect.right,rect.top },
			rect.BottomRight(),{ rect.left,rect.bottom },
		};
		const auto a = GetArea();
		for( const auto& pt : pts )
		{
			if( ContainsPoint( pt,a ) )
			{
				return true;
			}
		}
		return rect.Contains( v0 ) || rect.Contains( v1 ) || rect.Contains( v2 );
	}
public:
	T v0,v1,v2;
};