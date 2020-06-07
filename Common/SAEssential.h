#pragma once

#include "Packet.h"
#include "MemoryPool.h"
#include "NetworkManager.h"
#include "ServerConnection.h"
#include "ServerManager.h"
#include "ClientManager.h"

#pragma comment( lib, "Ws2_32.lib" )

#include <ws2tcpip.h>

static ServerManager g_sm;
static MemoryPool g_mp;