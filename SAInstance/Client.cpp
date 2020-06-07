#include "Client.h"

#include "ServerApp.h"
#include "ReceiptVerifier.h"
#include "UserPool.h"
#include "User.h"
#include "Packet.h"
#include "Database.h"
#include "Util.h"
#include "Protocol_Client.h"

#include <iostream>

Client::Client( const SOCKET socketID ) :
	m_socketID( socketID ),
	m_nAID( 0 )
{
	OnCreate();
}

Client::~Client()
{
	OnDestroy();
}

void Client::Disconnect()
{
	OutPacket oPacket;
	oPacket.Encode4( S0_DISCONNECT );
	SendPacket( oPacket );
}

void Client::OnCreate()
{
}

void Client::OnDestroy()
{
	UserPool::GetInstance()->RemoveUser( m_nAID );
}

void Client::Update()
{
	
}

SOCKET Client::GetSocketID()
{
	return m_socketID;
}

void Client::OnPacket( const InPacket& iPacket )
{
	int eType = iPacket.Decode4();

	switch ( eType ) {
		case R0_RECEIPT:	ReceiptVerifier::GetInstance()->OnPacket( iPacket ); break;
		case R0_LOGIN:		OnRequest_Login( iPacket ); break;
		case R0_USER:		UserPool::GetInstance()->OnPacket( iPacket ); break;
	}
}

void Client::SendPacket( OutPacket& oPacket )
{
	ServerApp::GetInstance()->SendPacket( m_socketID, oPacket );
}

void Client::OnRequest_Login( const InPacket& iPacket )
{
	m_sUUID = Util::W2S( iPacket.DecodeStr() );
	m_nAID = Database::GetInstance()->GetAIDByUUID( m_sUUID );

	OutPacket oPacket;
	oPacket.Encode4( S0_LOGIN );
	oPacket.Encode1( true );
	SendPacket( oPacket );

	UserPool::GetInstance()->AddUser( m_nAID, this );
}

void Client::OnRequest_User( const InPacket& iPacket )
{
	auto pUser = UserPool::GetInstance()->GetUser( m_nAID );
	pUser->OnPacket( iPacket );
}

int Client::GetAID()
{
	return m_nAID;
}
