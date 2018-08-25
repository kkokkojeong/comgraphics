
#ifndef DVMINIMAP_H
#define DVMINIMAP_H

#include "dvCommon.h"
#include <QGLWidget>

class DvMinimap : public QGLWidget
{
	Q_OBJECT

public:
	/* Construction & Destruction */
	DvMinimap(QWidget *parent = 0, QGLWidget *shareWidget = 0);
	virtual ~DvMinimap();

protected:
	/* Member functions */
	void	initializeGL();									// For Rendering
	void	paintGL();
	void	resizeGL(int _width, int _height);
	void	mousePressEvent(QMouseEvent *_event);			// Mouse Event Function
	void	mouseMoveEvent(QMouseEvent *_event);
	void	mouseReleaseEvent(QMouseEvent *_event);

public slots:
	void	idle();

public:
	void	LoadMinimapTex(int _region);
	void	DoTextureMapping();
	void	SetBoundingBoxInfo(int *_boxInfo);
	int		*getBoundingRect();								// Get the current bounding rect

	/*! \brief single pattern */
	static DvMinimap	*New();						// Constructor
	static void			Delete();					// Destructor
	static DvMinimap	*GetInstance();				// Get the instance pointer

private:
	/* Member variables */
	static DvMinimap	*m_pInstance;				// Instance
	QTimer				*m_Timer;					// Timer value
	int					m_MinimapCanvasSize[2];		// Mini Map size(w, h)
	IplImage			*m_LoadTex;					// Mini Map Texture
	GLuint				m_ImageTex;					// Texture to use Mapping
	GLint				m_PrevPosition[2];			// Previous Position
	GLint				m_CurrPosition[2];			// Current Position
	GLint				m_BoundingBand[4];			// Bounding Band of Region
	GLint				m_MinRegion[4];				// Minimum position of region
	GLint				m_MaxRegion[4];				// Maximum position of region
	GLboolean			m_IsPanning;				// Left - Select, Right - Panning
};

#endif	// DGMINIMAP_H