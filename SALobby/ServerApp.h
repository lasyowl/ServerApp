#pragma once

#include <WinSock2.h>
#include <map>
#include <memory>

#include "Singleton.h"

class Client;
class InPacket;
class OutPacket;

class ServerApp : public Singleton<ServerApp> {
public:
	ServerApp();
	~ServerApp();

private:
	void OnCreate();
	void OnDestroy();

public:
	void Run();

private:
	SOCKET m_sockListener;
	HANDLE m_hIOCP;
private:
	void InitSocket();
	void UpdateClient();
	void CloseSocket();

private:
	static DWORD WINAPI WorkerThread( LPVOID hIOCP );

	void AcceptThread();

	static void OnReceivePacket( const SOCKET& socketID, const InPacket& iPacket );

private:
	std::map<SOCKET, std::shared_ptr<Client> >	m_mpClient;
	std::mutex									m_lockMapClient;
public:
	void SendPacket( const SOCKET& socketID, OutPacket& oPacket );

	void CreateClientConnection( const SOCKET& socketID );
	void DestroyClientConnection( const SOCKET& socketID );
	std::shared_ptr<Client> GetClient( const SOCKET& socketID );
};