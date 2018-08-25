
#include "Topography.h" 


/*------------------------------------------------------------------------------
		Grid CLASS IMPLEMENTATION
------------------------------------------------------------------------------*/
/*!
	\brief
		Constructor
*/
Grid::Grid()
: m_r(0), m_c(0)
{
	m_horizon	  = 0;
	m_vertical	= 0;
	m_code		  = 0;
}

Grid::Grid(int r, int c)
{
	Create( r, c );
}

/*!
	\brief
		Destructor
*/
Grid::~Grid()
{
	if( m_horizon != 0 )
		delete [] m_horizon;
	if( m_vertical != 0 )
		delete [] m_vertical;
	if( m_code != 0 )
		delete [] m_code;
}

/*!
	\brief
		Memory allocation
*/
void Grid::Create(int r, int c)
{
	m_r = r;
	m_c = c;

	m_horizon	  = new float[m_r + 1];
	m_vertical	= new float[(m_c - 1)*m_r];
	m_code	  	= new int[m_r * m_c];
}

/*!
	\brief
		Get region code
*/
int Grid::GetCode(int r, int c)
{
	if(r < 0 || m_c <= c )
		return -1;

	return m_code[r*m_c + c];
}

/*------------------------------------------------------------------------------
		WorldInfo CLASS IMPLEMENTATION
------------------------------------------------------------------------------*/
/*!
	\brief
		Constructor
*/
WorldInfo::WorldInfo()
: m_xScale(0.0), m_rotY(0.0), m_rotX(0.0), m_yScale(0.0), m_refPtX(0.0), m_refPtY(0.0)
{ 
  
}

WorldInfo::WorldInfo( const char *fileName )
: m_fileName( fileName )
{
	Read( fileName );
}

/*!
  \brief
    Destructor
*/
WorldInfo::~WorldInfo()
{

}

/*!
	\brief
		Read data from the given file
*/
bool WorldInfo::Read( const char *fileName )
{
	m_fileName = fileName;

	ifstream in( fileName );
	if( in == NULL ) 
    return false;
	
  in >> m_xScale;
	in >> m_rotY; 
	in >> m_rotX;
	in >> m_yScale;
	in >> m_refPtX;
	in >> m_refPtY;
 
	in.close();

	return true;
}

/*!
	\brief
		Write data into file
*/
bool WorldInfo::Write( const char *fileName )
{
	ofstream out( fileName );
	if( out == NULL ) 
    return false;
    
	out << m_xScale << endl;
	out << m_rotY << endl; 
	out << m_rotX << endl;
	out << m_yScale << endl;
	out << m_refPtX << endl;
	out << m_refPtY << endl;
 
	out.close();

	return true;
}

/*!
	\brief
		Get geographic coordinates
*/
void WorldInfo::GetWorldPt( double x, double y, double &_x, double &_y )
{
	assert( m_xScale && m_yScale );

	_x = (m_xScale * x) + (m_rotX * y) + m_refPtX;
	_y = (m_yScale * y) + (m_rotY * x) + m_refPtY;
}

void WorldInfo::GetWorldPt( double x, double y, double *pt )
{
	GetWorldPt( x, y, pt[0], pt[1] );
}

/*!
	\brief
		Get image coordinates
		Note : This suppose that there are no rotation
*/
void WorldInfo::GetImagePt( double x, double y, double &_x, double &_y )
{
	assert( m_xScale && m_yScale );
 
	_x = (x - m_refPtX) / m_xScale;
	_y = (y - m_refPtY) / m_yScale;
}

void WorldInfo::GetImagePt( double x, double y, double *pt )
{
	GetImagePt( x, y, pt[0], pt[1] );
}

/*------------------------------------------------------------------------------
		DEMInfo CLASS IMPLEMENTATION
------------------------------------------------------------------------------*/
/*!
	\brief
		Constructor
*/
DEMInfo::DEMInfo()
: m_fileName(), m_width(0), m_height(0)
{
	m_interval[0] = 0;	m_size[0] = 0;
	m_interval[1] = 0;	m_size[1] = 0;
}

/*!
	\brief
		Destructor
*/
DEMInfo::~DEMInfo() 
{

}

