#pragma once

#include "ChiliMath.h"

template<typename T>
class Vec2_
{
public:
	constexpr Vec2_() = default;
	constexpr Vec2_( T x_in,T y_in )
		:
		x( x_in ),
		y( y_in )
	{}
	template<typename S>
	constexpr explicit Vec2_( const Vec2_<S>& src )
		:
		x( (T)src.x ),
		y( (T)src.y )
	{}
	Vec2_ operator+( const Vec2_& rhs ) const
	{
		return Vec2_( *this ) += rhs;
	}
	Vec2_& operator+=( const Vec2_& rhs )
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vec2_ operator-( const Vec2_& rhs ) const
	{
		return Vec2_( *this ) -= rhs;
	}
	Vec2_& operator-=( const Vec2_& rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	T operator*( const Vec2_& rhs ) const
	{
		return x * rhs.x + y * rhs.y;
	}
	Vec2_ operator*( T rhs ) const
	{
		return Vec2_( *this ) *= rhs;
	}
	Vec2_& operator*=( T rhs )
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	Vec2_ operator/( T rhs ) const
	{
		return Vec2_( *this ) /= rhs;
	}
	Vec2_& operator/=( T rhs )
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
	Vec2_ operator-() const
	{
		return Vec2_( -x,-y );
	}
	T GetLength() const
	{
		return (T)std::sqrt( GetLengthSq() );
	}
	T GetLengthSq() const
	{
		return x * x + y * y;
	}
	Vec2_& Normalize()
	{
		return *this = GetNormalized();
	}
	Vec2_ GetNormalized() const
	{
		const T len = GetLength();
		if( len != (T)0 )
		{
			return *this / len;
		}
		return *this;
	}
	bool operator==( const Vec2_& rhs ) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=( const Vec2_& rhs ) const
	{
		return !(*this == rhs);
	}
	Vec2_& Rotate( float theta )
	{
		return *this = GetRotated( theta );
	}
	Vec2_ GetRotated( float theta ) const
	{
		const float sin_theta = std::sin( theta );
		const float cos_theta = std::cos( theta );
		return { y * sin_theta + x * cos_theta,y * cos_theta - x * sin_theta };
	}
public:
	T x;
	T y;
};

typedef Vec2_<float> Vec2;
typedef Vec2_<int> Vei2;