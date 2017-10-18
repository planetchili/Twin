#pragma once

#include <vector>
#include "resource.h"
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
		Entry( const std::wstring& key,const T* pResource )
			:
			key( key ),
			pResource( pResource )
		{}
		std::wstring key;
		// this pointer owns the resource on the heap
		// put the resources on the heap to keep them STABLE
		const T* pResource;
	};
public:
	// retrieve a ptr to resource based on string (load if not exist)
	static const T* Retrieve( const std::wstring& key )
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
			delete e.pResource;
		}
	}
	// retrieve a ptr to resource based on string (load if not exist)
	const T* _Retrieve( const std::wstring& key )
	{
		// find position of resource OR where resource should be (with bin search)
		auto i = std::lower_bound( entries.begin(),entries.end(),key,
			[]( const Entry& e,const std::wstring& key )
			{
				return e.key < key;
			}
		);
		// if resource does not exist (i.e. i does not point to resource with right key)
		// load, store in sorted pos in codex, and set i to point to it
		if( i == entries.end() || i->key != key )
		{
			// construct entry in-place (i is the pos where the key *should* be)
			i = entries.emplace( i,key,new T( key ) );
		}
		// return ptr to resource in codex
		return i->pResource;
	}
	// remove all entries from codex
	void _Purge()
	{
		for( auto& e : entries )
		{
			delete e.pResource;
		}
		entries.clear();
	}
	// gets the singleton instance (creates if doesn't already exist)
	static Codex& Get()
	{
		static Codex codex;
		return codex;
	}
private:
	std::vector<Entry> entries;
};