#ifndef DVSERVERTHREAD_H 
#define DVSERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "dvServerManager.h"

using namespace std;

/* \brief define PROTOCOL */
const string DV_COMPLETE    = "777";					// 완료
const string DV_CONNECT		= "151";					// 서버 연결
const string DV_REQUEST		= "0";						// 데이터 요청
const string DV_ROTATE		= "50";						// 회전
const string DV_ZOOMIN		= "53";						// 확대 & 축소
const string DV_TRANSLATE	= "54";						// 이동


class DvServerThread;

static vector<QTcpSocket *>		 gClientList;
static QMutex					 gMutex;

/*!
*	\brief
*		a class for server thread
*/
class DvServerThread : public QThread
{
	Q_OBJECT

public:
	/* Constructor & Destructor */
	explicit DvServerThread(int socketDescriptor, QObject *parent = 0);
	virtual ~DvServerThread();

	/* Member functions */
	void run();
	void ExtractStr(string str);											// extract a string
	void CalculateResolution(float *px, float *py, int width, int height);	// calculate resolution
	QTcpSocket *GetSocket();

signals:
	void error(QTcpSocket::SocketError socketError);
	void connectserver();

public slots:
	void readyRead();
	void disconnected();

private:
	/* Member variables */
	QTcpSocket *m_ServerSocket;				// server socket
	int			m_SocketDescriptor;			// socket descriptor
	int			m_CanvasSize[2];
	int			m_RotatedCount;
};

//static vector<DvServerThread*> gServerThread;

#endif // DVSERVERTHREAD_H