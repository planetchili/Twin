#pragma once

#include <vector>
#include "Surface.h"
#include <algorithm>
#include <string>

// we will make this a singleton (there can be only one!)
class SurfaceCodex
{
private:
	class Entry
	{
	public:
		Entry( const std::string& key,Surface* pSurface )
			:
			key( key ),
			pSurface( pSurface )
		{}
		std::string key;
		// this pointer owns the surface on the heap
		// put the surfaces on the heap to keep them STABLE
		Surface* pSurface;
	};
public:
	// retrieve a ptr to surface based on string (load if not exist)
	static Surface* Retrieve( const std::string& key )
	{
		return Get()._Retrieve( key );
	}
	// remove all entries from codex
	static Surface* Purge()
	{
		Get()._Purge();
	}
private:
	SurfaceCodex() = default;
	~SurfaceCodex()
	{
		for( auto& e : entries )
		{
			delete e.pSurface;
		}
	}
	// retrieve a ptr to surface based on string (load if not exist)
	Surface* _Retrieve( const std::string& key )
	{
		// see if surface already exists in codex
		const auto i = std::find_if( entries.begin(),entries.end(),
			[&key]( const Entry& e )
			{
				return key == e.key;
			}
		);
		// if surface does not exist, load, store in codex, and return ptr
		if( i == entries.end() )
		{
			auto p = new Surface( key );
			entries.emplace_back( key,new Surface( key ) );
			return p;
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
	static SurfaceCodex& Get()
	{
		static SurfaceCodex codex;
		return codex;
	}
private:
	std::vector<Entry> entries;
};