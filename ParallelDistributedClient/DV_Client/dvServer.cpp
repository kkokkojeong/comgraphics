#include "dvServer.h"
#include "dvServerThread.h"
#include "dvServerManager.h"


using namespace std;

/*!
*	\brief
*		Constructor
*/
DvServer::DvServer(QObject *parent)
	: QTcpServer(parent)
{
}

/*!
*	\brief
*		Destructor
*/
DvServer::~DvServer()
{
}

/*!
*	\brief
*		start a server
*/
void DvServer::startServer()
{
	///< port number 9500
	int port = 9500;

	///< listen
	if( !this->listen( QHostAddress::Any, port ) ) {
		cout << "could not start server." << endl;
	}
	else {
		cout << "listening to port " << port << "..." << endl;
	}
}

/*!
*	\brief
*		called when a new connection is available
*/
void DvServer::incomingConnection(int socketDescriptor)
{
	///< new connection
	cout << socketDescriptor << "  Connecting..." << endl;

	///< will be run in a newly created thread
	DvServerThread *thread = new DvServerThread(socketDescriptor, this);

	///< connect signal / slot
	///< once a thread is not needed, it will be deleted later
	connect( thread, SIGNAL(finished()), thread, SLOT(deleteLater()) );

//	gServerThread.push_back( thread );
//	cout << "thread size : " << (int)gServerThread.size() << endl;

	thread->start();
}