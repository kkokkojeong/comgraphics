
#include "dvClientThread.h"
#include "dvRenderer.h"
#include <QtCore>
#include <string>
#include <fstream>
#include <qmessagebox.h>

/*! \brief Global static pointer used to ensure a single instance of the class. */
DvClientThread* DvClientThread::m_pInstance = NULL; 

/*!
*	\brief
*		Constructor
*/
DvClientThread::DvClientThread( QObject *parent /* = 0 */)
	: QThread(parent), m_bRecvData(false), m_SockState(0)
{
	m_ClientSocket = new QTcpSocket();

	///< slots / signals
	connect( m_ClientSocket, SIGNAL(readyRead()), this, SLOT(readyRead()) );
	connect( m_ClientSocket, SIGNAL(disconnected()), this, SLOT(disconnected()) );
}

DvClientThread *DvClientThread::New()
{
	///< Only allow one instance of class to be generated
	if( !m_pInstance )
		m_pInstance = new DvClientThread;

	return m_pInstance;
}

/*!
*	\brief
*		Destructor
*/
DvClientThread::~DvClientThread()
{
	if( m_ClientSocket->isValid() ){
		disconnectServer();
	}

	delete m_ClientSocket;
}

void DvClientThread::Delete()
{
	delete m_pInstance;
}

/*!
*	\brief
*		Get instance pointer using single pattern
*/
DvClientThread *DvClientThread::GetInstance()
{
	return m_pInstance;
}

/*!
*	\brief
*		Connect to server
*/
void DvClientThread::connectServer()
{
	///< set ip address & port
	QString ip   = "210.94.204.55";
	int     port = 9000; 

	///< connect
	m_ClientSocket->connectToHost( ip, port );
	
	if( m_ClientSocket->isValid() ) {
		cout << "IP Address : " << ip.toStdString() << ", Port : " << port << endl;
	}
	else {
		cout << "Fail to connect to server." << endl;
	}
	m_ClientSocket->waitForConnected();
}

/*!
*	\brief	
*		Send the data
*/
void DvClientThread::sendInformation()
{
	QByteArray sendInt;
	QByteArray sendFloat;

	///< First, Send Identity code (message type)
	int identity = 0;
	sendInt.append( reinterpret_cast<const char *>(&identity), sizeof(int) );
	m_ClientSocket->write(sendInt);
	m_ClientSocket->flush();
	sendInt.clear();

	///< Second, Send Interval
	float interval = 10.0;
	sendFloat.append( reinterpret_cast<const char *>(&interval), sizeof(float) );
	m_ClientSocket->write(sendFloat);
	m_ClientSocket->flush();
	sendFloat.clear();

	///< Third, Send Magnification
	int maginification = 10;
	sendInt.append( reinterpret_cast<const char *>(&maginification), sizeof(int) );
	m_ClientSocket->write(sendInt);
	m_ClientSocket->flush();
	sendInt.clear();

	///< Fourth, Send Region code
	int regioncode = 33611;
	sendInt.append( reinterpret_cast<const char *>(&regioncode), sizeof(int) );
	m_ClientSocket->write(sendInt);
	m_ClientSocket->flush();
	sendInt.clear();

	///< Fifth, Send Minimum boundary
	int minx = 156000;
	sendInt.append( reinterpret_cast<const char*>(&minx), sizeof(int) );
	m_ClientSocket->write(sendInt);
	m_ClientSocket->flush();
	sendInt.clear();

	int miny = 35000;
	sendInt.append( reinterpret_cast<const char*>(&miny), sizeof(int) );
	m_ClientSocket->write(sendInt);
	m_ClientSocket->flush();
	sendInt.clear();

	///< Sixth, Send Maximum boundary
	int maxx = 157000;
	sendInt.append( reinterpret_cast<const char*>(&maxx), sizeof(int) );
	m_ClientSocket->write(sendInt);
	m_ClientSocket->flush();
	sendInt.clear();

	int maxy = 36000;
	sendInt.append( reinterpret_cast<const char*>(&maxy), sizeof(int) );
	m_ClientSocket->write(sendInt);
	m_ClientSocket->flush();
	sendInt.clear();

	///< change socket state to communication mode
	m_SockState = 1;
}

