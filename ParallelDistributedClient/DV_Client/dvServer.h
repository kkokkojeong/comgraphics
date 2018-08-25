
#ifndef DVSERVER_H 
#define DVSERVER_H

#include <QTcpServer>

/*!
*	\brief
*		스마트폰과 TCP/IP 통신을 위한 서버 클래스
*		연결된 스마트폰마다 하나의 스레드 생성
*
*	\author 정석철
*	\date   2015. 9. 3
*/
class DvServer : public QTcpServer
{
	Q_OBJECT

public:
	/* Constructor & Destructor */
	explicit DvServer(QObject *parent = 0);
	virtual ~DvServer();

	/* Member functions */
	void startServer();

signals:

	public slots:
protected:
	void incomingConnection(int socketDescriptor);
};

#endif // DVSERVER_H