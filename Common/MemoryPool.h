#pragma once

#include "ConfigCommon.h"

class MemoryGroup {
public:
	MemoryGroup();

private:
	// Header
	bool	m_bInitialized;
	UINT	m_uDataSize;
	UINT	m_uCapacity;
	UINT	m_uAssigned;
	BYTE*	m_pBlockHead;
	BYTE*	m_pBlockNext;
	UINT64*	m_pBlockLink;

public:
	void	Initialize( UINT uDataSize );
public:
	inline void* Alloc() 
	{
		if ( !m_bInitialized )
			return nullptr;

		UINT uBlockSize = m_uDataSize + sizeof( UINT64* );

		BYTE* p;
		if ( !m_pBlockNext ) {
			m_pBlockNext = p = static_cast< BYTE* >( malloc( uBlockSize * Config::MEMORY_POOL_BLOCK_ALLOC_COUNT ) );

			if ( m_pBlockLink ) *m_pBlockLink = reinterpret_cast< UINT64 >( m_pBlockNext );

			for ( int nIdx = 1; nIdx < Config::MEMORY_POOL_BLOCK_ALLOC_COUNT + 1; ++nIdx ) {
				UINT64* pNext =
					reinterpret_cast< UINT64* >( p + nIdx * uBlockSize ) - 1;
				if ( nIdx < Config::MEMORY_POOL_BLOCK_ALLOC_COUNT )
					*pNext = reinterpret_cast< UINT64 >( pNext + 1 );
				else {
					*pNext = 0ULL;
					m_pBlockLink = pNext;
				}
			}

			m_uCapacity += Config::MEMORY_POOL_BLOCK_ALLOC_COUNT;
		}

		p = m_pBlockNext;
		m_pBlockNext = reinterpret_cast< BYTE* >( *reinterpret_cast< UINT64* >( p + m_uDataSize ) );
		++m_uAssigned;

		return p;
	}
	inline void	Free( void* pMem )
	{
		if ( !m_bInitialized || !m_uAssigned )
			return;

		BYTE* p = static_cast< BYTE* >( pMem );
		UINT64* pNext = reinterpret_cast< UINT64* >( p + m_uDataSize );
		*pNext = reinterpret_cast< UINT64 >( m_pBlockHead );
		m_pBlockHead = p;

		--m_uAssigned;
	}
};

class MemoryPool {
public:
	MemoryPool();

private:
	MemoryGroup m_memoryGroup[ Config::MEMORY_POOL_SIZE ];

private:
	void OnCreate();

public:
	template<typename T>
	T* Alloc() {
		size_t size = sizeof( T );

		if ( size > Config::MEMORY_POOL_SIZE )
			return static_cast< T* >( malloc( sizeof( T ) ) );

		return static_cast< T* >( m_memoryGroup[ size - 1 ].Alloc() );
	}

	template<typename T>
	void Free( T* p ) {
		size_t size = sizeof( T );

		if ( size > Config::MEMORY_POOL_SIZE )
			free( p );

		m_memoryGroup[ size - 1 ].Free( p );
	}
};