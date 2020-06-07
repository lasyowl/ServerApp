#pragma once

#include "Singleton.h"

#include <map>
#include <memory>

class InPacket;
class OutPacket;
class User;
class Client;

class UserPool : public Singleton<UserPool> {
public:
	UserPool();
	~UserPool();

public:
	void Update();

public:
	void OnPacket( const InPacket& iPacket );
private:
	void SendPacket( OutPacket& oPacket );

private:
	std::map<int, std::shared_ptr<User> > m_mUser;
public:
	std::shared_ptr<User> GetUser( int nAID );
	void AddUser( int nAID, Client* pClient );
	void RemoveUser( int nAID );
};