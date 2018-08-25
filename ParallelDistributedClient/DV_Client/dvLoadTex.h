#ifndef DVLOADTEX_H
#define DVLOADTEX_H

#include "dvCommon.h"
/*!
*	\brief
*		a class for texture data
*/
class DvTex
{
public:
	/* Constructor & Destructor */
	DvTex();
	virtual ~DvTex();

	/* Member functions */
	void	LoadTexFile(const char *filename);
	GLuint	TexMapping();


private:
	/* Member variables */
	IplImage			*m_TexImg;
	int					m_calledCount;
};

#endif // DVLOADTEX_H