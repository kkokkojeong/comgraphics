/*!
	\file
		WSock.cpp

	\brief
		Inline routines
*/

#include "WSock.h"


/*!
	\brief
		Display error about socket
	\param
		terminate - if true, terminate program
*/
void DisplayError(const char *msg, bool terminate /* = false */)
{
  cout << msg << endl;
	//fputs(msg,  stderr);
	//fputc('\n', stderr);

	if( terminate )
		exit(1);
}

/*!
	\brief
		Receive
*/
int Recvn(int s, void *buf, int len, int flags)
{
	int received;
	void *ptr = buf;
	int left = len;

	while( 0 < left) {
		received = recv(s, ptr, left, flags);

		if(received == -1)
			return -1;
		else if(received == 0)
			break;

		left -= received;
		//ptr += received;
	}
  
  return (len - left);
}


/*------------------------------------------------------------------------------
		WTCPSock Class Implementation
------------------------------------------------------------------------------*/

/*!
	\brief
		Constructor
*/
WTCPSock::WTCPSock( bool bInitWSA /* = true */ )
	: m_bInitWSA(bInitWSA)
{
//	if( m_bInitWSA ) {
//		WSAStartup( MAKEWORD(2, 2), &m_wsa);
//	}
}

/*!
	\brief
		Destructor
*/
WTCPSock::~WTCPSock()
{
//	closesocket( m_sock );		///< close socket

//	if( m_bInitWSA )
//		WSACleanup();

	close( m_sock );		///< close socket
}

/*!
	\brief
		Create socket
*/
void WTCPSock::CreateSock()
{
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if( m_sock == -1 ) {
		DisplayError("socket()", true);
	}
}

/*!
	\brief
		Set socket option to reuse address
*/
void WTCPSock::ReuseAddre()
{
	assert(m_sock != -1);

//	bool	optval = true;
	int optval = 1024 * 3;
	int retval = setsockopt(m_sock,
								SOL_SOCKET,
								SO_REUSEADDR,
								(void *)&optval,
								sizeof(optval));
	if( retval == -1 )
		DisplayError("setsockopt", false);
}

/*!
	\brief
		Get the socket
*/
int WTCPSock::GetSocket()
{
	return m_sock;
}

/*!
	\brief
		Get the socket address
*/
struct sockaddr_in *WTCPSock::GetServerAddr()
{
	return &m_addr;
}


/*------------------------------------------------------------------------------
		WTCP Server Socket Class Implementation
------------------------------------------------------------------------------*/
/*!
 	 \brief
 	 	 Constructor
 */
WTCPSockServer::WTCPSockServer()
: WTCPSock()
{
	CreateSock();	///< Creates server socket
	m_clientSock = INVALID_SOCKET;
	memset( &m_clientAddr, 0, sizeof(m_clientAddr) );
	//ZeroMemory( &m_clientAddr, sizeof(m_clientAddr) );
}

/*!
 	 \brief
 	 	 Destructor
 */
WTCPSockServer::~WTCPSockServer()
{
	close( m_clientSock );
}

/*!
	\brief
		Bind
*/
void WTCPSockServer::Bind()
{
	//ZeroMemory( &m_addr, sizeof(m_addr) );
	memset( &m_addr, 0, sizeof(m_addr) );
	m_addr.sin_family		= AF_INET;
	m_addr.sin_port			= htons(9000);
	m_addr.sin_addr.s_addr	= htonl( INADDR_ANY );

	if( bind( m_sock, (struct sockaddr *)&m_addr, sizeof(m_addr) ) == -1 )
		DisplayError( "bind()", true );
}

/*!
	\brief
		Listen
*/
void WTCPSockServer::Listen()
{
	if( listen(m_sock, SOMAXCONN) == -1 )
		DisplayError( "listen()", true );
}

/*!
	\brief
		Accept
*/
bool WTCPSockServer::Accept()
{
	socklen_t addr_len = sizeof(m_clientAddr);
	m_clientSock = accept( m_sock, (struct sockaddr *)&m_clientAddr, &addr_len	);

	if( m_clientSock == INVALID_SOCKET )
	{
		DisplayError( "accept()", false );
		return false;
	}
	return true;
}

/*!
	\brief
		Gets the client socket
*/
int WTCPSockServer::GetClientSocket()
{
	assert( m_clientSock != INVALID_SOCKET );
	return m_clientSock;
}

/*!
	\brief
		Gets the client address
*/
struct sockaddr_in *WTCPSockServer::GetClientAddr()
{
	return &m_clientAddr;
}

/*------------------------------------------------------------------------------
		WTCP Client Socket Class Implementation
------------------------------------------------------------------------------*/
/*!
	\brief
		Constructor
*/
WTCPSockClient::WTCPSockClient()
: WTCPSock(false)
{
	CreateSock();	///< Creates client socket
}

/**
	\brief
		Destructor
*/
WTCPSockClient::~WTCPSockClient()
{

}

/*!
	\brief
		Connect to server
*/
bool WTCPSockClient::Connect()
{
	int retval = connect(m_sock, (struct sockaddr *)&m_addr, sizeof(m_addr));

	if(retval == -1) {
		DisplayError( "connect()", false );
		return false;
	}
	return true;
}



