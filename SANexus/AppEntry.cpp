#include "Stdafx.h"

int main( int argc, char* argv[] )
{
	g_db.Initiate();

	NetworkManager::Initiate();

	SYSTEM_INFO systemInfo;
	GetSystemInfo( &systemInfo );
	g_sm.Initiate( systemInfo.dwNumberOfProcessors * 2, Config::MAX_CONCURRENT_CLIENT );
	g_sm.OpenSocket( Config::SERVER_NEXUS_LOGIN_PORT, Config::SERVER_LOGIN_IP );
	g_sm.Run();

	while ( 1 );

	NetworkManager::Terminate();
}
