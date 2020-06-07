#include "UserPool.h"

#include "Packet.h"
#include "User.h"

UserPool::UserPool()
{
}

UserPool::~UserPool()
{
}

void UserPool::Update()
{
}

void UserPool::OnPacket( const InPacket& iPacket )
{
}

void UserPool::SendPacket( OutPacket& oPacket )
{
}

std::shared_ptr<User> UserPool::GetUser( int nAID )
{
	if ( m_mUser.find( nAID ) == m_mUser.end() ) 
		return nullptr;

	return m_mUser[ nAID ];
}

void UserPool::AddUser( int nAID, Client* pClient )
{
	if ( m_mUser.find( nAID ) != m_mUser.end() ) return;

	m_mUser.emplace( std::make_pair( nAID, new User( nAID, pClient ) ) );
}

void UserPool::RemoveUser( int nAID )
{
	m_mUser.erase( nAID );
}