/*!
	\brief
		Read header
*/
void DEMInfo::Read( ifstream &in, bool isBinary )
{
	if( isBinary ) {
		in.read( (char *)(m_interval), sizeof(double)*2 );
		in.read( (char *)(m_bound._array), sizeof(double)*4 );
	}
	else {
		///< skip header
		string header;
		int nLine = 0;
		while ( getline( in, header ) )
		{
			++nLine;
			switch( nLine )
			{
			case 9:
				///< read interval
				sscanf( header.c_str(), "%lfmX%lfm", &(m_interval[0]), &(m_interval[1]) );
				break;
			case 12:
				///< read minimum
				sscanf( header.c_str(), "%lf %lf", &(m_bound.m_lo[0]), &(m_bound.m_lo[1]) );
				break;
			case 13:
				///< read maximum
				sscanf( header.c_str(), "%lf %lf", &(m_bound.m_hi[0]), &(m_bound.m_hi[1]) );
				break;
			}

			if( nLine == 29) 
				break;
		}
	}

	m_width			= m_bound.m_hi[0] - m_bound.m_lo[0];
	m_height		= m_bound.m_hi[1] - m_bound.m_lo[1];
	m_size[0]		= (int)(m_width / m_interval[0] + 1);
	m_size[1]		= (int)(m_height / m_interval[1] + 1);
}

/*!
	\brief
		Set DEM information
*/
void DEMInfo::Set(const char *fileName, double intervalX, double intervalY, OrthoRect<double> &r)
{
	m_fileName		= fileName;
	m_bound			  = r;
	m_interval[0]	= intervalX; 
  m_interval[1] = intervalY;
	m_width		  	= m_bound.m_hi[0] - m_bound.m_lo[0];
	m_height		  = m_bound.m_hi[1] - m_bound.m_lo[1];
	m_size[0]	  	= (int)(m_width / m_interval[0]) + 1;
	m_size[1]		  = (int)(m_height / m_interval[1]) + 1;
}

/*------------------------------------------------------------------------------
		DEM CLASS IMPLEMENTATION
------------------------------------------------------------------------------*/
/*!
	\brief
		Constructor
*/
DEM::DEM()
: m_data(0)
{

}

/*!
	\brief
		Destructor
*/
DEM::~DEM()
{
	if( m_data != 0 )
		cvReleaseImage( &m_data );
}

/*!
	\brief
		Set DEM information and allocate memory
*/
bool DEM::Create( double inter_x, double inter_y, OrthoRect<double> &r )
{
	Release();

	m_info.Set(" ", inter_x, inter_y, r);

	m_data = cvCreateImage( cvSize( m_info.m_size[0], m_info.m_size[1] ), IPL_DEPTH_32F, 1 );

	if( m_data == NULL )
		return false;
   
	return true;
}

/*!
	\brief
		Allocate memory for elevation map
		Note : This doesn't set DEM information. Accordingly, you should use only elevation data.
*/
bool DEM::Create( int w, int h )
{
	Release();

	m_info.m_size[0] = w;
	m_info.m_size[1] = h;

	m_data = cvCreateImage( cvSize( m_info.m_size[0], m_info.m_size[1] ), IPL_DEPTH_32F, 1 );

	if( m_data == NULL )
		return false;
   
	return true;
}

/*!
	\brief
		Release memory
*/
void DEM::Release()
{ 
	if( m_data != 0 ) 
		cvReleaseImage( &m_data );

	m_data = 0;
}

/*!
	\brief
		Read data from file
*/
bool DEM::Read( const char *fileName, bool isBinary )
{
	Release();

	m_info.m_fileName = fileName;

	ifstream in;

	if( isBinary )	  in.open( m_info.m_fileName.c_str(), ios_base::binary );
	else			        in.open( m_info.m_fileName.c_str() );
	
	if( in == NULL ) 
    return false;

	m_info.Read( in, isBinary );

	m_data = cvCreateImage( cvSize( m_info.m_size[0], m_info.m_size[1] ), IPL_DEPTH_32F, 1 );
	assert( m_data );

	///< read dataset
	if( isBinary ) 
  {
		in.read( m_data->imageData, sizeof(float) * m_info.m_size[0] * m_info.m_size[1] );
	}
	else 
  {
		float x, y, height;
		for( int i = 0; i < m_info.m_size[0] * m_info.m_size[1]; ++i )
		{
			in >> x >> y >> height;
			cvSetReal2D( m_data, i/m_info.m_size[0], i%m_info.m_size[0], height );
		}
	}

	in.close();

	return true;
}

/*!
	\brief
		Write data into file
*/
bool DEM::Write( const char *fileName )
{
	assert( m_data );

	ofstream out( fileName, ios_base::binary );
	if( out == NULL ) 
    return false;

	out.write( (char *)m_info.m_interval, sizeof(double) * 2 );
	out.write( (char *)m_info.m_bound._array, sizeof(double) * 4 );
	out.write( m_data->imageData, m_data->imageSize );

	out.close();

	return true;
}

/*!
	\brief
		Write data into file in ascii
*/
bool DEM::Save( const char *fileName )
{
	assert( m_data );

	ofstream out( fileName );
	if( out == NULL ) 
    return false;

	out << m_info.m_bound.m_lo[0] << " " << m_info.m_bound.m_lo[1] << endl;
	out << m_info.m_bound.m_hi[0] << " " << m_info.m_bound.m_hi[1] << endl;

	for (int i = 0; i < m_info.m_size[1]; ++i)
		for (int j = 0; j < m_info.m_size[0]; ++j)
			out << GetData(i, j) << " ";

	out.close();

	return true;
}

