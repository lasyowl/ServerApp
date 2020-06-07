#pragma once

#include <time.h>
#include <string>

#include "Singleton.h"

namespace web {
	namespace http {
		namespace oauth2 {
			namespace experimental {
				class oauth2_config;
				class oauth2_token;
			}
		}
	}
}
class OAuth2Listener;
class InPacket;
class OutPacket;

class ReceiptVerifier : public Singleton<ReceiptVerifier> {
public:
	// Client - ReceiptVerifier.cs
	enum PurchaseType {
		PRODUCT,
		SUBSCRIPT
	};

public:
	ReceiptVerifier();
	~ReceiptVerifier();

public:
	void Update( clock_t tCur );
	void OnPacket( const InPacket& iPacket );

private:
	void OnCreate();

private:
	void InitOAuth();
	void OpenBrowserOAuth();
	void RequestToken();
	void RefreshToken();
	void DisplayTokenInfo( const web::http::oauth2::experimental::oauth2_token& token );

private:
	std::shared_ptr<web::http::oauth2::experimental::oauth2_config> m_pOAuthConfig;
	std::shared_ptr<OAuth2Listener> m_pOAuthListener;

public:
	bool VerifyReceipt( const std::wstring& sPackageName, const PurchaseType& ePurchaseType, const std::wstring& sProductID, const std::wstring& sPurchaseToken );

private:
	bool	m_bRefreshToken;
	clock_t m_tRefreshToken;
};
