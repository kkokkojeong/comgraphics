#ifndef DVRENDERER_H 
#define DVRENDERER_H

#include "dvCommon.h"
#include "dvLoadDem.h"
#include "dvLoadTex.h"
#include "dvTrackball.h"
#include <QGLWidget>
#include <QTimer>

/*!
*	\class DvRenderer 

*
*	\brief 렌더링을 관리해주는 클래스
*/
class DvRenderer : public QGLWidget
{
	Q_OBJECT

public:
	/* Constructor & Destructor */
	DvRenderer(QWidget *parent = 0, QGLWidget *shareWidget = 0);
	virtual ~DvRenderer();

	/* QGLWidget - Member functions */
	void	initializeGL();							// Rendering
	void	paintGL();
	void	resizeGL(int _width, int _height);
	void	mousePressEvent(QMouseEvent *_event);	// Mouse Event
	void	mouseMoveEvent(QMouseEvent *_event);	// Mouse Event

	/* Member functions */
	void	SetRenderControlMode(int mode);			// Control Mode
	void	SetNaviControlMode(int mode);			// Navigate Mode

	void	SetZoomFactor(float factor);			// Set Zoom factor

	void	InitPanningFactor(int dx, int dy);		// Initialize Panning Factor
	void	SetPanningFactor(int dx, int dy);		// Set Panning Factor

	void	InitRotationFactor(int dx, int dy);		// Initialize Rotation Factor
	void	SetRotationFactor(int dx, int dy);		// Set Rotation Factor

	void	InitZoomFactor(float x1, float y1, float x2, float y2);
	void	CalculateZoomFactor(float x1, float y1, float x2, float y2);

	void	SetDemAndTexture();						// Set DEM & TEX
	void	LoadTexFile(const char *filename);		// Load a texture file(.jpg)
	void	RenderDemAndTexture();					// Render DEM & TEX
	void	RenderTeapot();							// Render Teapot

	void	GetCanvasSize(int *px, int *py);		// Get size of canvas

	/*! \brief single pattern */
	static DvRenderer	*New();						// Constructor
	static void			Delete();					// Destructor
	static DvRenderer	*GetInstance();				// Get the instance pointer


	GLuint				m_TexID;					// Polygon ID

	/* Slots */
public slots:
	void	idle();									// idle function

private:
	/* NAVI_MODE ENUM */
	enum NAVI_MODE {
		NONE = -1,
		TRANSLATE = 1,
		ROTATE = 2,
		ZOOMIN = 3,
		ZOOMOUT = 4
	};

	/* RENDER_MODE ENUM */
	enum RENDER_MODE {
		NOTRENDER = -1,
		RENDER = 1
	};
	int callcount;

	/* Member variables */
	static DvRenderer	*m_pInstance;				// Instance
	QTimer				*m_Timer;					// Timer value
	int					m_RenderCanvasSize[2];		// Display Map size(w, h)
	bool				m_bIsReceived;				// Is received
	DvDEM				m_Dem;						// Dem data
	DvTex				m_Tex;						// Tex data
	GLuint				m_PolygonID;				// Polygon ID

	float				m_CenterPosition[3];		// Center position
	float				m_PreviousPostion[2];		// Previous position
	float				m_CurrentPostion[2];		// Current position
	float				m_MoveScene[3];				// Move factor
	Trackball			m_Trackball;				// Trackball

	QLineF				m_PreviousLine;
	QLineF				m_CurrentLine;

	float					mouse_prev_x;
	float					mouse_prev_y;
	float					mouse_dx;
	float					mouse_dy;
	float				rotate_factor[2];

	/* Render mode 0: idle, 1: terrain */
	int					m_RenderMode;
	NAVI_MODE			m_NaviMode;

	IplImage *texImage;
};

#endif // DVRENDERER_H