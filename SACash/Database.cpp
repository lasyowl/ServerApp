#include "Database.h"

#include "Config.h"
#include "QueryBuilder.h"

#include <mysql.h>
#include <iostream>
#include <rpc.h>

#pragma comment(lib, "Rpcrt4.lib")

namespace {
	const char* g_sDBHost			= "localhost";
	const char* g_sDBUserID			= "root";
	const char* g_sDBUserPassword	= "dyfnekd1";
	const char* g_sDBTable			= "projectz";
	const int	g_nDBPort			= 3306;

	const char* g_sDBTable_AccountInfo = "accountinfo";
	const char* g_sDBTable_AccountData_Commodity = "accountdata_commodity";
}

Database::Database()
{
	OnCreate();
}

Database::~Database()
{
	OnDestroy();
}

void Database::OnCreate()
{
	m_pDB = std::make_unique<MYSQL>();

	Connect();

	LoadCache_AccountInfo();
}

void Database::OnDestroy()
{
	if ( m_pDB ) mysql_close( m_pDB.get() );
}

void Database::Connect()
{
	mysql_init( m_pDB.get() );

	bool bSuccess = mysql_real_connect( m_pDB.get(),
										g_sDBHost,
										g_sDBUserID,
										g_sDBUserPassword,
										g_sDBTable,
										g_nDBPort,
										nullptr,
										0 );

	if ( !bSuccess ) {
		printf( "error\n" );
		return;
	}
	else
		printf( "success\n" );

	/*QueryBuilder::Select qb;
	qb.SetTable( "test" );
	auto aa = Query( qb.Get() );*/

	/*QueryBuilder::INSERT qbi;
	qbi.SetTable( "test" );
	qbi.AddValue( 4083 );
	qbi.AddValue( "HAWI" );
	Query( qbi.Get() );*/

	/*QueryBuilder::UPDATE qbu;
	qbu.SetTable( "test" );
	qbu.AddValuePair( "TEST", "howi" );
	qbu.AddValuePair( "TWO", "hs" );
	qbu.AddConditionPair( "ID", 4080 );
	qbu.AddConditionPair( "TEST", "12" );
	Query( qbu.Get() );*/

	/*QueryBuilder::QDELETE qbd;
	qbd.SetTable( "test" );
	qbd.AddConditionPair( "ID", 4083 );
	qbd.AddConditionPair( "TEST", "HAWI" );
	Query( qbd.Get() );*/
}

std::shared_ptr<QueryResult> Database::Query( const QueryBuilder::QueryBase& query )
{
	std::shared_ptr<QueryResult> pResult( new QueryResult() );

	std::string sQuery = query.Get();
	std::cout << sQuery << std::endl;
	
	int query_stat = mysql_query( m_pDB.get(), sQuery.c_str() );
	if ( query_stat != 0 ) {
		printf( "mysql error : %s\n", mysql_error( m_pDB.get() ) );
		pResult->bSuccess = false;
		return pResult;
	}
	
	MYSQL_RES* pRes = mysql_store_result( m_pDB.get() );
	if ( !pRes ) {
		return pResult;
	}
	// Split rows
	for ( int iter = 0; iter < mysql_num_rows( pRes ); ++iter ) {
		std::map<std::string, std::string> mResult;
		MYSQL_ROW row = mysql_fetch_row( pRes );

		// Split values by field
		for ( int fieldIdx = 0; fieldIdx < mysql_num_fields( pRes ); ++fieldIdx ) {
			std::string sField = pRes->fields[ fieldIdx ].name;
			std::string sValue = row[ fieldIdx ] ? row[ fieldIdx ] : "NULL";

			mResult.emplace( sField, sValue );
		}

		pResult->lmResult.push_back( mResult );
	}

	return pResult;
}

void Database::LoadCache_AccountInfo()
{
	QueryBuilder::Select qb;
	qb.SetTable( g_sDBTable_AccountInfo );
	auto pResult = Query( qb );

	for ( auto& mResult : pResult->lmResult ) {
		m_mAID[ mResult[ "UUID" ] ] = std::stoi( mResult[ "AID" ] );
	}
}

std::string Database::CreateUUID()
{
	UUID uuid;
	if ( UuidCreate( &uuid ) != RPC_S_OK ) 
		return std::string();
	
	RPC_CSTR sRPC;
	if ( UuidToStringA( &uuid, &sRPC ) != RPC_S_OK )
		return std::string();

	std::string sUUID = ( char* ) sRPC;

	return sUUID;
}

int Database::GetAIDByUUID( const std::string& sUUID )
{
	if ( m_mAID.find( sUUID ) == m_mAID.end() ) return 0;

	return m_mAID[ sUUID ];
}

std::shared_ptr<UserCache> Database::GetCache_User( int nAID )
{
	if ( m_mUserCache.find( nAID ) == m_mUserCache.end() ) {
		LoadCache_User( nAID );
	}

	return m_mUserCache[ nAID ];
}

void Database::LoadCache_User( int nAID )
{
	std::shared_ptr<UserCache> pUserCache( new UserCache() );
	pUserCache->nCoin = DBGetCoin( nAID );
	pUserCache->nGem = DBGetGem( nAID );

	m_mUserCache.emplace( std::make_pair( nAID, pUserCache ) );
}

bool Database::DBCreateNewAccount()
{
	std::string sUUID = CreateUUID();

	QueryBuilder::Call qbc;
	qbc.SetStoredProcedure( std::string( "CreateNewAccount" ), sUUID );
	auto pResult = Query( qbc );

	return pResult->bSuccess;
}

int Database::DBGetCoin( int nAID )
{
	QueryBuilder::Select qb;
	qb.SetTable( g_sDBTable_AccountData_Commodity );
	qb.AddConditionPair( "AID", nAID );

	auto pResult = Query( qb );
	if ( !pResult->bSuccess || pResult->lmResult.empty() ) return 0;

	auto mResult = pResult->lmResult.front();
	std::string sResult = mResult.at( "Coin" );

	return std::atoi( sResult.c_str() );
}

int Database::DBGetGem( int nAID )
{
	QueryBuilder::Select qbs;
	qbs.SetTable( g_sDBTable_AccountData_Commodity );
	qbs.AddConditionPair( "AID", nAID );

	auto pResult = Query( qbs );
	if ( !pResult->bSuccess || pResult->lmResult.empty() ) return 0;

	auto mResult = pResult->lmResult.front();
	std::string sResult = mResult.at( "Gem" );

	return std::atoi( sResult.c_str() );
}

bool Database::DBIncCoin( int nAID, int nInc )
{
	QueryBuilder::Call qbc;
	qbc.SetStoredProcedure( std::string( "IncCoin" ), nAID, nInc );
	Query( qbc );

	return true;
}

bool Database::DBIncGem( int nAID, int nInc )
{
	QueryBuilder::Call qbc;
	qbc.SetStoredProcedure( std::string( "IncGem" ), nAID, nInc );
	Query( qbc );

	return false;
}
