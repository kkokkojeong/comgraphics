
#include "dvRenderer.h"
#include "dvClientThread.h"
#include <QtGui>
#include <QtOpenGL>

/*! \brief Global static pointer used to ensure a single instance of the class. */
DvRenderer* DvRenderer::m_pInstance = NULL; 

/*!
*	\brief 
*		Constructor
*/
DvRenderer::DvRenderer(QWidget *parent /* = 0 */, QGLWidget *shareWidget /* = 0 */)
	: QGLWidget(parent, shareWidget), m_bIsReceived(false), m_RenderMode(NOTRENDER), m_NaviMode(NONE)
{

	callcount = 0;
	///< initialize
	m_RenderCanvasSize[0] = 1900;
	m_RenderCanvasSize[1] = 1100;

	//m_RenderCanvasSize[0] = 1181;
	//m_RenderCanvasSize[1] = 711;

	m_MoveScene[0] = 0.0;
	m_MoveScene[1] = 0.0;
	m_MoveScene[2] = 2000.0;

	m_CenterPosition[0] = 0.0;
	m_CenterPosition[1] = 0.0;
	m_CenterPosition[2] = 0.0;

	m_PreviousPostion[0] = 0.0;
	m_PreviousPostion[1] = 0.0;

	m_CurrentPostion[0] = 0.0;
	m_CurrentPostion[1] = 0.0;

	rotate_factor[0] = 0;
	rotate_factor[1] = 0;

	///< connect idle function
	m_Timer = new QTimer;
	m_Timer->setSingleShot(false);
	m_Timer->start(1);
	connect( m_Timer, SIGNAL(timeout()), this, SLOT(idle()) );
}

DvRenderer *DvRenderer::New()
{
	///< Only allow one instance of class to be generated
	if( !m_pInstance )
		m_pInstance = new DvRenderer;

	return m_pInstance;
}

/*!
*	\brief 
*		Destructor
*/
DvRenderer::~DvRenderer()
{
	delete m_Timer;
}

void DvRenderer::Delete()
{
	delete m_pInstance;
}

/*!
*	\brief
*		Get instance pointer using single pattern
*/
DvRenderer *DvRenderer::GetInstance()
{
	return m_pInstance;
}


/*!
*	\brief	
*		Initialize OpenGL
*/
void DvRenderer::initializeGL()
{
	//glewInit();

	///< Setting for rendering effects
	glClearColor(0.7f, 0.7f, 0.7f, 0.0f);
	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
	glClearDepth(1.0f);

	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH); 

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);


	// Alpha Blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDisable(GL_COLOR_MATERIAL);

	///< Setup for light source 
	/*
	float lightAmbient[] 	= {0.5f, 0.5f, 0.5f, 1.0f};		// light source ambient
	float lightDiffuse[] 	= {1.0f, 1.0f, 1.0f, 1.0f};		// light source diffuse
	float lightPosition[]   = {0.0f, 10.0f, 50.0f, 1.0f};	// light source position

	glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
//	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	*/

	SetDemAndTexture();
	m_RenderMode = RENDER;

	///< Init trackball
	m_Trackball.SetTrackballCanvasSize( m_RenderCanvasSize[0], m_RenderCanvasSize[1] );
}

/*!
*	\brief 
*		reshape callback function
*/
void DvRenderer::resizeGL(int _width, int _height)
{
	makeCurrent();
	_height = (_height == 0) ? 1 : _height;

	m_Trackball.SetTrackballCanvasSize( _width, _height );
	m_RenderCanvasSize[0] = _width;
	m_RenderCanvasSize[1] = _height;

	glViewport(0, 0, _width, _height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)_width / _height, 0.1f, 1000000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	updateGL();
}

/*!
*	\brief 
*		idle callback function
*/
void DvRenderer::idle()
{	
	if( m_bIsReceived ) {
		cout << "[Render Start]" << endl;
		SetDemAndTexture();

		m_RenderMode  = RENDER;
		m_bIsReceived = false;
	}

	///< re-display
	updateGL();
	makeCurrent();
}

/*!
*	\brief 
*		display callback function
*/
void DvRenderer::paintGL()
{
//	makeCurrent();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0.0f, 0.0f, 3.0f + m_MoveScene[2],	// camera position
		0.0f, 0.0f, 0.0f,							// look at point
		0.0f, 1.0f, 0.0f);							// up vector

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	///< terrain
	if( m_RenderMode == RENDER ) {
		RenderDemAndTexture();	
	}
	glPopAttrib();
}