void DvClientThread::sendInformation(DEMInfo info)
{
	QByteArray sendInt;
	QByteArray sendFloat;

	cout << info.interval <<", " <<info.magnification<<", " << info.code<<", " << info.roi[0]<< ", " << info.roi[1]<<", " << info.roi[2]<<", " << info.roi[3]<<", " << endl;

	///< First, Send Identity code (message type)
	int identity = 0;
	sendInt.append( reinterpret_cast<const char *>(&identity), sizeof(int) );
	m_ClientSocket->write(sendInt);
	m_ClientSocket->flush();
	sendInt.clear();

	///< Second, Send Interval
	float interval = info.interval;
	sendFloat.append( reinterpret_cast<const char *>(&interval), sizeof(float) );
	m_ClientSocket->write(sendFloat);
	m_ClientSocket->flush();
	sendFloat.clear();

	///< Third, Send Magnification
	int maginification = 5;
	sendInt.append( reinterpret_cast<const char *>(&maginification), sizeof(int) );
	m_ClientSocket->write(sendInt);
	m_ClientSocket->flush();
	sendInt.clear();

	///< Fourth, Send Region code
	int regioncode = info.code;
	sendInt.append( reinterpret_cast<const char *>(&regioncode), sizeof(int) );
	m_ClientSocket->write(sendInt);
	m_ClientSocket->flush();
	sendInt.clear();

	///< Fifth, Send Minimum boundary
	int minx = info.roi[0];
	sendInt.append( reinterpret_cast<const char*>(&minx), sizeof(int) );
	m_ClientSocket->write(sendInt);
	m_ClientSocket->flush();
	sendInt.clear();

	int miny = info.roi[1];
	sendInt.append( reinterpret_cast<const char*>(&miny), sizeof(int) );
	m_ClientSocket->write(sendInt);
	m_ClientSocket->flush();
	sendInt.clear();

	///< Sixth, Send Maximum boundary
	int maxx = info.roi[2];
	sendInt.append( reinterpret_cast<const char*>(&maxx), sizeof(int) );
	m_ClientSocket->write(sendInt);
	m_ClientSocket->flush();
	sendInt.clear();

	int maxy = info.roi[3];
	sendInt.append( reinterpret_cast<const char*>(&maxy), sizeof(int) );
	m_ClientSocket->write(sendInt);
	m_ClientSocket->flush();
	sendInt.clear();

	///< change socket state to communication mode
	m_SockState = 1;
}

/*!
*	\brief
*		run a thread
*/
void DvClientThread::run()
{
	cout << "[Client Thread Started]" << endl;
//	int iCount = 0;
	while( true )
	{
//		iCount++;
//		Sleep(100);
//		cout << iCount << endl;
//		if( iCount == 10 ) {
//			DvRenderer::GetInstance()->SetRenderControlMode( 1 );
//		}

		if( !m_bRecvData )
			continue;

		///< receive data
		if( m_bRecvData && m_SockState == 1)
		{
			///< dem
			RecvDem();

			///< texture
			RecvTexture();

			///< rendering
			DvRenderer::GetInstance()->SetRenderControlMode( 1 );
			m_SockState = 0;
		}
	}
	// make this thread a loop,
	// thread will stay alive so that signal/slot to function properly
	// not dropped out in the middle when thread dies
	exec();
}

/*!
*	\brief
*		ready to read
*/
void DvClientThread::readyRead()
{
	if( m_bRecvData ) {
		return;
	}

	//DvRenderer::GetInstance()->SetRenderControlMode( 1 );

	cout << "[Ready to Ready!]" << endl; 
	m_bRecvData = true;
}

