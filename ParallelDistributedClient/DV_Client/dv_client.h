#ifndef DV_CLIENT_H
#define DV_CLIENT_H

#include <QtGui/QMainWindow>
#include "ui_dv_client.h"

#include "dvRenderer.h"
#include "dvMinimap.h"
#include "dvClientThread.h"
#include "dvServer.h"

//For Debugging
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console" )

class DV_Client : public QMainWindow
{
	Q_OBJECT

public:
	DV_Client(QWidget *parent = 0, Qt::WFlags flags = 0);
	~DV_Client();

private slots:
    void on_actionConnect_triggered();
    void on_actionSeogwi_triggered();
    void on_actionJeju_triggered();
    void on_actionAbout_triggered();
    void on_actionClose_triggered();
    void on_actionRequest_triggered();
    void on_actionRendering_triggered();
    void on_actionTranslate_toggled(bool arg1);
    void on_actionRotate_toggled(bool arg1);
    void on_actionZoomIn_toggled(bool arg1);
    void on_actionZoomOut_toggled(bool arg1);

private:
	Ui::DV_ClientClass ui;

	enum CITY_CODE{
		NONE   = -1,
		SEOGWI = 1,
		JEJU   = 2
	};
	DvRenderer		*m_RenderDisplay;			// Rendering 
	DvMinimap		*m_MinimapDisplay;			// Mini-map 
	DvClientThread	*m_ClientSocket;			// Client socket
	DvServer		*m_ServerSocket;			// Server socket
	CITY_CODE		m_CityCode;
};

#endif // DV_CLIENT_H