/*!
*	\brief
*		Set rendering option
*/
void DvRenderer::SetRenderControlMode(int mode)
{
	if( mode == 1 ) {
		m_bIsReceived = true;
	}
	makeCurrent();
	updateGL();
}

/*!
*	\brief
*		Set a dem and texture
*/
void DvRenderer::SetDemAndTexture()
{
	///< Dem
	m_Dem.LoadDemFile( DV_DEMNAME );

	///< calculate center
	m_Dem.GetCenterPosition( &m_CenterPosition[0], &m_CenterPosition[1], &m_CenterPosition[2] );
	m_CenterPosition[0] *= -1.0;
	m_CenterPosition[1] *= -1.0;
	m_CenterPosition[2] *= -1.0;

	///< Texture
	LoadTexFile(DV_TEXNAME);
}

/*!
*	\brief
*		Render a dem and texture
*/
void DvRenderer::RenderDemAndTexture()
{
//	glColor3f(0.1f, 0.1f, 0.1f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, m_TexID );
	glTranslatef( m_MoveScene[0], m_MoveScene[1], 0.0 );				// translation
	glRotatef(rotate_factor[0],0,1,0);
	glRotatef(rotate_factor[1],1,0,0);
	glTranslatef( m_CenterPosition[0], m_CenterPosition[1], m_CenterPosition[2] );

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	///< render
	m_Dem.DisplayDem();					///< polygon
	glDisable(GL_TEXTURE_2D);
}

/*!
*	\brief
*		Teapot
*/
void DvRenderer::RenderTeapot()
{
	glDisable( GL_TEXTURE_2D );
	glPushMatrix();
	{
		glColor3f(0.7f, 0.3f, 0.3f);
		glutSolidTeapot(300.0);
	}
	glPushMatrix();
	glEnable( GL_TEXTURE_2D );
}



/**
	@ brief
		Load a Texture
*/
void DvRenderer::LoadTexFile(const char *filename)
{
	texImage = cvLoadImage(filename, 1);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_TexID);
	glBindTexture(GL_TEXTURE_2D, m_TexID);

	// Set the texture magnification & minifying 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Create the best possible precision of the depth map
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texImage->width, texImage->height,
		0, GL_BGR_EXT, GL_UNSIGNED_BYTE, texImage->imageData);
}

/*!
*	\brief
*		Navigator mode
*/
void DvRenderer::SetNaviControlMode(int mode)
{
	if( mode == 1 )			m_NaviMode = TRANSLATE;
	else if( mode == 2 )	m_NaviMode = ROTATE;
	else if( mode == 3 )	m_NaviMode = ZOOMIN;
	else if( mode == 4 )	m_NaviMode = ZOOMOUT;
//	else if( mode == 5 )	
}

/*!
*	\brief
*		mouse press callback function
*/
void DvRenderer::mousePressEvent(QMouseEvent *_event)
{
	if( _event->button() == Qt::LeftButton )
	{
		if( m_NaviMode == TRANSLATE ) 
		{
			InitPanningFactor( _event->x(), _event->y() );
		}
		else if( m_NaviMode == ROTATE )
		{
			InitRotationFactor( _event->x(), _event->y() );
		}
	}

	if(_event->button() == Qt::MiddleButton) 
	{
		SetRenderControlMode(1);
	}

	updateGL();
}

/*!
*	\brief
*		mouse press callback function
*/
void DvRenderer::mouseMoveEvent(QMouseEvent *_event)
{
	if( _event->buttons() == Qt::LeftButton )
	{
		if( m_NaviMode == TRANSLATE ) 
		{
			SetPanningFactor( _event->x(), _event->y() );
		}
		else if( m_NaviMode == ROTATE )
		{
			SetRotationFactor( _event->x(), _event->y() );
		}
		else if( m_NaviMode == ZOOMIN )
		{
			m_MoveScene[2] -= 15.0;
		}
		else if( m_NaviMode == ZOOMOUT )
		{
			m_MoveScene[2] += 15.0;
		}
	}
}

