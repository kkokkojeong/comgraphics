
#include "dvMinimap.h"
#include <QtGui>
#include <QtOpenGL>

/*! \brief Global static pointer used to ensure a single instance of the class. */
DvMinimap* DvMinimap::m_pInstance = NULL; 

/**
	@ brief
		Construction
*/
DvMinimap::DvMinimap(QWidget *parent /* = 0 */, QGLWidget *shareWidget /* = 0 */) 
	: QGLWidget(parent, shareWidget), m_LoadTex(0)
{
	m_Timer = new QTimer;
	m_Timer->setSingleShot(false);
	m_Timer->start(1);
	connect(m_Timer, SIGNAL(timeout()), this, SLOT(idle()));

	// Set the size of Display map 
	m_MinimapCanvasSize[0] = 321;
	m_MinimapCanvasSize[1] = 271;

	// Initialize member variables
	m_IsPanning = false;
}

DvMinimap *DvMinimap::New()
{
	///< Only allow one instance of class to be generated
	if( !m_pInstance )
		m_pInstance = new DvMinimap;

	return m_pInstance;
}

/**
	@ brief
		Destruction
*/
DvMinimap::~DvMinimap()
{
	makeCurrent();
	delete m_Timer;				// free memory
	
	if( m_LoadTex != 0 )	
		cvReleaseImage( &m_LoadTex );
}

void DvMinimap::Delete()
{
	delete m_pInstance;
}

/**
	@ brief
		Initialize and Set OpenGL
*/
void DvMinimap::initializeGL()
{
	// Setting for rendering effects
	glClearColor(1.0, 1.0, 1.0, 1.0);	
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

	glEnable(GL_LINE_SMOOTH);

	LoadMinimapTex(DG_SEOGWIPO);						// Load Texture Image

	// Init Mouse Coordinate
//	m_BoundingBand[0] = 0;			m_BoundingBand[1] = 0;
//	m_BoundingBand[2] = 2957;		m_BoundingBand[3] = 3518;
}

/**
	@ brief
		reshape callback function
*/
void DvMinimap::resizeGL(int _width, int _height)
{
	makeCurrent();
	_height = (_height == 0) ? 1 : _height;

	// update to resize
	m_MinimapCanvasSize[0] = _width;
	m_MinimapCanvasSize[1] = _height;

	glViewport(0, 0, m_MinimapCanvasSize[0], m_MinimapCanvasSize[1]);

	updateGL();
}

/**
	@ brief
		display callback function
*/
void DvMinimap::paintGL()
{
	// Projection Matrix..
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(m_MinRegion[0], m_MaxRegion[0], m_MinRegion[1], m_MaxRegion[1], -0.10f, 10.0f);

	// Modelview Matrix..
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Rendering Start 
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);										// Enable Texture mapping
	glBindTexture(GL_TEXTURE_2D, m_ImageTex);

	// Begin Drawing The Background (One Quad)
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f((float)m_MinRegion[0], (float)m_MinRegion[1]);
		glTexCoord2f(1.0f, 1.0f); glVertex2f((float)m_MaxRegion[0], (float)m_MinRegion[1]);
		glTexCoord2f(1.0f, 0.0f); glVertex2f((float)m_MaxRegion[0], (float)m_MaxRegion[1]);
		glTexCoord2f(0.0f, 0.0f); glVertex2f((float)m_MinRegion[0], (float)m_MaxRegion[1]);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	// Draw & Select the region of interest
	glPushMatrix();
	glLineWidth(2.0f);
	glColor3f(1.0, 0.2f, 0.2f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(m_BoundingBand[0], m_BoundingBand[1]);
		glVertex2f(m_BoundingBand[2], m_BoundingBand[1]);
 		glVertex2f(m_BoundingBand[2], m_BoundingBand[3]);
 		glVertex2f(m_BoundingBand[0], m_BoundingBand[3]);
	glEnd();
	glPopMatrix();
}

