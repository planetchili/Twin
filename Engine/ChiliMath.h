#pragma once

#include <cmath>

static constexpr double PI_D = 3.14159265358979323846;
static constexpr float PI = (float)PI_D;

// integer division with round up if remainer exists
inline int div_int_ceil( int lhs,int rhs )
{
	return (lhs + rhs - 1) / rhs;
}

template <typename T>
auto sq( T val )
{
	return val * val;
}

template <class T> 
T gaussian( T x,T o )
{
	return ((T)1.0 / sqrt( (T)2.0 * PI * sq( o ) )) * exp( -sq( x ) / ((T)2.0 * sq( o )) );
}