/*!
*	\brief
*		Set zoom's factor
*/
void DvRenderer::SetZoomFactor(float factor)
{
	if( m_NaviMode == ZOOMIN || m_NaviMode == ZOOMOUT )
		m_MoveScene[2] += factor;

//	updateGL();
}

/*!
*	\brief
*		Initialize factor of panning
*/
void DvRenderer::InitPanningFactor(int dx, int dy)
{
	if( m_NaviMode == TRANSLATE )
	{
		m_PreviousPostion[0] = (float)dx;
		m_PreviousPostion[1] = (float)dy;

		m_CurrentPostion[0]  = (float)dx;
		m_CurrentPostion[1]  = (float)dy;
	}
	
}

/*!
*	\brief
*		Set factor of panning
*/
void DvRenderer::SetPanningFactor(int dx, int dy)
{
	if( m_NaviMode == TRANSLATE )
	{
		m_CurrentPostion[0] = dx;
		m_CurrentPostion[1] = dy;

		float denominator = 1000.0f / (m_MoveScene[2] * 0.3f);

		m_MoveScene[0] += ( (m_CurrentPostion[0] - m_PreviousPostion[0]) / ( denominator));
		m_MoveScene[1] += ( (m_CurrentPostion[1] - m_PreviousPostion[1]) / ( -1.0 * denominator));

		m_PreviousPostion[0] = dx;
		m_PreviousPostion[1] = dy;
	}
}

/*!
*	\brief
*		Initialize factor of rotation
*/
void DvRenderer::InitRotationFactor(int dx, int dy)
{
	if( m_NaviMode == ROTATE ) 
	{
		mouse_prev_x = (float)dx;
		mouse_prev_y = (float)dy;

		mouse_dx = (float)dx;
		mouse_dy = (float)dy;
		//m_Trackball.SetPreviousPosition( dx, dy );
	}
}

/*!
*	\brief
*		Set factor of rotation
*/
void DvRenderer::SetRotationFactor(int dx, int dy)
{
//	cout << dx << ", " << dy << endl;
	if( m_NaviMode == ROTATE ) 
	{
		mouse_dx = dx;
		mouse_dy = dy;

	//	float denominator = 1000.0f / (m_MoveScene[2] * 0.3f);
		rotate_factor[0] += (mouse_dx - mouse_prev_x) * 0.1;
		rotate_factor[1] += (mouse_dy - mouse_prev_y) * 0.1;

	//	cout << "factor : " <<rotate_factor[0] << ", " << rotate_factor[1] << endl;
	//	cout << "previous : " <<mouse_prev_x << ", " << mouse_prev_y << endl;
	//	cout << "current : " <<mouse_dx << ", " << mouse_dy << endl;

		//rotate_factor[0] *= 0.1;
		//rotate_factor[1] *= 0.1;

		mouse_prev_x=dx;
		mouse_prev_y=dy;
		//m_Trackball.SetTrackballUpdate( dx, dy );
		//m_Trackball.SetPreviousPosition( dx, dy );
	}
}

/*!
*	\brief
*		Get a windows size
*/
void DvRenderer::GetCanvasSize(int *px, int *py)
{
	*px = m_RenderCanvasSize[0];
	*py = m_RenderCanvasSize[1];
}

void DvRenderer::CalculateZoomFactor(float x1, float y1, float x2, float y2)
{
//	if( m_NaviMode == ZOOMIN || m_NaviMode == ZOOMOUT )
//	{
		m_CurrentLine = QLineF(x1, y1, x2, y2);

		float currentScalefactor = m_CurrentLine.length() / m_PreviousLine.length();

		if( currentScalefactor >= 1.0 ) {
			//m_NaviMode = ZOOMIN;
			m_MoveScene[2] -= 25.0;

			if( m_MoveScene[2] <= 0.0 )
				m_MoveScene[2] = 0.0;
		}
		else {
			//m_NaviMode = ZOOMOUT;
			m_MoveScene[2] += 25.0;
		}
		m_PreviousLine = m_CurrentLine;
//	}
}

void DvRenderer::InitZoomFactor(float x1, float y1, float x2, float y2)
{
//	if( m_NaviMode == ZOOMIN || m_NaviMode == ZOOMOUT )
//	{
		m_PreviousLine = QLineF(x1, y1, x2, y2);
		m_CurrentLine  = QLineF(x1, y1, x2, y2);
//	}
}