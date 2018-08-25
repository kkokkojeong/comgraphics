#ifndef TOPOGRAPHY_H
#define TOPOGRAPHY_H

#include <iostream>
#include <map>
#include <string>
#include <stdlib.h>
#include <sys/param.h>
#include <fstream>
#include <cv.h>
#include <highgui.h>
#include <assert.h>

using namespace std;

#define DEFAULT_TEXTURE_COVERAGE 0.29999999999999999 ///< One pixel cover 0.299...m

///< CITY CODE
#define CITY_CODE_SEOGWI			33611	///< SEOGWI prefix number
#define CITY_CODE_JEJU				33607	///< JEJU prefix number

///< REGION CODE
/*
	SEOGWI
		
		33611005
		33611006
		33611015
		33611016

	JEJU

		33607051
		33607061
		33607062
		33607071
		33607072
		33607081
		33607082
		33607091
		33607092
		33607093
*/

enum CITY_INDEX {							      ///< Index for using array
	INDEX_SEOGWI = 0,
	INDEX_JEJU,
	NUM_CITY
};

enum FILE_TYPE {						       	///< File type
	FILE_DEM = 0,
	FILE_TEXTURE,
	FILE_WORLD
};

static string FileExtension[3] = {					///< File extension
	".dem", ".jpg", ".tfw"
};

/*------------------------------------------------------------------
	CLASS FORWARD DECLARATION
--------------------------------------------------------------------*/
template<class DataType>
class OrthoRect;
class WorldInfo;
class DEMInfo;
class DEM;
class Texture;

/*------------------------------------------------------------------
	STRUCTURE DEFINITION
--------------------------------------------------------------------*/
/*!
	\brief
		Region information
*/
typedef struct
{
	float	lo_x;	///< rower bound
	float	lo_y;
	float	hi_x;	///< upper bound
	float	hi_y;
	int		code;	///< what area is required (city code or region code)
}Region;

/*------------------------------------------------------------------
	AUXILIARY FUNCTION PROTOTYPE
--------------------------------------------------------------------*/
string GetFilePath( int index, int city_code, FILE_TYPE T, string path = "" );
int CITY_CODE_TO_INDEX( int city_code );
void PrintRegion(Region *r);
char* Itoa(int value, char* str, int radix);

/*------------------------------------------------------------------
	CLASS FOR ORTHORECT
--------------------------------------------------------------------*/
/*!
  \brief
    a class for 2-dimensional orthogonal bounding rect
*/
template<class DataType>
class OrthoRect
{
public:
  /* Constructor & Destructor */
  inline OrthoRect( DataType lo_x = 0.0, DataType lo_y = 0.0, DataType hi_x = 0.0, DataType hi_y = 0.0 );
  inline OrthoRect( const OrthoRect<DataType> &r );
  inline OrthoRect( const Region &r );
  virtual ~OrthoRect();
  
  /* Operator */
  inline OrthoRect &operator=( const OrthoRect<DataType> &r );
  
  /* Member functions */
  inline DataType Width() const;
  inline DataType Height() const;
  inline void Set( DataType lo_x, DataType lo_y, DataType hi_x, DataType hi_y );
  inline bool InRect( DataType x, DataType y );
  
public:
  /* Member variables */
  union {
    struct {
      DataType x1, y1, x2, y2;
    };
    struct {
      DataType m_lo[2];
      DataType m_hi[2];
    };
    DataType _array[4];
  };
};

/*!
  \brief
    Constructor
*/

template<class DataType>
inline OrthoRect<DataType>::OrthoRect( DataType lo_x /*= 0.0*/, DataType lo_y /*= 0.0*/, DataType hi_x /*= 0.0*/, DataType hi_y /*= 0.0*/ )
{
  m_lo[0] = lo_x; 
  m_lo[1] = lo_y;
  
  m_hi[0] = hi_x;  
  m_hi[1] = hi_y;
}

/*!
  \brief
    Copy constructor
*/
template<class DataType>
inline OrthoRect<DataType>::OrthoRect( const OrthoRect<DataType> &r )
{
	m_lo[0] = r.m_lo[0]; 
  m_lo[1] = r.m_lo[1]; 
  
	m_hi[0] = r.m_hi[0]; 
  m_hi[1] = r.m_hi[1];
}

