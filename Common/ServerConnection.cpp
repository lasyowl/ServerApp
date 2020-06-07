#include "StdafxCommon.h"
#include "ServerConnection.h"

void ServerConnection::Initiate( const SOCKET& socket )
{
	m_socket = socket;
}

void ServerConnection::Destroy()
{
}

void ServerConnection::Update()
{
}

void ServerConnection::OnPacket( const InPacket& iPacket )
{
	if ( m_cbOnPacket )
		m_cbOnPacket( iPacket );
}

void ServerConnection::SendPacket( OutPacket& oPacket )
{

}

void ServerConnection::RegisterCallbackOnPacket( std::function<void( const InPacket& )> f )
{
	m_cbOnPacket = f;
}