/*!
	\brief
		Crop elevation map with given extent
*/
void DEM::Crop(int x, int y, int w, int h, DEM &dest)
{
	assert( m_data );
	
	cvSetImageROI( m_data, cvRect(x, y, w, h) );
	cvCopy( m_data, dest.GetCVData() );
	cvResetImageROI( m_data );
}

/*!
	\brief
		Set region of interest
*/
void DEM::SetROI(int x, int y, int w, int h)
{
	assert( m_data );
 
	cvSetImageROI( m_data, cvRect(x, y, w, h) );
}

/*!
	\brief
		Reset region of interest
*/
void DEM::ResetROI()
{
	cvResetImageROI( m_data );
}

/*!
	\brief
		Resize elevation map with given size
*/
void DEM::Resize(int w, int h)
{
	assert( m_data );
	IplImage *temp = cvCreateImage( cvSize(w, h), IPL_DEPTH_32F, 1 );

	cvResize(m_data, temp, CV_INTER_AREA );
	cvReleaseImage( &m_data );
	m_data = temp;

	m_info.m_size[0] = w;
	m_info.m_size[1] = h;
}

/*!
	\brief
		Gets information of DEM
*/
const DEMInfo *DEM::GetInfo() const
{
	return &m_info;
}

/*!
	\brief
		Transform given coord on uniform grid to geographic coord 
*/
void DEM::GetWorldCoord(int x, int y, double *coord) 
{
	GetWorldCoord(x, y, coord[0], coord[1]); 
}

void DEM::GetWorldCoord(int x, int y, double &_x, double &_y)
{
	_x = x * m_info.m_interval[0] + m_info.m_bound.m_lo[0];
	_y = y * m_info.m_interval[1] + m_info.m_bound.m_lo[1];
}


/*!
	\brief
		Gets the cell index corresponding given geographic coord
*/
void DEM::GetCell(double x, double y, int *cell)
{ 
	GetCell(x, y, cell[0], cell[1]); 
}

void DEM::GetCell(double x, double y, int &i, int &j)
{
	assert( m_info.m_bound.InRect(x, y) );

	x -= m_info.m_bound.m_lo[0];
	y -= m_info.m_bound.m_lo[1];
	i = MIN( (int)(x / m_info.m_interval[0] + 0.5), m_info.m_size[0] - 1 );
	j = MIN( (int)(y / m_info.m_interval[1] + 0.5), m_info.m_size[1] - 1 );
}

/*!
	\brief
		Gets the pointer to iplImage
*/
IplImage *DEM::GetCVData()
{
	return m_data;
}

/*!
	\brief
		Gets the pointer to data array
*/
char *DEM::GetData(int row)
{
	assert( m_data );
	return m_data->imageData + row * m_data->widthStep;
}

/*!
	\brief
		Gets the pointer to data array
*/
float DEM::GetData(int y, int x)
{
	assert( m_data );
	return (float)cvGetReal2D( m_data, y, x );
}

/*!
	\brief
		Gets the data size in bytes
*/
int DEM::GetDataSize() const
{
	assert( m_data );
	return m_data->imageSize;
}

/*!
	\brief
		Gets size of aligned map row in bytes
*/
int DEM::GetWidthStep() const
{
	assert( m_data );
	return m_data->widthStep;
}

/*!
	\brief
		Gets width of array of the elevation map
*/
int DEM::GetWidth() const
{
	return m_info.m_size[0];
}

/*!
	\brief
		Gets height of array of the elevation map
*/
int DEM::GetHeight() const
{
	return m_info.m_size[1];
}

/*!
	\brief
		Print information of DEM
*/
void DEM::PrintInfo() const
{
	cout << "----------------------------------" << endl;
	cout << "Name: "<< m_info.m_fileName << endl;
	cout << "Interval: " << m_info.m_interval[0] << " " << m_info.m_interval[1] << endl;
	cout << "Bound: " << m_info.m_bound.m_lo[0] << " " << m_info.m_bound.m_lo[1] << " ";
	cout << m_info.m_bound.m_hi[0] << " " << m_info.m_bound.m_hi[1] << endl;
}

/*------------------------------------------------------------------------------
		Texture CLASS IMPLEMENTATION
------------------------------------------------------------------------------*/
/*!
	\brief
		Constructor
*/
Texture::Texture()
: m_imgToWrdXform()
{
  m_pImg = 0;
}

/*!
	\brief
		Destructor
*/
Texture::~Texture()
{
  Destroy();
}

