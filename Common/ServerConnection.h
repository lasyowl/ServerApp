#pragma once

class InPacket;
class OutPacket;
class User;

class IClientImplement {
public:
	virtual void OnPacket( const InPacket& iPacket ) = 0;
};

class ServerConnection {
public:
	ServerConnection() = delete;
	~ServerConnection() = delete;

public:
	void Initiate( const SOCKET& socket );
	void Destroy();
	void Update();

private:
	SOCKET m_socket;
public:
	SOCKET GetSocketID() { return m_socket; }

public:
	void OnPacket( const InPacket& iPacket );
	void SendPacket( OutPacket& oPacket );

private:
	std::shared_ptr<IClientImplement> m_pImplement;
	std::function<void( const InPacket& )> m_cbOnPacket;
public:
	void RegisterCallbackOnPacket( std::function<void( const InPacket& )> f );
};