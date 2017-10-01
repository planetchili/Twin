#pragma once

#include <vector>

// remove an element from a vector
// messes up the order of elements
// (faster than erase and doesn't need iter)
template<typename T>
inline void remove_element( std::vector<T>& vec,size_t index )
{
	// swap element to be removed with element at back
	std::swap( vec[index],vec.back() );
	// back is now what we want dead, so pop back!
	vec.pop_back();
}

// binary search finds matching element
// otherwise returns end
// Acc is a functor used to access the search keys in the elements
template<class Iter,typename T,typename Acc>
auto binary_find( Iter begin,Iter end,const T& val,
				  Acc acc = []( const Iter::value_type& obj ) 
				  ->const Iter::value_type& { return obj; } )
{
	// Finds the lower bound in at most log(last - first) + 1 comparisons
	const auto i = std::lower_bound( begin,end,val,
		[acc]( const Iter::value_type& lhs,const T& rhs )
		{
			return acc( lhs ) < rhs;
		}
	);

	// if i is end, not found and do not dereference
	// else i is either greater than or equal to value
	// if not greater than, must be equal, so return it
	if( i != end && !(val < acc( *i ) ) )
	{
		return i; // found
	}
	else
	{
		return end; // not found
	}
}