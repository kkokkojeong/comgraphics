
#include "dvLoadDem.h"
#include "dvRenderer.h"

/*!
*	\brief
*		Constructor
*/
DvDEM::DvDEM()
	: m_HeighsArray(NULL), m_DisplayList(0)
{
}

/*!
*	\brief
*		Destructor
*/
DvDEM::~DvDEM()
{
	if( m_HeighsArray != NULL )
		delete [] m_HeighsArray;
}

/*!
*	\brief
*		Load the dem file
*/
void DvDEM::LoadDemFile(const char *filename)
{
	float	interval;
	int		roi[4];
	
	///< open
	ifstream inter( filename, ios_base::binary );

	///< read
	inter.read( (char *)&interval, sizeof(float) );			// interval
	inter.read( (char *)roi, sizeof(int) * 4 );					// roi

	int w  = (int) ( (roi[2] - roi[0]) / interval + 1);
	int h  = (int) ( (roi[3] - roi[1]) / interval + 1);

	m_HeighsArray = new float[ w * h ];

	inter.read( (char *)m_HeighsArray, sizeof(float) * w * h ); // heights

	/*
	for(int i = 0; i < m_reigonWidth * m_reigonHeight; i++)
	{
		m_heightValue[i] = heights[i];

		if(_file == "ImJinRiver_Bin.dem")
			m_heightValue[i] *= 10.0;

		if(_file == "Jindo_Bin.dem")
			m_heightValue[i] *= 3.0;

		//cout << m_heightValue[i] << endl;
	}
	*/

	///< close
	inter.close();

	///< assign
	m_DemInfo.roi[0]	= roi[0];
	m_DemInfo.roi[1]	= roi[1];
	m_DemInfo.roi[2]	= roi[2];
	m_DemInfo.roi[3]	= roi[3];

	m_DemInfo.interval  = interval;
	m_DemInfo.width		= w;
	m_DemInfo.height	= h;

// 	for(int i = 0; i < 4; i+=2) {
// 		cout << 	m_DemInfo.roi[i] << ", " << 	m_DemInfo.roi[i+1] << endl;
// 	}
// 	cout << m_DemInfo.interval << endl;
// 	cout << m_DemInfo.width << endl;
// 	cout << m_DemInfo.height << endl;
}

