#pragma once

#include "ConfigCommon.h"

#include <string>

class InPacket {
public:
	InPacket( char* pBuffer );
	~InPacket();

private:
	int				m_header;
	char			m_buffer[ Config::SOCKET_BUFFER_SIZE ];
	mutable char*	m_pBufferPos;
public:

	__int64			Decode8() const;
	int				Decode4() const;
	short			Decode2() const;
	char			Decode1() const;
	std::wstring	DecodeStr() const;
};

class OutPacket {
public:
	OutPacket();
	~OutPacket();

private:
	int		m_header;
	char	m_buffer[ Config::SOCKET_BUFFER_SIZE ];
	char*	m_pBufferPos;
public:
	char*		GetBuffer();
	unsigned	GetBufferSize();

	void Encode8( __int64 val );
	void Encode4( int val );
	void Encode2( short val );
	void Encode1( char val );
	void EncodeStr( const std::wstring& val );
};