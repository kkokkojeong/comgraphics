
#include "dvServerThread.h"
#include "dvClientThread.h"
#include "dvRenderer.h"

/*!
*	\brief
*		Constructor
*/
DvServerThread::DvServerThread(int socketDescriptor, QObject *parent)
	: QThread(parent), m_SocketDescriptor(socketDescriptor)
{
	/* Galaxy 4 - 1080 * 1920
	*  Gal Tab  - 1600 * 2560 
	*/
	m_CanvasSize[0] = 1080;
	m_CanvasSize[1] = 1920;

	m_RotatedCount = 0;
}

/*!
*	\brief
*		Destructor
*/
DvServerThread::~DvServerThread()
{
}

/*!
*	\brief
*		run a thread
*/
void DvServerThread::run()
{
	std::cout << "[ServerThread started]" << std::endl;

	///< set the id
	m_ServerSocket = new QTcpSocket();

	if( !m_ServerSocket->setSocketDescriptor(m_SocketDescriptor) ) {
		emit error( m_ServerSocket->error() );
		return;
	}

	gMutex.lock();
	gClientList.push_back( m_ServerSocket );
	gMutex.unlock();

	///< connect socket and signal
	// note - Qt::DirectConnection is used because it's multithreaded
	//        This makes the slot to be invoked immediately, when the signal is emitted.
	connect( m_ServerSocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection );
	connect( m_ServerSocket, SIGNAL(disconnected()), this, SLOT(disconnected()) );

	///< multiple clinets, we want to know which is which
	std::cout << m_SocketDescriptor << " client connected." << std::endl;

	// make this thread a loop,
	// thread will stay alive so that signal/slot to function properly
	// not dropped out in the middle when thread dies
	exec();
}

/*!
*	\brief
*		ready for read a string
*/
void DvServerThread::readyRead()
{
	///< get the string
	QByteArray str = m_ServerSocket->readAll();

	QString tmp( str );
	string parse = tmp.toStdString();

//	cout << "Received message : " << parse << endl;

	///< Extract the string
	ExtractStr( parse );	

	///< Send complete
	//m_ServerSocket->write( DV_COMPLETE.c_str() );
	//m_ServerSocket->flush();

//	cout << "thread size : " << (int)gClientList.size() << endl;

	gMutex.lock();
	for(int i = 0; i < (int)gClientList.size(); i++) {
		//gClientList[i]->write( DV_COMPLETE.c_str() );
		//gClientList[i]->flush();
		//cout << "Send message : " << DV_COMPLETE << endl;

		gClientList[i]->write( parse.c_str() );
		gClientList[i]->flush();
		cout << "Send message : " << parse << endl;
	}
	gMutex.unlock();

//	cout << m_SocketDescriptor << "/Send message : " << parse.c_str() << endl;
//	m_ServerSocket->write( parse.c_str() );
//	m_ServerSocket->flush();
}

/*!
*	\brief
*		disconnect
*/
void DvServerThread::disconnected()
{
	std::cout << m_SocketDescriptor << " Disconnected." << std::endl;
	/*
	for(int i = 0; i < (int)gClientList.size(); i++) {
		if( gClientList[i] == m_ServerSocket )

	}
	*/
	m_ServerSocket->deleteLater();
	exit(0);
}

QTcpSocket *DvServerThread::GetSocket()
{
	return m_ServerSocket;
}