/**
	@ brief
		idle callback function
*/
void DvMinimap::idle()
{
	updateGL();
	makeCurrent();
}

/**
	@ brief
		load mini map texture using opencv
*/
void DvMinimap::LoadMinimapTex(int _region)
{
	//> Seogwipo
	//> Min 155023, 34161
	//> Max 159676, 39707
	if(_region == DG_SEOGWIPO)
	{
		m_LoadTex = cvLoadImage("../Minimap/MapSeogwipo.jpg", 1);

		m_MinRegion[0] = 155023;
		m_MinRegion[1] = 34161;

		m_MaxRegion[0] = 159676;
		m_MaxRegion[1] = 39707;
	}

	//> Jeju
	//> Min 153538, 39714
	//> Max 158149, 53589
	else if(_region == DG_JEJU)
	{
		m_LoadTex = cvLoadImage("../Minimap/MapJeju.jpg", 1);

		m_MinRegion[0] = 153538;
		m_MinRegion[1] = 39714;

		m_MaxRegion[0] = 158149;
		m_MaxRegion[1] = 53589;
	}

	//> ImjinRiver
	//> Min 144,070,783, 4,496,999
	//> Max 144,165,375, 4,609,543
	//> 2957 * 3518
	else if(_region == DG_IMJINRIVER)
	{
		m_LoadTex = cvLoadImage("../Minimap/ImjinRiver.png", 1);

		m_MinRegion[0] = 144070783;
		m_MinRegion[1] = 4496999;

		m_MaxRegion[0] = 144165375;
		m_MaxRegion[1] = 4609543;
	}

	if(m_LoadTex == NULL){
		cout << "Fail to Load Texture Image.." << endl;
		exit(1);
	}
	
	for(int i = 0; i < 2; i++)				// min region index(0 , 1) , max region index(2, 3)
	{
		m_BoundingBand[i]		= m_MinRegion[i];
		m_BoundingBand[i + 2]	= m_MaxRegion[i];
	}

	DoTextureMapping();
}

/**
	@ brief
		texture mapping
*/
void DvMinimap::DoTextureMapping()
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_ImageTex);
	glBindTexture(GL_TEXTURE_2D, m_ImageTex);

	// Set the texture magnification & minifying 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Create the best possible precision of the depth map
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_LoadTex->width, m_LoadTex->height,
		0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_LoadTex->imageData);
}

/**
	@ brief
		mouse callback function - press event
*/
void DvMinimap::mousePressEvent(QMouseEvent *_event)
{
	int x = _event->x();
	int y = m_MinimapCanvasSize[1] - _event->y();

	if(_event->button() == Qt::LeftButton)		// Left Mouse Event
	{
		m_IsPanning = false;
		float dx, dy;
		dx = (m_MaxRegion[0] - m_MinRegion[0]) / (float)m_MinimapCanvasSize[0];
		dy = (m_MaxRegion[1] - m_MinRegion[1]) / (float)m_MinimapCanvasSize[1];

		m_BoundingBand[0] = (int) (m_MinRegion[0] + x * dx);
		m_BoundingBand[1] = (int) (m_MinRegion[1] + y * dy);

		m_BoundingBand[2] = (int) (m_MinRegion[0] + x * dx);
		m_BoundingBand[3] = (int) (m_MinRegion[1] + y * dy);
		
		m_PrevPosition[0] = x;
 		m_PrevPosition[1] = y;
	}
	if(_event->button() == Qt::RightButton)		// Right Mouse Event
	{
		m_IsPanning = true;

		m_PrevPosition[0] = x;
		m_PrevPosition[1] = y;

		m_CurrPosition[0] = x;
		m_CurrPosition[1] = y;
	}

	
	updateGL();
}

