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
		// operator needed for binary search and lower bound
		bool operator<( const Entry& rhs ) const
		{
			return key < rhs.key;
		}
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
		// see if resource already exists in codex with binary search
		const auto i = binary_find( entries.begin(),entries.end(),key,
			[]( const Entry& e )
			{
				return e.key;
			}
		);
		// if resource does not exist, load, store in sorted pos in codex, and return ptr
		if( i == entries.end() )
		{
			// create an entry based on key and new (heap) loaded resource
			const Entry e( key,new T( key ) );
			// find sorted position in entries vector
			const auto i = std::lower_bound( entries.begin(),entries.end(),e );
			// insert entry
			entries.insert( i,e );
			// return pointer to resource
			return e.pResource;
		}
		// else return ptr to existing resource in codex
		else
		{
			return i->pResource;
		}
	}
	// remove all entries from codex
	void _Purge()
	{
		for( auto& e : entries )
		{
			delete e.presource;
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