/*!
*	\brief
*		Extract the string
*/
void DvServerThread::ExtractStr(string str)
{
	static int calledCount = 0;

	///< Token
	while( str.find(",") != string::npos ) 
	{
		int parseIdx = str.find(",");
		str.erase( parseIdx, 1 );
		str.insert( parseIdx, " " );
		//count++;
	}
	
	///< Extract
	istringstream  checkParse(str);
	string		   protocol;

	checkParse >> protocol;
//	cout << protocol << endl;

	/* Compare protocol */
	///< Connect
	if( protocol == DV_CONNECT ) {
		cout << "[Connect Signal]" << endl;

		if( calledCount == 0) {
			DvClientThread::GetInstance()->connectServer();
			calledCount++;
		}
	}

	///< Request
	else if( protocol == DV_REQUEST ) {
		cout << "[Request Signal]" << endl;

		DEMInfo info;
		int		type;
		
		// type(0) - interval - magnification - city code - roi
		checkParse >> type >> info.interval >> info.magnification >> info.code 
			>> info.roi[0]>> info.roi[1] >> info.roi[2] >> info.roi[3];

		//if( calledCount == 1 )
		//if(calledCount == 0) {
			DvClientThread::GetInstance()->setIsCompleted( false );
			DvClientThread::GetInstance()->sendInformation(info);
		//	calledCount++;
		//}
	}

	///< Translation
	else if( protocol == DV_TRANSLATE ) {
	
		int   isInit;			///< press or move
		float dx, dy;			///< screen coordinate
		int	  width,height;		///< canvas size
		checkParse >> isInit >> dx >> dy;

		///< Get canvas size
		int canvasSize[2];
		DvRenderer::GetInstance()->GetCanvasSize( &canvasSize[0], &canvasSize[1] );

		///< Calculate resolution
		//m_CanvasSize[0] = width;
		//m_CanvasSize[1] = height;
		CalculateResolution( &dx, &dy, canvasSize[0], canvasSize[1] );

		DvRenderer::GetInstance()->SetNaviControlMode(1);

		if( isInit == 1 ) {
			DvRenderer::GetInstance()->InitPanningFactor((int)dx, (int)dy);
		}
		else {					
			DvRenderer::GetInstance()->SetPanningFactor( (int)dx, (int)dy);

		}
	}

	///< Rotation
	else if( protocol == DV_ROTATE ) {

		int   isInit;			///< press or move
		float dx, dy;			///< screen coordinate
		int	  width,height;		///< canvas size

		checkParse >> isInit >> dx >> dy >> width >> height;

	//	cout << "Rotation: " << isInit << endl;

		///< Get canvas size
		int canvasSize[2];
		DvRenderer::GetInstance()->GetCanvasSize( &canvasSize[0], &canvasSize[1] );

		///< Calculate resolution
		m_CanvasSize[0] = width;
		m_CanvasSize[1] = height;
		CalculateResolution( &dx, &dy, canvasSize[0], canvasSize[1] );

		DvRenderer::GetInstance()->SetNaviControlMode(2);

		if( isInit == 1 ) {
			DvRenderer::GetInstance()->InitRotationFactor((int)dx, (int)dy);
		//	m_RotatedCount++;
		}
		else {			
			m_RotatedCount++;
			DvRenderer::GetInstance()->SetRotationFactor( (int)dx, (int)dy);

			if( m_RotatedCount >= 10 )
				m_RotatedCount = 0;
		}
	}
	
	///< ZoomIn & Out
	else if( protocol == DV_ZOOMIN ) {
		
		//float factor;
		int   isInit;
		float preX, preY, currentX, currentY;
		checkParse >> isInit >> preX >> preY >> currentX >> currentY;


		if( isInit == 1 ) DvRenderer::GetInstance()->InitZoomFactor( preX, preY, currentX, currentY );
		else			  DvRenderer::GetInstance()->CalculateZoomFactor( preX, preY, currentX, currentY );

	//	if( factor >= ? )
	//		DvRenderer::GetInstance()->SetNaviControlMode(3);
	//	else
	//		DvRenderer::GetInstance()->SetNaviControlMode(3);

	//	DvRenderer::GetInstance()->SetZoomFactor(-15.0);
	}
	
	/*
	///< ZoomOut
	else if( protocol == DV_ZOOMOUT ) {
		DvRenderer::GetInstance()->SetNaviControlMode(4);
		DvRenderer::GetInstance()->SetZoomFactor(15.0);
	}
	*/
}

/*!
*	\brief
*		Calculate resolution
*	\param
*		px, py - return position
*/
void DvServerThread::CalculateResolution(float *px, float *py, int width, int height)
{
	float factorX = *px / (float)m_CanvasSize[0];
	float factorY = *py / (float)m_CanvasSize[1];

	*px = (float) width * factorX; 
	*py = (float)height * factorY;

//	cout << *px << ", " << *py << endl;
}