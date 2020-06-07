#pragma once

namespace QueryBuilder{
	class QueryBase;
}
struct MYSQL;

struct QueryResult {
	bool bSuccess;
	std::list<std::map<std::string, std::string> > lmResult;

	QueryResult() :bSuccess( true ) {}
};

struct UserCache {
	int nCoin;
	int nGem;
};

class Database {
public:
	Database();
	~Database();

private:
	std::unique_ptr<MYSQL> m_pDB;
private:
	void OnCreate();
	void OnDestroy();

public:
	void Initiate();

private:
	void Connect();
	std::shared_ptr<QueryResult> Query( const QueryBuilder::QueryBase& query );
	void LoadCache_AccountInfo();
	void LoadCache_User( int nAID );

private:
	std::map<std::string, int>					m_mAID;			// <UUID, AID>
	std::map<int, std::shared_ptr<UserCache> >	m_mUserCache;	// <AID, Cache>
private:
	std::string CreateUUID();
public:
	int GetAIDByUUID( const std::string& sUUID );
	std::shared_ptr<UserCache> GetCache_User( int nAID );

public:
	bool DBCreateNewAccount();
	int DBGetCoin( int nAID );
	int DBGetGem( int nAID );
	bool DBIncCoin( int nAID, int nInc );
	bool DBIncGem( int nAID, int nInc );
};