/*!
	\brief
		Load image
*/
bool Texture::Load(const char* filename, int desired_color)
{
	m_pImg = cvLoadImage( filename, desired_color );
	return (m_pImg != NULL);
}

/*!
	\brief
		Crop image with given extent
*/
void Texture::Crop(int x, int y, int w, int h, Texture &dest)
{
	assert( m_pImg );
	
	cvSetImageROI( m_pImg, cvRect(x, y, w, h) );
	cvCopy( m_pImg, dest.GetImage() );
	cvResetImageROI( m_pImg );
}

/*!
	\brief
		Resize image with given size
*/
void Texture::Resize(int w, int h)
{
	assert( m_pImg );
	IplImage *temp = cvCreateImage( cvSize(w, h), IPL_DEPTH_8U, 3 );

	cvResize(m_pImg, temp, CV_INTER_AREA );
	cvReleaseImage( &m_pImg );
	m_pImg = temp;
}

/*!
	\brief
		Gets world information
*/
WorldInfo &Texture::GetWorldInfo()
{
	return m_imgToWrdXform;
}

/*!
	\brief
		Gets the pointer to data array
*/
char *Texture::GetData(int row)
{
	assert( m_pImg );
	return m_pImg->imageData + row * m_pImg->widthStep;
}

/*!
	\brief
		Gets size of aligned map row in bytes
*/
int Texture::GetWidthStep() const
{
	assert( m_pImg );
	return m_pImg->widthStep;
}

/*!
	\brief
		Gets the number of channel
*/
int Texture::GetNumChannel() const
{
	assert( m_pImg );
	return m_pImg->nChannels;
}

/*!
  \brief
    Save
*/
bool Texture::Save(const char* filename)
{
  if( !m_pImg )
		return false;
	cvSaveImage( filename, m_pImg );
	return true;
}

/*!
  \brief
    Create
*/
bool Texture::Create( int w, int h, int bpp, int origin )
{
  const unsigned max_img_size = 10000;

	if( (bpp != 8 && bpp != 24 && bpp != 32) ||
		(unsigned)w >=  max_img_size || (unsigned)h >= max_img_size ||
		(origin != IPL_ORIGIN_TL && origin != IPL_ORIGIN_BL))
	{
		assert(0); // most probably, it is a programming error
		return false;
	}
	if( !m_pImg || m_pImg->width != w || m_pImg->height != h )
	{
		if( m_pImg && m_pImg->nSize == sizeof(IplImage))
			Destroy();
		/* prepare IPL header */
		m_pImg = cvCreateImage( cvSize( w, h ), IPL_DEPTH_8U, bpp/8 );
	}
	if( m_pImg )
		m_pImg->origin = origin == 0 ? IPL_ORIGIN_TL : IPL_ORIGIN_BL;
	return m_pImg != 0;
}

/*!
  \brief
    Destroy
*/
void Texture::Destroy()
{
  cvReleaseImage( &m_pImg );
}

/*------------------------------------------------------------------------------
		AUXILIARY ROUTINE IMPLEMENTATION
------------------------------------------------------------------------------*/
/*!
	\brief
		Get file path corresponding index and city code with given extension
*/
string GetFilePath( int index, int city_code, FILE_TYPE T, string path )
{
	string r = path;
	char temp[128];

	if( city_code == CITY_CODE_SEOGWI )		r += "/seogwi/";
	else if( city_code == CITY_CODE_JEJU )	r += "/jeju/";

	//Itoa( index, temp, 10 );
  sprintf(temp, "%d", index);
  r += temp;
  
  //cout << r + FileExtension[T] << endl;

	return (r + FileExtension[T] );
}

/*!
	\brief
		Convert city_code to array index
*/
int CITY_CODE_TO_INDEX( int city_code )
{
	switch( city_code )
	{
		case CITY_CODE_SEOGWI:
			return INDEX_SEOGWI;
		case CITY_CODE_JEJU:
			return INDEX_JEJU;
		default:
			return -1;
	}
}

/*!
	\brief
		Print extent of the given region
*/
void PrintRegion(Region *r)
{
	cout << r->code << " " << r->lo_x << " " << r->lo_y  << " " << r->hi_x << " " << r->hi_y << endl; 
}

/* The Itoa code is in the puiblic domain */
char* Itoa(int value, char* str, int radix) 
{
    static char dig[] =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz";
    int n = 0, neg = 0;
    unsigned int v;
    char* p, *q;
    char c;

    if (radix == 10 && value < 0) {
        value = -value;
        neg = 1;
    }
    v = value;
    do {
        str[n++] = dig[v%radix];
        v /= radix;
    } while (v);
    if (neg)
        str[n++] = '-';
    str[n] = '\0';
    for (p = str, q = p + n/2; p != q; ++p, --q)
        c = *p, *p = *q, *q = c;
    return str;
}