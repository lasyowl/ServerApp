#pragma once

class InPacket;
class OutPacket;
class Client;

class User {
public:
	User( int nAID, Client* pClient );
	~User();

public:
	void OnCreate();
	void OnDestroy();
	void Update();

private:
	Client* m_pClient;

public:
	void OnPacket( const InPacket& iPacket );
private:
	void SendPacket( OutPacket& oPacket );
};