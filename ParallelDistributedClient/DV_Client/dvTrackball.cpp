
#include "dVTrackball.h"

/**
	@ brief
		Construction
*/
Trackball::Trackball()
{
	// Initialize the Member variables
	for(int i = 0; i < 2; i++)
	{
		m_PreviousPosition[i] = 0;
		m_CurrentPosition[i] = 0;
	}
	for(int i = 0; i < 4; i++)
	{
		m_CurrentQuatM[i] = 0.0;
		m_LastQuatM[i] = 0.0;
	}

	canvas_width = 512;
	canvas_height = 512;
	trackball(m_CurrentQuatM, 0.0, 0.0, 0.0, 0.0);
}

/**
	@ brief
		Destruction
*/
Trackball::~Trackball()
{

}

/**
	@ brief
		Member function that clears the Trackball
*/
void Trackball::Clear()
{
	for(int i = 0; i < 2; i++)
	{
		m_PreviousPosition[i] = 0;
		m_CurrentPosition[i] = 0;
	}
	for(int i = 0; i < 4; i++)
	{
		m_CurrentQuatM[i] = 0.0;
		m_LastQuatM[i] = 0.0;
	}
	trackball(m_CurrentQuatM, 0.0, 0.0, 0.0, 0.0);
}

/**
	@ brief
		Member function that sets the previous position
*/
void Trackball::SetPreviousPosition(const int _x, const int _y)
{
	m_PreviousPosition[0] = _x;
	m_PreviousPosition[1] = _y;

	//cout << m_PreviousPosition[0] << " " << m_PreviousPosition[1] << endl;
}

/**
	@ brief
		Member function that updates the trackball
*/
void Trackball::SetTrackballUpdate(const int _x, const int _y)
{
	int w = canvas_width;
	int h = canvas_height;

	trackball(m_LastQuatM,
		(2.0 * m_PreviousPosition[0] - w) / w,
		(h - 2.0 * m_PreviousPosition[1]) / h,
		(2.0 * _x - w) / w,
		(h - 2.0 * _y) / h);

	m_PreviousPosition[0] = _x;
	m_PreviousPosition[1] = _y;

	Update();
}

/**
	@ brief
		Member function that sets the canvas size(width, height)
*/
void Trackball::SetTrackballCanvasSize(const int _w, const int _h)
{
	canvas_width	= _w;
	canvas_height	= _h;
}

/**
	@ brief
		Member function to pdate..
*/
void Trackball::Update()
{
	add_quats(m_LastQuatM, m_CurrentQuatM, m_CurrentQuatM);
}

/**
	@ brief
		Member function that gets current matrix(quaternion)
*/
void Trackball::GetMatrix(float m[4][4])
{
	build_rotmatrix(m, m_CurrentQuatM);
}

/**
	@ brief
		Initialize vector with 0.0
*/
void vzero(float *v)
{	
	v[0] = 0.0;
	v[1] = 0.0;
	v[2] = 0.0;
}

