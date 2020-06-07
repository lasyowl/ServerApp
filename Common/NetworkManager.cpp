#include "StdafxCommon.h"
#include "NetworkManager.h"

void NetworkManager::Initiate()
{
	// 0. Startup
	WSADATA wsaData;
	WORD wVersion = MAKEWORD( 2, 2 );
	int nResult = WSAStartup( wVersion, &wsaData );
	if ( nResult != 0 ) {
		Debug::Log( "Error - Failed to initiate network" );
		return;
	}
}

void NetworkManager::Terminate()
{
	WSACleanup();
}