/*!
	\brief
		Construct by given region
*/
template<class DataType>
inline OrthoRect<DataType>::OrthoRect( const Region &r )
{
	m_lo[0] = r.lo_x; m_lo[1] = r.lo_y; 
	m_hi[0] = r.hi_x; m_hi[1] = r.hi_y;
}

/*!
	\brief
		Destructor
*/
template<class DataType>
OrthoRect<DataType>::~OrthoRect()
{

}

/*!
	\brief
		Assign operator
*/
template<class DataType>
inline OrthoRect<DataType> &OrthoRect<DataType>::operator=( const OrthoRect<DataType> &r )
{
	m_lo[0] = r.m_lo[0]; m_lo[1] = r.m_lo[1];
	m_hi[0] = r.m_hi[0]; m_hi[1] = r.m_hi[1];
	return *this;
}

/*!
	\brief
		Gets the width
*/
template<class DataType>
inline DataType OrthoRect<DataType>::Width() const
{
	return m_hi[0] - m_lo[0];
}

/*!
	\brief
		Gets the height
*/
template<class DataType>
inline DataType OrthoRect<DataType>::Height() const
{
	return m_hi[1] - m_lo[1];
}

/*!
	\brief
		Set boundary
*/
template<class DataType>
inline void OrthoRect<DataType>::Set( DataType lo_x, DataType lo_y, DataType hi_x, DataType hi_y )
{
	m_lo[0] = lo_x; m_lo[1] = lo_y; m_hi[0] = hi_x; m_hi[1] = hi_y;
}

/*!
	\brief
		Check whether given point is in the rect
*/
template<class DataType>
inline bool OrthoRect<DataType>::InRect( DataType x, DataType y )
{
	return ( m_lo[0] <= x && x <= m_hi[0] && m_lo[1] <= y && y <= m_hi[1] );
}

/*------------------------------------------------------------------
	CLASS FOR GRID
--------------------------------------------------------------------*/
/*!
	\brief
		a class for grid
*/
class Grid
{
public:
  /* Constructor & Destructor */
  Grid();
  Grid( int r, int c );
  virtual ~Grid();
  
  ///> Prevent malfunction
  Grid( const Grid & );
  Grid &operator=( const Grid & );
  
  /* Member functions */
  void Create( int r, int c );
  int GetCode( int r, int c );
  //inline float GetVCut( int r, int c );

public:
  /* Member variables */
  int    m_r;
  int    m_c;
  float  *m_horizon;      ///< horizontal splitting point each column
  float  *m_vertical;     ///< vertical splitting point each row
  float  m_hLimit[2];     ///< horizontal boundary
  int    *m_code;
};


/*------------------------------------------------------------------
	CLASS FOR WORLDINFO
--------------------------------------------------------------------*/
/*!
	\brief
		The class provides real world coordinate information 
		that allows a corresponding .tif file to be correctly 
		positioned on a map or in a mapping system.
*/
class WorldInfo
{
public:
  /* Constructor & Destructor */
  WorldInfo();
  WorldInfo( const char *filename );
  virtual ~WorldInfo();
  
  ///< Prevent malfunction
  WorldInfo( const WorldInfo & );
  WorldInfo &operator=( const WorldInfo & );
  
  /* Member functions */
  bool Read( const char *fileName );
  bool Write( const char *fileName );
  void GetWorldPt( double x, double y, double &_x, double &_y );
  void GetWorldPt( double x, double y, double *pt );
  void GetImagePt( double x, double y, double &_x, double &_y );
  void GetImagePt( double x, double y, double *pt );
  
  /* Functions to get */
  string GetName() const       { return m_fileName; }
  double GetXScale() const     { return m_xScale;   }
  double GetYScale() const     { return m_yScale;   }
  double GetRotX() const       { return m_rotX;     }
  double GetRotY() const       { return m_rotY;     }
  double GetRefX() const       { return m_refPtX;   }
  double GetRefY() const       { return m_refPtY;   }
  
private:
  /* Member variables */
  string	m_fileName;		///< file name
  double	m_xScale;		  ///< horizontal distance in meters represented by each pixel
  double	m_rotY;			  ///< rotation about y axis
	double	m_rotX;			  ///< rotation about x axis
	double	m_yScale;		  ///< vertical distance in meters represented by each pixel
	double	m_refPtX;		  ///< x-reference point
	double	m_refPtY;	  	///< y-reference point
};