/**
	@ brief
		mouse callback function - move event
*/
void DvMinimap::mouseMoveEvent(QMouseEvent *_event)
{
	int x = _event->x();
	int y = m_MinimapCanvasSize[1] - _event->y();

	// Out of Range check
	if(x < 0)							x = 0;
	else if(x > m_MinimapCanvasSize[0])	x = m_MinimapCanvasSize[0];

	if(y < 0)							y = 0;
	else if(y > m_MinimapCanvasSize[1])	y = m_MinimapCanvasSize[1];

	float dx, dy;
	dx = (m_MaxRegion[0] - m_MinRegion[0]) / (float)m_MinimapCanvasSize[0];
	dy = (m_MaxRegion[1] - m_MinRegion[1]) / (float)m_MinimapCanvasSize[1];

	m_CurrPosition[0] = x;
	m_CurrPosition[1] = y;
	
	if(!m_IsPanning)								// if select mini map..
	{
		m_BoundingBand[2] = (int) (m_MinRegion[0] + x * dx);
		m_BoundingBand[3] = (int) (m_MinRegion[1] + y * dy);		
	}
	else											// if panning region select..
	{
		float mvX, mvY;
		mvX = (m_CurrPosition[0] - m_PrevPosition[0]);
		mvY = (m_CurrPosition[1] - m_PrevPosition[1]);

		m_BoundingBand[0] += mvX * dx;
		m_BoundingBand[1] += mvY * dy;
		m_BoundingBand[2] += mvX * dx;
		m_BoundingBand[3] += mvY * dy;

		m_PrevPosition[0] = m_CurrPosition[0];
		m_PrevPosition[1] = m_CurrPosition[1];
	}
	SetBoundingBoxInfo(m_BoundingBand);			// Update region
	updateGL();
}

/**
	@ brief
		Update the region
*/
void DvMinimap::SetBoundingBoxInfo(int *_boxInfo)
{
	// Check width
	if(_boxInfo[0] < m_MinRegion[0])		_boxInfo[0] = m_MinRegion[0];
	else if(_boxInfo[0] > m_MaxRegion[0])	_boxInfo[0] = m_MaxRegion[0];
	
	if(_boxInfo[2] < m_MinRegion[0])		_boxInfo[2] = m_MinRegion[0];
	else if(_boxInfo[2] > m_MaxRegion[0])	_boxInfo[2] = m_MaxRegion[0];

	// Check height
	if(_boxInfo[1] < m_MinRegion[1])		_boxInfo[1] = m_MinRegion[1];
	else if(_boxInfo[1] > m_MaxRegion[1])	_boxInfo[1] = m_MaxRegion[1];

	if(_boxInfo[3] < m_MinRegion[1])		_boxInfo[3] = m_MinRegion[1];
	else if(_boxInfo[3] > m_MaxRegion[1])	_boxInfo[3] = m_MaxRegion[1];

	// Swap Minimum value
	int *tempArray = new int[4];
	int temp;

	for(int i = 0; i < 4; i++)
	{
		tempArray[i] = _boxInfo[i];
	}

	if(tempArray[0] > tempArray[2])
	{
		temp = tempArray[2];
		tempArray[2] = tempArray[0];
		tempArray[0] = temp;
	}

	if(tempArray[1] > tempArray[3])
	{
		temp = tempArray[3];
		tempArray[3] = tempArray[1];
		tempArray[1] = temp;
	}

	delete tempArray;
}

/**
	@ brief
		mouse callback function - release event
*/
void DvMinimap::mouseReleaseEvent(QMouseEvent *_event)
{
// 	cout << m_BoundingBand[0] << ", " << m_BoundingBand[1] << endl;
// 	cout << m_BoundingBand[2] << ", " << m_BoundingBand[3] << endl;
}

/**
	@ brief
		get the current bounding rectangle(min, max - x, y)
*/

int *DvMinimap::getBoundingRect()
{
	return m_BoundingBand;
}

/*!
*	\brief
*		Get instance pointer using single pattern
*/
DvMinimap *DvMinimap::GetInstance()
{
	return m_pInstance;
}