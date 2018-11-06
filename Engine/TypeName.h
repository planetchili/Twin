#pragma once
#include <string>
#include <typeinfo>

template<typename T>
std::wstring type_name_wide( T&& o )
{
	const std::string s = typeid(std::forward<T>( o )).name();
	return std::wstring( s.begin(),s.end() );
}