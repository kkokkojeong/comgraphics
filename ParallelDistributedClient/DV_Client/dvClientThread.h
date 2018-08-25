#ifndef DVCLIENTTHREAD_H
#define DVCLIENTTHREAD_H


#include <QThread>
#include <QTcpSocket>

#include <iostream>

#include "dvCommon.h"

using namespace std;

const char DV_DEMNAME[] = "elevation_map.dem";
const char DV_TEXNAME[] = "texture.jpg";

///< define dem information
typedef struct{
	int		code;
	int		roi[4];
	float	interval;
	int		magnification;
	int		width; 
	int		height;
}DEMInfo;

/*!
*	\brief
*		a class for client thread
*/
class DvClientThread : public QThread
{
	Q_OBJECT

public:
	/* Constructor & Destructor */
	explicit DvClientThread(QObject *parent = 0);
	virtual ~DvClientThread();

	/* Member functions */
	void		connectServer();								///< connect server
	void		disconnectServer();								///< disconnect server

	void		setIsCompleted(bool src);						///< set bool receive
	void		sendInformation();								///< send information
	void		sendInformation(DEMInfo info);
	void		RecvDem();										///< receive dem data
	void		RecvTexture();									///< receive texture

	int			  ByteArrayToInt(QByteArray src);
	float		  ByteArrayToFloat(QByteArray src);
	unsigned char ByteArrayToChar(QByteArray src);

	bool		WriteDemFile(float interval, int *roi, float *heights, int size);
	bool		WriteTexFile(unsigned char *tex, int size);

	void		run();											///< run a thread

public slots:
	void		readyRead();									///< ready to read
	void		disconnected();									///< ready to disconnect

signals:
	void		error(QTcpSocket::SocketError socketError);		///< error
	void		StartRender(int) ;								///< start render

public:
	/*! \brief single pattern */
	static DvClientThread	*New();						// Constructor
	static void				Delete();					// Destructor
	static DvClientThread	*GetInstance();				// Get the instance pointer

private:
	/* Member variables */
	static DvClientThread	*m_pInstance;					// Instance

	QTcpSocket		*m_ClientSocket;							///< client socket
	bool			m_bRecvData;
	int				m_SockState;								///< socket state (idle 0, communication 1)
};

#endif // DVCLIENTTHREAD_H