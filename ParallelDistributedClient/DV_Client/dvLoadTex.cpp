
#include "dvLoadTex.h"

/*!
*	\brief
*		Constructor
*/
DvTex::DvTex()
	: m_TexImg(0)
{
	m_calledCount = 0;
}

/*!
*	\brief
*		Destructor
*/
DvTex::~DvTex()
{
	if( m_TexImg != 0 )
		cvReleaseImage( &m_TexImg );
}

/*!
*	\brief
*		Load the texture file
*/
void DvTex::LoadTexFile(const char *filename)
{
	///< load
	m_TexImg = cvLoadImage( filename, 1 );

//	TexMapping();
}

/*!
*	\brief
*		Texture mapping
*/
GLuint DvTex::TexMapping()
{
	GLuint texData = 0;
//	if( m_calledCount == 0 ) {
		///< texture mapping
		
	//glDeleteTextures(1, &texData);
	glDeleteTextures(1, &texData);
	
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texData);
	glBindTexture(GL_TEXTURE_2D, texData);
	
	// Set the texture magnification & minifying 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Create the best possible precision of the depth map
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_TexImg->width, m_TexImg->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_TexImg->imageData);
	
//		m_calledCount++;

		
		
		
//	}
	return texData;

	
}