/*!
*	\brief
*		Display the dem
*/
void DvDEM::DisplayDem()
{
	float  Tx, Ty;
	double Px, Py;

	int    step = 1;
	float  correct = (float)( m_DemInfo.height - step - 1);

	///< get general lists
	//m_DisplayList = glGenLists(1);

	//> Compile the Object list
	//glNewList(m_genLists, GL_COMPILE);
	glBegin(GL_QUADS);
	for(int i = 0; i < m_DemInfo.height - step - 1; i++) 
	{

		for(int j = 0; j < m_DemInfo.width - step - 1; j++) 
		{
			// Polygon (0, 0), Texture(0, 0)
			Tx = (float) (j) / (float) (m_DemInfo.width - step - 1);
			Ty = (float) (correct -  i) / (float) (m_DemInfo.height - step - 1);

			Px = (m_DemInfo.roi[0] + (j * m_DemInfo.interval));
			Py = (m_DemInfo.roi[1] + (i * m_DemInfo.interval));
			glTexCoord2f(Tx, Ty);
			glVertex3d(Px, Py, m_HeighsArray[m_DemInfo.width * i + j]);

			// Polygon (1, 0), Texture(1, 0)
			Tx = (float) (j+1) / (float) (m_DemInfo.width - step - 1);
			Ty = (float) (correct - i) / (float) (m_DemInfo.height - step - 1);

			Px = (m_DemInfo.roi[0] + ((j+1) * m_DemInfo.interval));
			Py = (m_DemInfo.roi[1] + (i * m_DemInfo.interval));
			glTexCoord2f(Tx, Ty);
			glVertex3d(Px, Py, m_HeighsArray[m_DemInfo.width * i + (j+1)]);

			// Polygon (1, 1), Texture(1, 1)
			Tx = (float) (j+1) / (float) (m_DemInfo.width - step - 1);
			Ty = (float) (correct - (i+1)) / (float) (m_DemInfo.height - step - 1);

			Px = (m_DemInfo.roi[0] + ((j+1) * m_DemInfo.interval));
			Py = (m_DemInfo.roi[1] + ((i+1) * m_DemInfo.interval));
			glTexCoord2f(Tx, Ty);
			glVertex3d(Px, Py, m_HeighsArray[m_DemInfo.width * (i+1) + (j+1)]);

			// Polygon (0, 1), Texture(0, 1)
			Tx = (float) (j) / (float) (m_DemInfo.width - step - 1);
			Ty = (float) ( correct - (i+1)) / (float) (m_DemInfo.height - step - 1);

			Px = (m_DemInfo.roi[0] + (j * m_DemInfo.interval));
			Py = (m_DemInfo.roi[1] + ((i+1) * m_DemInfo.interval));
			glTexCoord2f(Tx, Ty);
			glVertex3d(Px, Py, m_HeighsArray[m_DemInfo.width * (i+1) + j]);	
		}

	}
	glEnd();

//	glEndList();
	

	//glNewList(m_DisplayList, GL_COMPILE);
	/*
	for(int i = 0; i < m_DemInfo.height - step - 1; i++) 
	{
		for(int j = 0; j < m_DemInfo.width - step - 1; j++) 
		{
			glBegin(GL_POLYGON);
			{
				// Polygon (0, 0), Texture(0, 0)
				Tx = (float) (j) / (float) (m_DemInfo.width - step - 1);
				Ty = (float) (correct -  i) / (float) (m_DemInfo.height - step - 1);

				Px = (m_DemInfo.roi[0] + (j * m_DemInfo.interval));
				Py = (m_DemInfo.roi[1] + (i * m_DemInfo.interval));
				glTexCoord2f(Tx, Ty);
				glVertex3d(Px, Py, m_HeighsArray[m_DemInfo.width * i + j]);

				// Polygon (1, 0), Texture(1, 0)
				Tx = (float) (j+1) / (float) (m_DemInfo.width - step - 1);
				Ty = (float) (correct - i) / (float) (m_DemInfo.height - step - 1);

				Px = (m_DemInfo.roi[0] + ((j+1) * m_DemInfo.interval));
				Py = (m_DemInfo.roi[1] + (i * m_DemInfo.interval));
				glTexCoord2f(Tx, Ty);
				glVertex3d(Px, Py, m_HeighsArray[m_DemInfo.width * i + (j+1)]);

				// Polygon (1, 1), Texture(1, 1)
				Tx = (float) (j+1) / (float) (m_DemInfo.width - step - 1);
				Ty = (float) (correct - (i+1)) / (float) (m_DemInfo.height - step - 1);

				Px = (m_DemInfo.roi[0] + ((j+1) * m_DemInfo.interval));
				Py = (m_DemInfo.roi[1] + ((i+1) * m_DemInfo.interval));
				glTexCoord2f(Tx, Ty);
				glVertex3d(Px, Py, m_HeighsArray[m_DemInfo.width * (i+1) + (j+1)]);
			}
			glEnd();
			glBegin(GL_POLYGON);
			{
				// Polygon (0, 1), Texture(0, 1)
				Tx = (float) (j) / (float) (m_DemInfo.width - step - 1);
				Ty = (float) ( correct - (i+1)) / (float) (m_DemInfo.height - step - 1);

				Px = (m_DemInfo.roi[0] + (j * m_DemInfo.interval));
				Py = (m_DemInfo.roi[1] + ((i+1) * m_DemInfo.interval));
				glTexCoord2f(Tx, Ty);
				glVertex3d(Px, Py, m_HeighsArray[m_DemInfo.width * (i+1) + j]);

				// Polygon (0, 0), Texture(0, 0)
				Tx = (float) (j) / (float) (m_DemInfo.width - step - 1);
				Ty = (float) (correct -  i) / (float) (m_DemInfo.height - step - 1);

				Px = (m_DemInfo.roi[0] + (j * m_DemInfo.interval));
				Py = (m_DemInfo.roi[1] + (i * m_DemInfo.interval));
				glTexCoord2f(Tx, Ty);
				glVertex3d(Px, Py, m_HeighsArray[m_DemInfo.width * i + j]);

				// Polygon (1, 1), Texture(1, 1)
				Tx = (float) (j+1) / (float) (m_DemInfo.width - step - 1);
				Ty = (float) (correct - (i+1)) / (float) (m_DemInfo.height - step - 1);

				Px = (m_DemInfo.roi[0] + ((j+1) * m_DemInfo.interval));
				Py = (m_DemInfo.roi[1] + ((i+1) * m_DemInfo.interval));
				glTexCoord2f(Tx, Ty);
				glVertex3d(Px, Py, m_HeighsArray[m_DemInfo.width * (i+1) + (j+1)]);

			}
			glEnd();			
		}
	}
	*/
	
//	glEndList();

//	return m_DisplayList;
}

/*!
*	\brief
*		Get a display list
*/
GLuint DvDEM::GetDisplayList()
{
	return m_DisplayList;
}


/*!
*	\brief
*		Calculate center position(x, y, z)
*/
void DvDEM::GetCenterPosition(float *px, float *py, float *pz)
{
	///< calculate height min, max
	float heightMin = m_HeighsArray[0];
	float heightMax = m_HeighsArray[0];

	for(int i = 0; i < m_DemInfo.width * m_DemInfo.height; i++) 
	{
		heightMin = DG_MIN(heightMin, m_HeighsArray[i]);
		heightMax = DG_MAX(heightMax, m_HeighsArray[i]);
	}

	///< pointer retuner
	*px = 0.5 * (m_DemInfo.roi[0] + m_DemInfo.roi[2]);
	*py = 0.5 * (m_DemInfo.roi[1] + m_DemInfo.roi[3]);
	*pz = 0.5 * (heightMin + heightMax);
}