#include "Stdafx.h"
#include "QueryBuilder.h"

namespace QueryBuilder {

	///////////////////////////////
	// Base class for query builder
	///////////////////////////////
	void QueryBase::SetTable( std::string sTable )
	{
		m_sTable = sTable;
	}

	std::string QueryBase::MakeDBString( const std::string& sValue )
	{
		std::string sDBValue;
		sDBValue.append( "'" );
		sDBValue.append( sValue );
		sDBValue.append( "'" );

		return sDBValue;
	}

	std::string QueryBase::MakeDBString( int nValue )
	{
		return std::to_string( nValue );
	}

	///////////////////////////////
	// Query builder : SELECT
	///////////////////////////////
	std::string Select::Get() const
	{
		if ( m_sTable.empty() ) return nullptr;

		std::string sConditions;
		if ( !m_lCondition.empty() ) {
			sConditions.append( "WHERE " );
			for ( auto iter = m_lCondition.begin(); iter != m_lCondition.end(); ) {
				sConditions.append( iter->first );
				sConditions.append( "=" );
				sConditions.append( iter->second );
				++iter;
				if ( iter != m_lCondition.end() ) sConditions.append( " AND " );
			}
		}

		char sQuery[ Config::QUERY_BUFFER_SIZE ];
		sprintf_s( sQuery, "SELECT %s FROM %s %s", m_sField.empty() ? "*" : m_sField.c_str(), m_sTable.c_str(), sConditions.c_str() );

		return sQuery;
	}

	void Select::Reset()
	{
		m_sTable.clear();
		m_sField.clear();
		m_lCondition.clear();
	}

	void Select::SetField( const std::string& sField )
	{
		m_sField = sField;
	}

	///////////////////////////////
	// Query builder : INSERT
	///////////////////////////////
	std::string Insert::Get() const
	{
		if ( m_sTable.empty() ) return nullptr;

		std::string sFields;
		std::string sValues;
		for ( auto iter = m_lField.begin(); iter != m_lField.end(); ) {
			sFields.append( iter->first );
			sValues.append( iter->second );
			++iter;
			if ( iter != m_lField.end() ) {
				sFields.append( "," );
				sValues.append( "," );
			}
		}

		char sQuery[ Config::QUERY_BUFFER_SIZE ];
		sprintf_s( sQuery, "INSERT INTO %s (%s) VALUES (%s)", m_sTable.c_str(), sFields.c_str(), sValues.c_str() );

		return sQuery;
	}

	void Insert::Reset()
	{
		m_sTable.clear();
		m_lField.clear();
	}

	///////////////////////////////
	// Query builder : UPDATE
	///////////////////////////////
	std::string Update::Get() const
	{
		if ( m_sTable.empty() ) return nullptr;

		std::string sValues;
		for ( auto iter = m_lField.begin(); iter != m_lField.end(); ) {
			sValues.append( iter->first );
			sValues.append( "=" );
			sValues.append( iter->second );
			++iter;
			if ( iter != m_lField.end() ) sValues.append( "," );
		}

		std::string sConditions;
		for ( auto iter = m_lCondition.begin(); iter != m_lCondition.end(); ) {
			sConditions.append( iter->first );
			sConditions.append( "=" );
			sConditions.append( iter->second );
			++iter;
			if ( iter != m_lCondition.end() ) sConditions.append( " AND " );
		}

		char sQuery[ Config::QUERY_BUFFER_SIZE ];
		sprintf_s( sQuery, "UPDATE %s SET %s WHERE %s", m_sTable.c_str(), sValues.c_str(), sConditions.c_str() );

		return sQuery;
	}

	void Update::Reset()
	{
		m_sTable.clear();
		m_lField.clear();
		m_lCondition.clear();
	}

	///////////////////////////////
	// Query builder : DELETE
	///////////////////////////////
	std::string Delete::Get() const
	{
		if ( m_sTable.empty() ) return nullptr;

		std::string sConditions;
		for ( auto iter = m_lCondition.begin(); iter != m_lCondition.end(); ) {
			sConditions.append( "(" );
			sConditions.append( iter->first );
			sConditions.append( "=" );
			sConditions.append( iter->second );
			sConditions.append( ")" );
			++iter;
			if ( iter != m_lCondition.end() ) sConditions.append( " AND " );
		}

		char sQuery[ Config::QUERY_BUFFER_SIZE ];
		sprintf_s( sQuery, "DELETE FROM %s WHERE %s", m_sTable.c_str(), sConditions.c_str() );

		return sQuery;
	}

	void Delete::Reset()
	{
		m_sTable.clear();
		m_lCondition.clear();
	}

	///////////////////////////////
	// Query builder : CALL
	///////////////////////////////
	std::string Call::Get() const
	{
		if ( m_sStoredProcedure.empty() ) return nullptr;

		std::string sParameters;
		for ( auto iter = m_lParameter.begin(); iter != m_lParameter.end(); ) {
			sParameters.append( *iter );
			++iter;
			if ( iter != m_lParameter.end() ) sParameters.append( "," );
		}

		char sQuery[ Config::QUERY_BUFFER_SIZE ];
		sprintf_s( sQuery, "CALL %s(%s)", m_sStoredProcedure.c_str(), sParameters.c_str() );

		return sQuery;
	}

	void Call::Reset()
	{
		m_sTable.clear();
		m_sStoredProcedure.clear();
		m_lParameter.clear();
	}
}