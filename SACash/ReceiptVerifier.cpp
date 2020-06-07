#include "ReceiptVerifier.h"

#include "OAuth2Listener.h"
#include "Packet.h"

#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>
#include <mutex>
#include <windows.h>
#include <Shellapi.h>

#ifdef _DEBUG
#pragma comment( lib, "cpprest140d_2_6" )
#else
#pragma comment( lib, "cpprest140_2_6" )
#endif

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;

namespace {
	const utility::string_t g_sKey = L"340527650098-7fcdi347c7abe8p6sj3cmrqk95oigsum.apps.googleusercontent.com";
	const utility::string_t g_sSecretKey = L"w01Xt_gwRxqMWYzc5pd3StWC";
	const utility::string_t g_sScope = L"https://www.googleapis.com/auth/androidpublisher";
	const utility::string_t g_sAuthEndPoint = L"https://accounts.google.com/o/oauth2/v2/auth";
	const utility::string_t g_sAccessTokenEndPoint = L"https://www.googleapis.com/oauth2/v4/token";
	const utility::string_t g_sRefreshTokenEndPoint = L"https://accounts.google.com/o/oauth2/token";
	const utility::string_t g_sRedirectURI = L"http://localhost:8889/";
	const utility::string_t g_sVerifyURI = L"https://www.googleapis.com/androidpublisher/v3/applications/";

	// Client - ReceiptVerifier.cs
	enum PacketType_Recv {
		VERIFY
	};
	// Client - ServerConnection.cs
	enum PacketType_Send {
		RESULT_RECEIPT,
	};
}

ReceiptVerifier::ReceiptVerifier() :
	m_bRefreshToken( false )
{
	OnCreate();
}

ReceiptVerifier::~ReceiptVerifier()
{
}

void ReceiptVerifier::OnCreate()
{
	InitOAuth();
	RequestToken();
}

void ReceiptVerifier::Update( clock_t tCur )
{
	if ( !m_pOAuthConfig ) return;

	if ( m_bRefreshToken && tCur > m_tRefreshToken ) {
		m_bRefreshToken = false;
		RefreshToken();
	}
}

void ReceiptVerifier::OnPacket( const InPacket& iPacket )
{
	int eType = iPacket.Decode4();
	switch ( eType )
	{
		case PacketType_Recv::VERIFY:
		{
			std::wstring sPackageName = iPacket.DecodeStr();
			PurchaseType ePurchaseType = static_cast< PurchaseType >( iPacket.Decode4() );
			std::wstring sProductID = iPacket.DecodeStr();
			std::wstring sPurchaseToken = iPacket.DecodeStr();
			VerifyReceipt( sPackageName,
						   ePurchaseType,
						   sProductID,
						   sPurchaseToken );
			break;
		}
	}
}

void ReceiptVerifier::InitOAuth()
{
	m_pOAuthConfig = std::make_shared<oauth2_config>( g_sKey,
													  g_sSecretKey,
													  g_sAuthEndPoint,
													  g_sAccessTokenEndPoint,
													  g_sRedirectURI,
													  g_sScope  );
	m_pOAuthListener = std::make_shared<OAuth2Listener>( g_sRedirectURI,
														 m_pOAuthConfig );
}

void ReceiptVerifier::OpenBrowserOAuth()
{
	utility::string_t sURI( m_pOAuthConfig->build_authorization_uri( true ) );

	auto r = ShellExecuteA( NULL,
							"open",
							utility::conversions::utf16_to_utf8( sURI ).c_str(),
							NULL,
							NULL,
							SW_SHOWNORMAL );
}

void ReceiptVerifier::RequestToken()
{
	if ( !m_pOAuthListener || !m_pOAuthConfig ) return;
	if ( m_pOAuthConfig->client_key().empty() || m_pOAuthConfig->client_secret().empty() ) return;
	
	OpenBrowserOAuth();

	bool bSuccess = m_pOAuthListener->ListenCode().get();

	if ( bSuccess ) {
		const oauth2_token& token = m_pOAuthConfig->token();

		std::cout << "Autorization success!!" << std::endl;
		DisplayTokenInfo( token );
		
		m_tRefreshToken = clock() + token.expires_in() * 1000 / 2;
		m_bRefreshToken = true;
	}
	else
		printf( "Authorization failed\n" );
}

void ReceiptVerifier::RefreshToken()
{
	if ( !m_pOAuthConfig ) return;

	oauth2_token token = m_pOAuthConfig->token();

	// 0. Display current token info
	std::cout << "Refreshing access token..." << std::endl;
	DisplayTokenInfo( token );

	utility::string_t sRefreshToken = token.refresh_token();

	// 1. Send refresh token request
	m_pOAuthConfig->token_from_refresh().then( [ = ] ( pplx::task<void> task ) mutable {
		try {
			task.wait();

			// 2. Reserve token info
			const oauth2_token& newToken = m_pOAuthConfig->token();
			token.set_access_token( newToken.access_token() );
			m_pOAuthConfig->set_token( token );

			// 3. Display refreshed token info
			std::wcout << L"- AccessToken ( NEW ): " << token.access_token() << std::endl;
			std::cout << "==========================" << std::endl;

			m_tRefreshToken = clock() + token.expires_in() * 1000 / 2;
			m_bRefreshToken = true;
		}
		catch ( std::exception e ) {
			std::cout << e.what() << std::endl;
		}
	} );
}

void ReceiptVerifier::DisplayTokenInfo( const oauth2_token& token )
{
	std::cout << "==========================" << std::endl;
	std::wcout << L"- AccessToken: " << token.access_token() << std::endl;
	std::wcout << L"- RefreshToken: " << token.refresh_token() << std::endl;
	std::wcout << L"- Scope: " << token.scope() << std::endl;
	std::wcout << L"- IsValid: " << token.is_valid_access_token() << std::endl;
	std::wcout << L"- ExpiresIn: " << token.expires_in() << std::endl;
	std::wcout << L"- TokenType: " << token.token_type() << std::endl;
	std::cout << "==========================" << std::endl;
}

bool ReceiptVerifier::VerifyReceipt( const std::wstring& sPackageName, const PurchaseType& ePurchaseType, const std::wstring& sProductID, const std::wstring& sPurchaseToken )
{
	if ( !m_pOAuthConfig ) return false;

	const oauth2_token& token = m_pOAuthConfig->token();
	if ( !token.is_valid_access_token() ) return false;

	std::cout << "Verifying receipt..." << std::endl;
	std::wstring sURI = g_sVerifyURI;
	sURI.append( sPackageName );
	switch ( ePurchaseType ) {
		case PurchaseType::PRODUCT:		sURI.append( L"/purchases/products/" ); break;
		case PurchaseType::SUBSCRIPT:	sURI.append( L"/purchases/subscribe/" ); break;
	}
	sURI.append( sProductID );
	sURI.append( L"/tokens/" );
	sURI.append( sPurchaseToken );
	sURI.append( L"?access_token=" );
	sURI.append( token.access_token() );

	http_client client( sURI );
	client.request( methods::GET ).then( [ = ] ( http_response response ) {
		std::wcout << L"====================" << std::endl;
		std::wcout << response.to_string() << std::endl;
		std::wcout << L"====================" << std::endl;
	} );
	
	return true;
}
