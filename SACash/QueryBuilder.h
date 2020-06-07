#pragma once

#include "Config.h"

#include <string>
#include <list>

namespace QueryBuilder {

	///////////////////////////////
	// Base class for query builder
	///////////////////////////////
	class QueryBase {
	protected:
		QueryBase() {}
		
	protected:
		std::string m_sTable;
	public:
		virtual std::string Get() const = 0;
		virtual void Reset() = 0;
		virtual void SetTable( std::string sTable );
	protected:
		std::string MakeDBString( const std::string& sValue );
		std::string MakeDBString( int nValue );
	};

	///////////////////////////////
	// Query builder : SELECT
	///////////////////////////////
	class Select : public QueryBase {
	private:
		std::string m_sField;
		std::list<std::pair<std::string, std::string> > m_lCondition;	// <field, value>
	public:
		virtual std::string Get() const override;
		virtual void Reset() override;
		void SetField( const std::string& sField );

		template<typename Ty>
		void AddConditionPair( const std::string& sField, const Ty& value )
		{
			m_lCondition.push_back( std::make_pair( sField, MakeDBString( value ) ) );
		}
	};

	///////////////////////////////
	// Query builder : INSERT
	///////////////////////////////
	class Insert : public QueryBase {
	private:
		std::list<std::pair<std::string, std::string> > m_lField;		// <field, value>
	public:
		virtual std::string Get() const override;
		virtual void Reset() override;

		template<typename Ty>
		void AddValuePair( const std::string& sField, const Ty& value )
		{
			m_lField.push_back( std::make_pair( sField, MakeDBString( value ) ) );
		}
	};

	///////////////////////////////
	// Query builder : UPDATE
	///////////////////////////////
	class Update : public QueryBase {
	private:
		std::list<std::pair<std::string, std::string> > m_lField;		// <field, value>
		std::list<std::pair<std::string, std::string> > m_lCondition;	// <field, value>
	public:
		virtual std::string Get() const override;
		virtual void Reset() override;

		template<typename Ty>
		void AddConditionPair( const std::string& sField, const Ty& value )
		{
			m_lCondition.push_back( std::make_pair( sField, MakeDBString( value ) ) );
		}
		template<typename Ty>
		void AddValuePair( const std::string& sField, const Ty& value )
		{
			m_lField.push_back( std::make_pair( sField, MakeDBString( value ) ) );
		}
	};

	///////////////////////////////
	// Query builder : DELETE
	///////////////////////////////
	class Delete : public QueryBase {
	private:
		std::list<std::pair<std::string, std::string> > m_lCondition; // <field, value>
	public:
		virtual std::string Get() const override;
		virtual void Reset() override;

		template<typename Ty>
		void AddConditionPair( const std::string& sField, const Ty& value )
		{
			m_lCondition.push_back( std::make_pair( sField, MakeDBString( value ) ) );
		}
	};

	///////////////////////////////
	// Query builder : CALL
	///////////////////////////////
	class Call : public QueryBase {
	private:
		std::string m_sStoredProcedure;
		std::list<std::string> m_lParameter;
	public:
		virtual std::string Get() const override;
		virtual void Reset() override;

		template<typename Ty, typename...Args>
		void SetStoredProcedure( const std::string& sStoredProcedure, Ty ty, Args...args )
		{
			m_sStoredProcedure = sStoredProcedure;
			SetStoredProcedure( ty, args... );
		}
	private:
		template<typename Ty, typename...Args>
		void SetStoredProcedure( Ty ty, Args...args )
		{
			m_lParameter.push_back( MakeDBString( ty ) );
			SetStoredProcedure( args... );
		}
		void SetStoredProcedure() {}
	};
}