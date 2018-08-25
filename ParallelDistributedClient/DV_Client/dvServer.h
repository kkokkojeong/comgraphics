
#ifndef DVSERVER_H 
#define DVSERVER_H

#include <QTcpServer>

/*!
*	\brief
*		����Ʈ���� TCP/IP ����� ���� ���� Ŭ����
*		����� ����Ʈ������ �ϳ��� ������ ����
*
*	\author ����ö
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