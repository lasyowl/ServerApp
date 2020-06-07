#pragma once

#include <memory>

#include "cpprest/http_client.h"
#include "cpprest/http_listener.h"

class OAuth2Listener
{
public:
	OAuth2Listener( web::uri listen_uri, std::shared_ptr<web::http::oauth2::experimental::oauth2_config> pConfig );
	~OAuth2Listener();

	OAuth2Listener( OAuth2Listener&& right );

public:
	pplx::task<bool> ListenCode();

private:
	std::shared_ptr<web::http::experimental::listener::http_listener>	m_pListener;
	std::shared_ptr<web::http::oauth2::experimental::oauth2_config> 	m_pConfig;

	pplx::task_completion_event<bool> m_tce;

	std::mutex m_lock;
};