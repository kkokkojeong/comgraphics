#include "dv_client.h"
#include <QtGui>
#include <QFileDialog>

/*!
*	\brief
*		Constructor
*/
DV_Client::DV_Client(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags), m_CityCode(SEOGWI)
{
	ui.setupUi(this);

	///< Rendering widget
	m_RenderDisplay = DvRenderer::New();
	m_RenderDisplay->setFocusPolicy(Qt::ClickFocus);
	ui.scrollArea_Render->setWidget(m_RenderDisplay);
	ui.scrollArea_Render->setWidgetResizable(true);

	///< Minimap widget
	m_MinimapDisplay = DvMinimap::New();
	m_MinimapDisplay->setFocusPolicy(Qt::ClickFocus);
	ui.scrollArea_Mini->setWidget(m_MinimapDisplay);
	ui.scrollArea_Mini->setVisible(false);
	ui.scrollArea_Mini->setWidgetResizable(true);

	///< Socket
	m_ClientSocket = DvClientThread::New();					///< client
	m_ClientSocket->start();

	///< server
	m_ServerSocket = new DvServer(this);
	m_ServerSocket->startServer();

	///< Signal / Slots
	connect( m_ServerSocket, SIGNAL(connectserver()), this, SLOT(on_actionConnect_triggered()) );
}

/*!
*	\brief
*		Destructor
*/
DV_Client::~DV_Client()
{
	//< delete
	m_RenderDisplay->Delete();
	m_MinimapDisplay->Delete();
	m_ClientSocket->Delete();

	if( m_ServerSocket != NULL )
		delete m_ServerSocket;
}

/*!
*	\brief
*		Action menu - Connect to Server
*/
void DV_Client::on_actionConnect_triggered()
{
	cout << "Connect server." << endl;
	m_ClientSocket->connectServer();
}

/*!
*	\brief
*		Action Menu - Select the region (Seogwi)
*/
void DV_Client::on_actionSeogwi_triggered()
{
	m_CityCode = SEOGWI;
	m_MinimapDisplay->LoadMinimapTex(DG_SEOGWIPO);
}

/*!
*	\brief
*		Action Menu - Select the region (Jeju)
*/
void DV_Client::on_actionJeju_triggered()
{
	m_CityCode = JEJU;
	m_MinimapDisplay->LoadMinimapTex(DG_JEJU);
}

/*!
*	\brief
*		Action menu - About
*/
void DV_Client::on_actionAbout_triggered()
{
	QMessageBox::about(this, tr("About KISTI_Client Program"),
		tr("Programmer : Seokcheol Jeong in Dongguk Univ Graphics Lab"));
}

/*!
*	\brief
*		Action menu - Exit
*/
void DV_Client::on_actionClose_triggered()
{
	this->close();
}

/*!
*	\brief
*		Action Menu - Request the interested region
*/
void DV_Client::on_actionRequest_triggered()
{
	DEMInfo	roiInfo;

	///< code
	if( m_CityCode == SEOGWI )
		roiInfo.code = CITY_CODE_SEOGWI;
	else if( m_CityCode == JEJU )
		roiInfo.code = CITY_CODE_JEJU;

	///< get roi
	int *rect = m_MinimapDisplay->getBoundingRect();

	roiInfo.roi[0] = rect[0];
	roiInfo.roi[1] = rect[3];
	roiInfo.roi[2] = rect[2];
	roiInfo.roi[3] = rect[1];

	///< lod
	roiInfo.interval      = 10.0;
	roiInfo.magnification = 5;

	///< thread start
	m_ClientSocket->setIsCompleted( false );
	m_ClientSocket->sendInformation( roiInfo );
}

/*!
*	\brief
*		Rendering
*/
void DV_Client::on_actionRendering_triggered()
{
	m_RenderDisplay->SetRenderControlMode( 1 );
}

/*!
*	\brief
*		Action Menu - Translate(Panning)
*/
void DV_Client::on_actionTranslate_toggled(bool arg1)
{
	if(arg1) {
		ui.actionRotate->setChecked( false );
		ui.actionZoomIn->setChecked( false );
		ui.actionZoomOut->setChecked( false );

		m_RenderDisplay->SetNaviControlMode(1);
	}
}

/*!
*	\brief
*		Action Menu - Rotate
*/
void DV_Client::on_actionRotate_toggled(bool arg1)
{
	if(arg1) {
		ui.actionTranslate->setChecked( false );
		ui.actionZoomIn->setChecked( false );
		ui.actionZoomOut->setChecked( false );
	
		m_RenderDisplay->SetNaviControlMode(2);
	}
}

/*!
*	\brief
*		Action Menu - Zoom in
*/
void DV_Client::on_actionZoomIn_toggled(bool arg1)
{
	if(arg1) {
		ui.actionRotate->setChecked( false );
		ui.actionTranslate->setChecked( false );
		ui.actionZoomOut->setChecked( false );

		m_RenderDisplay->SetNaviControlMode(3);
	}
}

/*!
*	\brief
*		Action Menu - Zoom out
*/
void DV_Client::on_actionZoomOut_toggled(bool arg1)
{
	if(arg1) {
		ui.actionRotate->setChecked( false );
		ui.actionZoomIn->setChecked( false );
		ui.actionTranslate->setChecked( false );

		m_RenderDisplay->SetNaviControlMode(4);
	}
}
