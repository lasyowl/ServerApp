#include "StdafxCommon.h"
#include "ClientManager.h"

enum SOCKET_INIT_SEQUENCE {
	CONNECT,
};

ClientManager::ClientManager() :
	m_socket( NULL ),
	m_bReady( false )
{
	OnCreate();
}

ClientManager::~ClientManager()
{
	OnDestroy();
}

void ClientManager::OnCreate()
{
}

void ClientManager::OnDestroy()
{
}

void ClientManager::Initiate()
{
}

void ClientManager::OpenSocket( const int nPort, const char* sTargetIP )
{
	// 0. Create socket
	m_socket = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( m_socket == INVALID_SOCKET ) {
		Debug::Log( "Error - Failed to create socket" );
		return;
	}
	u_long nSockOn = 1;
	ioctlsocket( m_socket, FIONBIO, &nSockOn );
	
	// 1. Set server address
	SOCKADDR_IN serverAddr;
	memset( &serverAddr, 0, sizeof( SOCKADDR_IN ) );
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons( nPort );
	inet_pton( AF_INET, sTargetIP, &serverAddr.sin_addr.S_un.S_addr );

	// 2. Connect to server
	FD_SET wset; FD_ZERO( &wset );
	wset.fd_count = 1;
	wset.fd_array[ 0 ] = m_socket;
	timeval timeval{ 5, 0 };
	const int CONNECT_SUCCESS = 1;

	Debug::Log( "Connecting to server" );
	connect( m_socket, reinterpret_cast< sockaddr* >( &serverAddr ), sizeof( SOCKADDR_IN ) );
	if ( select( 0, NULL, &wset, NULL, &timeval ) == CONNECT_SUCCESS ) {
		m_bReady = true;
		Debug::Log( "Connected to server successfully" );
	}
	else {
		closesocket( m_socket );
		Debug::Log( "Failed to connect server" );
	}
}

void ClientManager::Run()
{
	if ( !m_bReady ) {
		Debug::Log( "Not ready" );
		return;
	}

	m_tSocket = std::thread( &ClientManager::SocketThread, this );
}

void ClientManager::SocketThread()
{
	char buf[ Config::SOCKET_BUFFER_SIZE ];
	while ( true ) {
		int nReceived = recv( m_socket, buf, Config::SOCKET_BUFFER_SIZE, 0 );

		if ( nReceived == -1 ) {
			Sleep( 0 );
			continue;
		}
		else if ( nReceived == 0 ) {
			Debug::Log( "Quit socket thread : Socket closed" );
			break;
		}

		InPacket iPacket( buf );
		OnPacket( iPacket );
	}
}

void ClientManager::OnPacket( const InPacket& iPacket )
{
}

void ClientManager::SendPacket( OutPacket& oPacket )
{
	send( m_socket, oPacket.GetBuffer(), oPacket.GetBufferSize(), 0 );
}
