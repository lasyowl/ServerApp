#include "OAuth2Listener.h"

OAuth2Listener::OAuth2Listener( web::uri listen_uri, std::shared_ptr<web::http::oauth2::experimental::oauth2_config> pConfig ) :
	m_pListener( new web::http::experimental::listener::http_listener( listen_uri ) ), 
	m_pConfig( pConfig )
{
	m_pListener->support( [ this ] ( web::http::http_request request ) {
		if ( request.request_uri().path() == U( "/" ) && !request.request_uri().query().empty() )
		{
			m_lock.lock();

			ucout << request.request_uri().to_string() << std::endl;
			m_pConfig->token_from_redirected_uri( request.request_uri() )
				.then( [ this, request ] ( pplx::task<void> token_task ) {
				try
				{
					token_task.wait();
					m_tce.set( true );
				}
				catch ( const web::http::http_exception& e )
				{
					ucout << "Error: " << e.what() << std::endl;
					m_tce.set( false );
				}
			} );

			request.reply( web::http::status_codes::OK, U( "Ok." ) );

			m_lock.unlock();
		}
		else
		{
			request.reply( web::http::status_codes::NotFound, U( "Not found." ) );
		}
	} );

	m_pListener->open().wait();
}

OAuth2Listener::~OAuth2Listener()
{
	printf( "close listener\n" );
	m_pListener->close().wait();
}

OAuth2Listener::OAuth2Listener( OAuth2Listener&& right )
{
	m_pListener = right.m_pListener;
	m_pConfig = right.m_pConfig;
}

pplx::task<bool> OAuth2Listener::ListenCode()
{
	return pplx::create_task( m_tce );
}