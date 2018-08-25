
/*!
	\file
		WMPI.h

	\brief
		a class for TCP/IP socket
*/

#ifndef WSOCK_H
#define WSOCK_H

#ifdef _WIN32					///< Windows
#include <winsock2.h>
#else							///< Linux
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#endif

#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

///< Message Type
enum MessageType{
	MESSAGE_REQUEST_ROI = 0,
	MESSAGE_REQUIREMNETS,
	MESSAGE_WORK_RESULT,
	MESSAGE_DEM_TRANSFER,
	MESSAGE_TEXTURE_TRANSFER,
	MESSAGE_STATUS,
	NUMBER_OF_MESSAGE_TYPE
};

#define MESSAGE_BUFFSIZE	4096	///< Buffer size
#define INVALID_SOCKET -1

//typedef struct sockaddr_in SOCKADDR_IN;

///< Prototype
int Recvn(int s, void *buf, int len, int flags);
void DisplayError(const char *msg, bool terminate);

class WTCPSock
{
public:
	/* Constructor & Destructor */
	WTCPSock( bool bInitWSA = true );
	virtual ~WTCPSock();

	///< Prevent malfunction
	WTCPSock( const WTCPSock & );
	WTCPSock &operator=( const WTCPSock & );

	/* Member function */
	void CreateSock();
	void	ReuseAddre();
	int  GetSocket();
	struct sockaddr_in *GetServerAddr();

	friend void DisplayError(const char *msg, bool terminate);
	friend int Recvn(int s, void *buf, int len, int flags);

protected:
	/* Member variables */
	bool							  m_bInitWSA;				///< if true, call WSAStartup, WSACleanup
	int							    m_sock;							///< socket
	struct sockaddr_in 	m_addr;
};

/*!
 	 \brief
 	 	 a class for TCP server socket
 */
class WTCPSockServer : public WTCPSock
{
public:
	/* Constructor & Destructor */
	WTCPSockServer();
	virtual ~WTCPSockServer();

	///< Prevent malfunction
	WTCPSockServer( const WTCPSockServer & );
	WTCPSockServer &operator=( const WTCPSockServer & );

	/* Member functions */
	void	Bind();
	void	Listen();
	bool	Accept();
	int 	GetClientSocket();
	struct sockaddr_in *GetClientAddr();

private:
	/* Member variables */
	int						     m_clientSock;
	struct sockaddr_in m_clientAddr;

};

/*!
 	 \brief
 	 	 a class for TCP client socket
 */
class WTCPSockClient : public WTCPSock
{
public:
	/* Constructor & Destructor */
	WTCPSockClient();
	virtual ~WTCPSockClient();

	///< Prevent malfunction
	WTCPSockClient( const WTCPSockClient & );
	WTCPSockClient &operator=( const WTCPSockClient & );

	/* Member functions */
	bool Connect();
};



#endif // WSOCK_H
