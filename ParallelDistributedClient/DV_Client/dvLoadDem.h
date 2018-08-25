#ifndef DVLOADDEM 
#define DVLOADDEM

#include "dvCommon.h"
#include "dvClientThread.h"

/*!
*	\brief
*		a class for dem data
*/
class DvDEM
{
public:
	/* Constructor & Destructor */
	DvDEM();
	virtual ~DvDEM();

	/* Member functions */
	void		LoadDemFile(const char *filename);			///< load
	void		DisplayDem();								///< display
	GLuint		GetDisplayList();

	void		GetCenterPosition(float *px, float *py, float *pz);

private:
	/* Member variables */
	float		*m_HeighsArray;								///< heigt array
	DEMInfo		m_DemInfo;									///< information
	GLuint		m_DisplayList;								///< display list
};


#endif // DVLOADDEM