#include "StdafxCommon.h"
#include "MemoryPool.h"

MemoryGroup::MemoryGroup() :
	m_bInitialized( false ),
	m_uDataSize( 0 ),
	m_uCapacity( 0 ),
	m_uAssigned( 0 ),
	m_pBlockHead( nullptr ),
	m_pBlockNext( nullptr ),
	m_pBlockLink( nullptr )
{
}
void MemoryGroup::Initialize( UINT uDataSize )
{
	m_uDataSize = uDataSize;
	m_bInitialized = true;
}

MemoryPool::MemoryPool()
{
	OnCreate();
}

void MemoryPool::OnCreate()
{
	for ( int nIdx = 0; nIdx < Config::MEMORY_POOL_SIZE; ++nIdx ) {
		m_memoryGroup[ nIdx ].Initialize( nIdx + 1 );
	}
}