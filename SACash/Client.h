#pragma once

#include <WinSock2.h>
#include <memory>
#include <string>

class InPacket;
class OutPacket;
class User;

class Client {
public:
	Client( const SOCKET socketID );
	~Client();

public:
	void Disconnect();
	void OnCreate();
	void OnDestroy();
	void Update();

private:
	SOCKET m_socketID;
public:
	SOCKET GetSocketID();

public:
	void OnPacket( const InPacket& iPacket );
	void SendPacket( OutPacket& oPacket );

private:
	std::string m_sUUID;
	int			m_nAID;
private:
	void OnRequest_Login( const InPacket& iPacket );
	void OnRequest_User( const InPacket& iPacket );
public:
	int GetAID();
};