/*!
*	\brief
*		Receive dem
*/
void DvClientThread::RecvDem()
{
	float      interval = 10.0;			///< interval
	int	       roi[4];					///< roi
	int		   fileSize;				///< data size
	QByteArray bin;						///< inputed byte array

	///< time delay
	Sleep(5000);

	///< ROI (region of interested)
	bin    = m_ClientSocket->read( sizeof(int) );
	roi[0] = ByteArrayToInt( bin );

	bin    = m_ClientSocket->read( sizeof(int) );
	roi[1] = ByteArrayToInt( bin );

	bin    = m_ClientSocket->read( sizeof(int) );
	roi[2] = ByteArrayToInt( bin );

	bin    = m_ClientSocket->read( sizeof(int) );
	roi[3] = ByteArrayToInt( bin );

	///< size of dem
	bin	     = m_ClientSocket->read( sizeof(int) );
	fileSize = ByteArrayToInt( bin );

	cout << roi[0] << ", " << roi[1] << ", " << roi[2] << ", " << roi[3] << endl; 
	cout << fileSize << endl;

	///< calculate resolution
	int w = (int)((roi[2] - roi[0]) / interval) + 1;
	int h = (int)((roi[3] - roi[1]) / interval) + 1;

	///< height array
	float *heights = new float[ fileSize / sizeof(float) ];		// memory allocation

	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{
			bin				= m_ClientSocket->read( sizeof(float) );
			heights[i*w+j]  = ByteArrayToFloat(bin);
		}
	}

	///< write file (.dem)
	WriteDemFile( interval, roi, heights, fileSize );

	delete [] heights;											// memory free
}

/*!
*	\brief
*		Receive texture
*/
void DvClientThread::RecvTexture()
{
	int		   fileSize;
	QByteArray bin;	

	///< time delay
	Sleep(5000);

	///< size of texture
	bin      = m_ClientSocket->read( sizeof(int) );
	fileSize = ByteArrayToInt( bin );

	cout << fileSize << endl;

	///< texture data
	unsigned char *texData = new unsigned char[fileSize];

	for(int i = 0; i < fileSize; i++)
	{
		bin        = m_ClientSocket->read( sizeof(char) );
		texData[i] = ByteArrayToChar( bin );
	}
	
	///< write file (.jpg)
	WriteTexFile( texData, fileSize );

	delete [] texData;
}

/*!
*	\brief
*		ByteArray To Integer Type
*/
int DvClientThread::ByteArrayToInt(QByteArray src)
{
	int retval;
	
	memcpy( &retval, src, sizeof(int) );

	return retval;
}

/*!
*	\brief
*		ByteArray To Float Type
*/
float DvClientThread::ByteArrayToFloat(QByteArray src)
{
	float retval;

	memcpy( &retval, src, sizeof(float) );

	return retval;
}

/**
	@ brief
		ByteArray To Character Type
*/
unsigned char DvClientThread::ByteArrayToChar(QByteArray src)
{
	char retval;

	memcpy( &retval, src, sizeof(char) );

	return retval;
}

/*!
*	\brief
*		disconnect
*/
void DvClientThread::disconnectServer()
{
	m_ClientSocket->disconnectFromHost();
	m_ClientSocket->close();
}

void DvClientThread::disconnected()
{
	m_ClientSocket->deleteLater();
	exit(0);
}

/*!
*	\brief
*		Write the dem file (.dem)
*/
bool DvClientThread::WriteDemFile(float interval, int *roi, float *heights, int size)
{
	ofstream out( DV_DEMNAME, ios_base::binary );

	if( !out.is_open() )
		return false;

	out.write( (char *)&interval, sizeof(float) );
	out.write( (char *)&roi[0], sizeof(int) );			out.write( (char *)&roi[1], sizeof(int) );
	out.write( (char *)&roi[2], sizeof(int) );			out.write( (char *)&roi[3], sizeof(int) );

	out.write( (char *)heights, size );
	out.close();				

	return true;
}

/*!
*	\brief
*		Write the texture file (.jpg)
*/
bool DvClientThread::WriteTexFile(unsigned char *tex, int size)
{
	ofstream out( DV_TEXNAME, ios_base::binary );

	if( !out.is_open() )
		return false;

	out.write( (char *)tex, size );

	out.close();

	return true;
}

/*!
*	\brief
*		Set that ready to read data
*	\param
*		true or false
*/
void DvClientThread::setIsCompleted(bool src)
{
	m_bRecvData = src;
}