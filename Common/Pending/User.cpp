#include "User.h"

#include "Client.h"
#include "Database.h"
#include "Protocol_Client.h"
#include "Packet.h"

User::User( int nAID, Client* pClient ) :
	m_pClient( pClient )
{
	OnCreate();
}

User::~User()
{
	OnDestroy();
}

void User::OnCreate()
{
	auto pUserCache = Database::GetInstance()->GetCache_User( m_pClient->GetAID() );

	OutPacket oPacket;
	oPacket.Encode4( S0_USER );
	oPacket.Encode4( S1_USER_SYNC_COMMODITY );
	oPacket.Encode4( pUserCache->nCoin );
	oPacket.Encode4( pUserCache->nGem );
	SendPacket( oPacket );
}

void User::OnDestroy()
{
}

void User::Update()
{
}

void User::OnPacket( const InPacket& iPacket )
{
	int eType = iPacket.Decode4();
	/*switch ( eType ) {
		case 
	}*/
}

void User::SendPacket( OutPacket& oPacket )
{
	m_pClient->SendPacket( oPacket );
}
