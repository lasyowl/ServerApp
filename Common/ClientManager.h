#pragma once

class InPacket;
class OutPacket;

class ClientManager {
public:
	ClientManager();
	~ClientManager();

private:
	void OnCreate();
	void OnDestroy();

private:
	bool	m_bReady;
	SOCKET	m_socket;
	std::thread m_tSocket;
public:
	void Initiate();
	void OpenSocket( const int nPort, const char* sTargetIP );
	void Run();

private:
	void SocketThread();

private:
	void OnPacket( const InPacket& iPacket );
	void SendPacket( OutPacket& oPacket );

	// Misc functions
private:
};