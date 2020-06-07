#pragma once

#include "ConfigCommon.h"

class InPacket;
class OutPacket;
class ServerConnection;
class ServerManager;
enum SOCKET_INIT_SEQUENCE;

////////////////////////////////////
// How to use
// 0. Initiate()
// 1. OpenSocket()
// 2. Run()
////////////////////////////////////

////////////////////////////////////
// Socket structure
////////////////////////////////////
struct SOCKETINFO {
	SOCKETINFO()
	{
		memset( this, 0, sizeof( SOCKETINFO ) );

		dataBuffer.len = Config::SOCKET_BUFFER_SIZE;
		dataBuffer.buf = msgBuffer;
	}

	void ClearBuffer()
	{
		memset( msgBuffer, 0, Config::SOCKET_BUFFER_SIZE );
		dataBuffer.len = Config::SOCKET_BUFFER_SIZE;
		dataBuffer.buf = msgBuffer;
	}

	WSAOVERLAPPED	overlapped;
	WSABUF			dataBuffer;
	SOCKET			socket;
	char			msgBuffer[ Config::SOCKET_BUFFER_SIZE ];
	int				nIdx;
};
struct WORKERPARAM {
	HANDLE			hThread;
	ServerManager*	pServerManager;
};

class ServerManager {
public:
	ServerManager();
	~ServerManager();

private:
	void OnCreate();
	void OnDestroy();

private:
	bool	m_bReady;
	SOCKET	m_sockListener;
	WORKERPARAM	m_workerParam;
	std::thread m_tListener;
	std::mutex				m_lockSockInfo;
	std::vector<SOCKETINFO> m_aSockInfo;
	std::queue<int>			m_qSockIdxReady;
	std::set<int>			m_sSockIdxAssigned;
public:
	void Initiate( const int nWorkerThreadCount, const int nConnectionCount );
	void OpenSocket( const int nPort, const char* sTargetIP = nullptr );
	void Run();

private:
	void UpdateThread();
	void AcceptThread();
	static DWORD WINAPI WorkerThread( LPVOID hIOCP );

private:
	std::mutex	m_lockClient;
	std::thread m_tUpdate;
	std::unordered_map<SOCKET, std::shared_ptr<ServerConnection>> m_mClient;
private:
	void OnPacket( const SOCKET& socket, const InPacket& iPacket );
	void OnSocketAccept( const SOCKET& socket );
	void OnSocketClose( const SOCKET& socket );

public:
	void SendPacket( const SOCKET& socketID, OutPacket& oPacket );

// Misc functions
private:
	bool	HandleSocketError( const int nResult, enum SOCKET_INIT_SEQUENCE eSequence, const SOCKET socket );
	SOCKETINFO& PopSocketReady();
	void		PushSocketReady( const int nIdx );
};