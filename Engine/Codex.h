#pragma once

#include <vector>
#include "Surface.h"
#include "ChiliUtil.h"
#include <algorithm>
#include <string>

// we will make this a singleton (there can be only one!)
template<class T>
class Codex
{
private:
	class Entry
	{
	public:
		Entry( const std::string& key,const T* pSurface )
			:
			key( key ),
			pSurface( pSurface )
		{}
		std::string key;
		// this pointer owns the surface on the heap
		// put the surfaces on the heap to keep them STABLE
		const T* pSurface;
		// operator needed for binary search and lower bound
		bool operator<( const Entry& rhs ) const
		{
			return key < rhs.key;
		}
	};
public:
	// retrieve a ptr to surface based on string (load if not exist)
	static const T* Retrieve( const std::string& key )
	{
		return Get()._Retrieve( key );
	}
	// remove all entries from codex
	static void Purge()
	{
		Get()._Purge();
	}
private:
	Codex() = default;
	~Codex()
	{
		for( auto& e : entries )
		{
			delete e.pSurface;
		}
	}
	// retrieve a ptr to surface based on string (load if not exist)
	const Surface* _Retrieve( const std::string& key )
	{
		// see if surface already exists in codex with binary search
		const auto i = binary_find( entries.begin(),entries.end(),key,
			[]( const Entry& e )
			{
				return e.key;
			}
		);
		// if surface does not exist, load, store in sorted pos in codex, and return ptr
		if( i == entries.end() )
		{
			// create an entry based on key and new (heap) loaded surface
			const Entry e( key,new Surface( key ) );
			// find sorted position in entries vector
			const auto i = std::lower_bound( entries.begin(),entries.end(),e );
			// insert entry
			entries.insert( i,e );
			// return pointer to Surface
			return e.pSurface;
		}
		// else return ptr to existing surface in codex
		else
		{
			return i->pSurface;
		}
	}
	// remove all entries from codex
	void _Purge()
	{
		for( auto& e : entries )
		{
			delete e.pSurface;
		}
		entries.clear();
	}
	// gets the singleton instance (creates if doesn't already exist)
	static Codex& Get()
	{
		static SurfaceCodex codex;
		return codex;
	}
private:
	std::vector<Entry> entries;
};

using SurfaceCodex = Codex<Surface>;