#ifndef DVTRACKBALL_H
#define DVTRACKBALL_H

#include "dvCommon.h"

class Trackball
{
public:
	/* Construction & Destruction */
	Trackball();
	virtual ~Trackball();

	/* Member Functions*/
	void	SetTrackballCanvasSize(const int _w, const int _h);
	void	SetPreviousPosition(const int _x, const int _y);
	void	SetTrackballUpdate(const int _x, const int _y);
	void	GetMatrix(float m[4][4]);
	void	Clear();

protected:
	void	Update();

	/* Friend Functions*/
	friend void trackball(float q[4], float p1x, float p1y, float p2x, float p2y);
	friend void negate_quat(float *q, float *qn);
	friend void build_rotmatrix(float m[4][4], float q[4]);
	friend void axis_to_quat(float a[3], float phi, float q[4]);
	friend void add_quats(float *q1, float *q2, float *dest);

private:
	/* Member Variables */
	int		canvas_width;						// Canvas Size
	int		canvas_height;
	int		m_PreviousPosition[2];				// Previous Position
	int		m_CurrentPosition[2];				// Current Position
	float	m_CurrentQuatM[4];					// Using Quaternion, current matrix
	float	m_LastQuatM[4];						//					 last matrix
};
#endif // DVTRACKBALL_H