#include "Packet.h"

#include <cstring>

////////////////////////////////
// Incoming packet
////////////////////////////////

InPacket::InPacket( char* pBuffer )
{
	memcpy( m_buffer, pBuffer, Config::SOCKET_BUFFER_SIZE );

	m_pBufferPos = m_buffer;
}

InPacket::~InPacket()
{
}

__int64 InPacket::Decode8() const
{
	__int64* pBufferPosBefore = reinterpret_cast< __int64* >( m_pBufferPos );

	m_pBufferPos += sizeof( __int64 );

	return *pBufferPosBefore;
}

int InPacket::Decode4() const
{
	int* pBufferPosBefore = reinterpret_cast< int* >( m_pBufferPos );

	m_pBufferPos += sizeof( int );

	return *pBufferPosBefore;
}

short InPacket::Decode2() const
{
	short* pBufferPosBefore = reinterpret_cast< short* >( m_pBufferPos );

	m_pBufferPos += sizeof( short );

	return *pBufferPosBefore;
}

char InPacket::Decode1() const
{
	char* pBufferPosBefore = reinterpret_cast< char* >( m_pBufferPos );

	m_pBufferPos += sizeof( char );

	return *pBufferPosBefore;
}

std::wstring InPacket::DecodeStr() const
{
	int nLength = Decode4();
	std::wstring sVal( nLength, 0 );
	for ( int idx = 0; idx < nLength; ++idx ) {
		sVal[ idx ] = Decode2();
	}

	return sVal;
}

////////////////////////////////
// Outgoing packet
////////////////////////////////

OutPacket::OutPacket() :
	m_pBufferPos( m_buffer )
{
}

OutPacket::~OutPacket()
{
}

char* OutPacket::GetBuffer()
{
	return m_buffer;
}

int OutPacket::GetBufferSize()
{
	return m_pBufferPos - m_buffer;
}

void OutPacket::Encode8( __int64 val )
{
	__int64* pBuffer = reinterpret_cast< __int64* >( m_pBufferPos );
	*pBuffer = val;

	m_pBufferPos += sizeof( __int64 );
}

void OutPacket::Encode4( int val )
{
	int* pBuffer = reinterpret_cast< int* >( m_pBufferPos );
	*pBuffer = val;

	m_pBufferPos += sizeof( int );
}

void OutPacket::Encode2( short val )
{
	short* pBuffer = reinterpret_cast< short* >( m_pBufferPos );
	*pBuffer = val;

	m_pBufferPos += sizeof( short );
}

void OutPacket::Encode1( char val )
{
	*m_pBufferPos = val;

	m_pBufferPos += sizeof( char );
}

void OutPacket::EncodeStr( const std::wstring& val )
{
	Encode4( val.size() );
	for ( wchar_t wc : val ) {
		Encode2( wc );
	}
}