/**
	@ brief
		Set vector's value with parameters
*/
void vset(float *v, float x, float y, float z)
{	
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

/**
	@ brief
		Vector Operator -
*/
void vsub(const float *src1, const float *src2, float *dst)
{	
	dst[0] = src1[0] - src2[0];
	dst[1] = src1[1] - src2[1];
	dst[2] = src1[2] - src2[2];
}

/**
	@ brief
		Vector Operator .. Copy
*/
void vcopy(const float *v1, float *v2)
{	
	register int i;
	for (i = 0 ; i < 3 ; i++)
		v2[i] = v1[i];
}

/**
	@ brief
		Vector Operator Crossproduct
*/
void vcross(const float *v1, const float *v2, float *cross)
{	
	float temp[3];

	temp[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
	temp[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
	temp[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
	vcopy(temp, cross);
}

/**
	@ brief
		get the vector length 
*/
float vlength(const float *v)
{	
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

/**
	@ brief
		modify the vector length
*/
void vscale(float *v, float div)
{	
	v[0] *= div;
	v[1] *= div;
	v[2] *= div;
}

/**
	@ brief
		Vector normalize..
*/
void vnormal(float *v)
{	
	vscale(v, 1.0 / vlength(v));
}

/**
	@ brief
		Vector Operator dot product
*/
float vdot(const float *v1, const float *v2)
{	
	return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

/**
	@ brief
		Vector Operator +
*/
void vadd(const float *src1, const float *src2, float *dst)
{	
	dst[0] = src1[0] + src2[0];
	dst[1] = src1[1] + src2[1];
	dst[2] = src1[2] + src2[2];
}

/**
	@ brief
		Vector project to spherical coordinate
*/
float tb_project_to_sphere(float r, float x, float y)
{	
	float d, t, z;

	d = sqrt(x*x + y*y);
	if (d < r * 0.70710678118654752440) {   
		z = sqrt(r*r - d*d);
	} else {         
		t = r / 1.41421356237309504880;
		z = t*t / d;
	}
	return z;
}

/**
	@ brief
		trackball ..
*/
void trackball(float q[4], float p1x, float p1y, float p2x, float p2y)
{	
	float a[3];
	float phi; 
	float p1[3], p2[3], d[3];
	float t;

	if (p1x == p2x && p1y == p2y) {        
		vzero(q);
		q[3] = 1.0;
		return;
	}


	vset(p1,p1x,p1y,tb_project_to_sphere(TRACKBALLSIZE,p1x,p1y));
	vset(p2,p2x,p2y,tb_project_to_sphere(TRACKBALLSIZE,p2x,p2y));

	vcross(p2,p1,a);

	vsub(p1,p2,d);
	t = vlength(d) / (2.0*TRACKBALLSIZE);

	if (t > 1.0) t = 1.0;
	if (t < -1.0) t = -1.0;
	phi = 2.0 * asin(t);

	axis_to_quat(a,phi,q);
}

/**
	@ brief
		axis to quaternion
*/
void axis_to_quat(float a[3], float phi, float q[4])
{
	vnormal(a);
	vcopy(a,q);
	vscale(q,sin(phi/2.0));
	q[3] = cos(phi/2.0);
}
/**
	@ brief
		quaternion normalize
*/
#define RENORMCOUNT 97
void normalize_quat(float q[4])
{	
	int i;
	float mag;

	mag = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
	for (i = 0; i < 4; i++) q[i] /= mag;
}

/**
	@ brief
		quaternion to negative value
*/
void negate_quat(float q[4], float nq[4])
{	
	nq[0] = -q[0];
	nq[1] = -q[1];
	nq[2] = -q[2];
	nq[3] = q[3];
}

/**
	@ brief
		quaternion operator +
*/
void add_quats(float q1[4], float q2[4], float dest[4])
{
	static int count = 0;
	float t1[4], t2[4], t3[4];
	float tf[4];

	vcopy(q1,t1);
	vscale(t1,q2[3]);

	vcopy(q2,t2);
	vscale(t2,q1[3]);

	vcross(q2,q1,t3);
	vadd(t1,t2,tf);
	vadd(t3,tf,tf);
	tf[3] = q1[3] * q2[3] - vdot(q1,q2);

	dest[0] = tf[0];
	dest[1] = tf[1];
	dest[2] = tf[2];
	dest[3] = tf[3];

	if (++count > RENORMCOUNT) {
		count = 0;
		normalize_quat(dest);
	}

}

/**
	@ brief
		build the rotation matrix using quaternion 
*/
void build_rotmatrix(float m[4][4], float q[4])
{	
	m[0][0] = 1.0 - 2.0 * (q[1] * q[1] + q[2] * q[2]);
	m[0][1] = 2.0 * (q[0] * q[1] - q[2] * q[3]);
	m[0][2] = 2.0 * (q[2] * q[0] + q[1] * q[3]);
	m[0][3] = 0.0;

	m[1][0] = 2.0 * (q[0] * q[1] + q[2] * q[3]);
	m[1][1]= 1.0 - 2.0 * (q[2] * q[2] + q[0] * q[0]);
	m[1][2] = 2.0 * (q[1] * q[2] - q[0] * q[3]);
	m[1][3] = 0.0;

	m[2][0] = 2.0 * (q[2] * q[0] - q[1] * q[3]);
	m[2][1] = 2.0 * (q[1] * q[2] + q[0] * q[3]);
	m[2][2] = 1.0 - 2.0 * (q[1] * q[1] + q[0] * q[0]);
	m[2][3] = 0.0;

	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;
	m[3][3] = 1.0;
}