/*------------------------------------------------------------------
	CLASS FOR DEMINFO
--------------------------------------------------------------------*/
/*!
	\brief
		Information of Digital elevation model
*/
class DEMInfo
{
  ///< friend class
  friend class DEM;
  
public:
  /* Constructor & Destructor */
  DEMInfo();
  virtual ~DEMInfo();
  
  ///< Prevent malfunction
  DEMInfo( const DEMInfo &info );
  DEMInfo &operator=( const DEMInfo &rhs );
  
  /* Member functions */
  ///< Read header
  void Read( ifstream &in, bool isBinary = true );
  void Set( const char *fileName, double intervalX, double intervalY, OrthoRect<double> &r );
  
public:
  /* Member variables */
  string			    	m_fileName;		    ///< file name
	double			    	m_width;		      ///< width of the area of coverage
	double				    m_height;	      	///< height of the area of coverage
	double				    m_interval[2];  	///< grid interval
	OrthoRect<double>	m_bound;		      ///< bound rect
	int					      m_size[2];	    	///< size of array of elevation values about 2-dimension
};

/*------------------------------------------------------------------
	CLASS FOR DEM
--------------------------------------------------------------------*/
/*!
	\brief
		Digital elevation model
		Note : This class is specified for the JEJU DEM
*/
class DEM
{
public:
  /* Constructor & Destructor */
  DEM();
  virtual ~DEM();
  
  ///< Prevent malfunction
  DEM( const DEM & );
  DEM &operator=( const DEM & );
  
  /* Member functions */
  bool Create( double inter_x, double inter_y, OrthoRect<double> &r );
  bool Create( int w, int h );
  void Release();
  bool Read( const char *fileName, bool isBinary = true );
  bool Write( const char *fileName );
  bool Save( const char *filName );
  void Crop(int x, int y, int w, int h, DEM &dest);
  void SetROI(int x, int y, int w, int h);
  void ResetROI();
  void Resize(int w, int h);
  const DEMInfo *GetInfo() const;
  void GetWorldCoord(int x, int y, double *coord);
  void GetWorldCoord(int x, int y, double &_x, double &_y);
  void GetCell(double x, double y, int *cell);
	void GetCell(double x, double y, int &_x, int &_y);
  IplImage * GetCVData();
  char *GetData(int row = 0);
	float GetData(int y, int x);
	int GetDataSize() const;
	int GetWidthStep() const;
	int GetWidth() const;
	int GetHeight() const;
	void PrintInfo() const;
 
protected:
	DEMInfo		 m_info;			///< information
	IplImage	*m_data;	  	///< elevation map
};

/*------------------------------------------------------------------
	CLASS FOR TEXTURE
--------------------------------------------------------------------*/
/*!
	\brief
		Texture class derived from CImage in OpenCV
*/
class Texture
{
	template<class DataType> friend class Topography;

public:
	Texture();
	~Texture();

	///< Prevent malfunction
	Texture( const Texture &tex );
	Texture &operator=( const Texture &tex );

	bool Load(const char* filename, int desired_color = 1);
	void Crop(int x, int y, int w, int h, Texture &dest);
	void Resize(int w, int h);
	WorldInfo &GetWorldInfo();
	char *GetData(int row = 0);
	int GetWidthStep() const;
	int GetNumChannel() const;
  
  int Width()   { return !m_pImg ? 0 : !m_pImg->roi ? m_pImg->width : m_pImg->roi->width;    }
  int Height()  { return !m_pImg ? 0 : !m_pImg->roi ? m_pImg->height : m_pImg->roi->height;  }
  IplImage* GetImage() { return m_pImg; }
  
  bool  Save( const char* filename );
  bool  Create( int width, int height, int bits_per_pixel, int image_origin = 0 );
  void  Destroy();
  

protected:
	WorldInfo			m_imgToWrdXform;	///< transform image to world
  IplImage*     m_pImg;
};

#endif