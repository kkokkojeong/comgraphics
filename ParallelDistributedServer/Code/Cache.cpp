
#include "Cache.h"

/*------------------------------------------------------------------------------
		DiskCache CLASS IMPLEMENTATION
------------------------------------------------------------------------------*/
/*!
	\brief
		Constructor
*/
DiskCache::DiskCache()
{
	m_cache.set_empty_key(-1);	///< Initialize hash map
  m_binPath = "/fusionio1/seokcheol/ParallelComputing/bin";
	//GetBinPath( m_binPath );	///< Get bin path

	if ( !Read() ) {
		cerr << "[App Control] Error occurred during read disk cache file." << endl;
		exit(-1);
	}
}

/*!
	\brief
		Destructor
*/
DiskCache::~DiskCache()
{
}

/*!
	\brief
		Read cache table from disk
*/
bool DiskCache::Read()
{
	string fileName( m_binPath );
	fileName += "/";
	fileName += DISK_CACHE_FILE_NAME;

  //cout << fileName << endl;

	ifstream in( fileName.c_str() );

	if( in == NULL )
		return false;

	int	region_code;
	bool exist;

	for (int i = 0; i < NUM_SEOGWI_DATA + NUM_JEJU_DATA; ++i)
	{
		in >> region_code;
		in >> exist;

		m_cache.insert( make_pair( region_code, exist ) );
	}

	in.close();

	return true;
}

/*!
	\brief
		Update cache file
*/
void DiskCache::Update()
{
	string fileName( m_binPath );
	fileName += "/";
	fileName += DISK_CACHE_FILE_NAME;

	ofstream out( fileName.c_str() );

	if ( out == NULL )
	{
		cerr << "Unable to open file : " << fileName.c_str() << endl;
		return;
	}

	dense_hash_map<int, bool>::iterator it = m_cache.begin();

	for ( ; it != m_cache.end(); ++it)
		out << it->first << " " << it->second << endl;

	out.close();
}

/*!
	\brief
		Set value according to given key
*/
void DiskCache::Set(int region_code, bool exist)
{
	dense_hash_map<int, bool>::iterator it = m_cache.find( region_code );

	if( it != m_cache.end() )
		it->second = exist;
}

/*!
	\brief
		Check weather data exist in the disk
*/
bool DiskCache::Check(int region_code)
{
	dense_hash_map<int, bool>::iterator it = m_cache.find( region_code );
	return it->second;
}

/*------------------------------------------------------------------------------
		SlaveMemCache CLASS IMPLEMENTATION
------------------------------------------------------------------------------*/
/*!
  \brief
    Constructor
*/
SlaveMemCache::SlaveMemCache()
{
  m_memory = 0;
	m_cache.set_empty_key(-1);	///< Initialize hash map
	m_cache.set_deleted_key(-2);
}

/*!
	\brief
		Destructor
*/
SlaveMemCache::~SlaveMemCache()
{
	dense_hash_map<int, SlaveCacheData *>::iterator it = m_cache.begin();

	for ( ; it != m_cache.end(); ++it)
		delete it->second;
}

/*!
	\brief
		Gets the amount of the memory
*/
int SlaveMemCache::GetMemSize() const
{
	return m_memory;
}

/*!
	\brief
		Check weather data exist in the memory
*/
bool SlaveMemCache::Check(int region_code)
{
	dense_hash_map<int, SlaveCacheData *>::iterator it = m_cache.find( region_code );
	return (it != m_cache.end());
}

/*!
	\brief
		Gets the data according to given code
*/
SlaveCacheData *SlaveMemCache::GetData(int region_code)
{
	dense_hash_map<int, SlaveCacheData *>::iterator it = m_cache.find( region_code );
	if( it != m_cache.end() )
	{
		it->second->hit += 1;
		return it->second;
	}
	else
		return NULL;
}

/*!
	\brief
		Add data
*/
void SlaveMemCache::AddData(int region_code, SlaveCacheData *data)
{
	dense_hash_map<int, SlaveCacheData *>::iterator it = m_cache.find( region_code );
	if( it == m_cache.end() )
	{
		m_cache.insert( make_pair(region_code, data) );
		m_memory += data->tex.GetImage()->imageSize;
		m_memory += data->dem.GetDataSize();
	}

	if ( SLAVE_CACHE_MEM_LIMIT < m_memory )
		DiscardData( region_code );
}

/*!
	\brief
		Delete data according to given code
*/
void SlaveMemCache::DeleteData(int region_code)
{
	dense_hash_map<int, SlaveCacheData *>::iterator it = m_cache.find( region_code );
	if( it != m_cache.end() )
	{
		m_cache.erase( it );
	}
}

/*!
	\brief
		Delete data that has seldom used
*/
void SlaveMemCache::DiscardData(int exception)
{
	dense_hash_map<int, SlaveCacheData *>::iterator it = m_cache.begin();
	dense_hash_map<int, SlaveCacheData *>::iterator minimum = it;

	for ( ; it != m_cache.end(); ++it )
	{
		if( it->second->hit < minimum->second->hit && it->first != exception)
			minimum = it;
	}

	if ( minimum->first == exception )
		++minimum;

	m_memory -= minimum->second->tex.GetImage()->imageSize;
	m_memory -= minimum->second->dem.GetDataSize();

	delete (minimum->second);

	m_cache.erase( minimum );
}

/*------------------------------------------------------------------------------
		MasterCacheData CLASS IMPLEMENTATION
------------------------------------------------------------------------------*/
/*!
  \brief
    Constructor
*/
MasterCacheData::MasterCacheData(int dem_size, int tex_size)
{
  m_heights = new float[dem_size];
  m_pixels  = new char[tex_size];
}

/*!
  \brief
    Destructor
*/
MasterCacheData::~MasterCacheData()
{
  delete [] m_heights;
  delete [] m_pixels;
}

/*------------------------------------------------------------------------------
		MasterMemCache CLASS IMPLEMENTATION
------------------------------------------------------------------------------*/
/*!
	\brief
		Constructor
*/
MasterMemCache::MasterMemCache()
{
	m_boundary[INDEX_SEOGWI].Set(155023.0, 34161.0, 159676.0, 39707.0);
	m_boundary[INDEX_JEJU].Set(153538.0, 39714.0, 158149.0, 53589.0);

	m_cityCode		  = 0;
	m_interval		  = 0;
	m_magnification = 0;
	m_texBlockSize	= 0;

	m_w = 0;
	m_h = 0;
	m_cacheMap.set_empty_key(-1);
	m_cacheMap.set_deleted_key(-2);
}

/*!
	\brief
		Destructor
*/
MasterMemCache::~MasterMemCache()
{
	Release();
}

/*!
	\brief
		Initialize cache
*/
void MasterMemCache::Init(int cityCode, float interval, int magnification)
{
	m_memory		= 0;
	m_cityCode		= cityCode;
	m_interval		= interval;
	m_magnification = magnification;

	m_texBlockSize	= (int)((BLOCK_DATA_SIZE * m_interval)/(DEFAULT_TEXTURE_COVERAGE * (double)m_magnification) + 0.5);

	float length	= BLOCK_DATA_SIZE*interval;

	m_w				= (int)ceil( m_boundary[CITY_CODE_TO_INDEX(m_cityCode)].Width() / length );
	m_h				= (int)ceil( m_boundary[CITY_CODE_TO_INDEX(m_cityCode)].Height() / length );
}

/*!
	\brief
		Release data
*/
void MasterMemCache::Release()
{
	dense_hash_map<unsigned int, MasterCacheData *>::iterator it = m_cacheMap.begin();

	for (; it != m_cacheMap.end(); ++it)
		delete it->second;

	m_cacheMap.clear();
	m_que.clear();

	m_cityCode		= 0;
	m_interval		= 0;
	m_magnification = 0;

	m_w				= 0;
	m_h				= 0;
}

/*!
	\brief
		Is empty?
*/
bool MasterMemCache::Empty()
{
	return (m_memory == 0);
}

/*!
	\brief
		Insert data
*/
void MasterMemCache::Insert(int index, MasterCacheData *data)
{
	m_que.push_back( index );

	m_cacheMap.insert( make_pair(index, data) );

	m_memory += BLOCK_DATA_SIZE*BLOCK_DATA_SIZE*sizeof(float);
	m_memory += m_texBlockSize * m_texBlockSize*3;

	if ( MASTER_CACHE_MEM_LIMIT < m_memory )
		Discard();
}

/*!
	\brief
		Discard data
*/
void MasterMemCache::Discard()
{
	dense_hash_map<unsigned int, MasterCacheData *>::iterator it = m_cacheMap.find( m_que.front() );
	delete it->second;
	m_cacheMap.erase( it );
	m_que.pop_front();

	m_memory -= BLOCK_DATA_SIZE*BLOCK_DATA_SIZE*sizeof(float);
	m_memory -= m_texBlockSize * m_texBlockSize * 3;
}

/*!
	\brief
		Check cache
*/
bool MasterMemCache::CheckCache(unsigned int index)
{
	dense_hash_map<unsigned int, MasterCacheData *>::iterator it = m_cacheMap.find( index );
	if( it != m_cacheMap.end() )
		return true;
	else
		return false;
}

/*!
	\brief
		Get data
*/
MasterCacheData *MasterMemCache::GetData(unsigned int index)
{
	dense_hash_map<unsigned int, MasterCacheData *>::iterator it = m_cacheMap.find( index );
	if( it != m_cacheMap.end() )
		return it->second;
	else
		return NULL;
}

/*------------------------------------------------------------------------------
		Splitter CLASS IMPLEMENTATION
------------------------------------------------------------------------------*/
/*!
	\brief
		Constructor
*/
Splitter::Splitter()
{
}

/*!
	\brief
		Destructor
*/
Splitter::~Splitter()
{

}

/*!
	\brief
		Initialize
		Note : Requested region is adjusted by discrete grid
*/
void Splitter::Init(Grid *grid, float interval, Region *r)
{
	m_bound.Set( grid->m_hLimit[0], grid->m_horizon[grid->m_r], grid->m_hLimit[1], grid->m_horizon[0] );
	m_blockDist			= BLOCK_DATA_SIZE*interval;
	m_nHorizonElements	= (int)ceil( m_bound.Width() / m_blockDist );

	r->lo_x = (float)(m_bound.m_lo[0] + (int)((r->lo_x - m_bound.m_lo[0])/interval + 0.5) * interval);
	r->lo_y = (float)(m_bound.m_lo[1] + (int)((r->lo_y - m_bound.m_lo[1])/interval + 0.5) * interval);
	r->hi_x = (float)(m_bound.m_lo[0] + min((int)((r->hi_x - m_bound.m_lo[0])/interval + 0.5), (int)((m_bound.m_hi[0] - m_bound.m_lo[0])/interval)) * interval);
	r->hi_y = (float)(m_bound.m_lo[1] + min((int)((r->hi_y - m_bound.m_lo[1])/interval + 0.5), (int)((m_bound.m_hi[1] - m_bound.m_lo[1])/interval)) * interval);
}

/*!
	\brief
		Set offset
*/
void Splitter::SetOffset(Region *r, float interval, int magnification)
{
	OrthoRect<float> firstBlock;
	OrthoRect<float> lastBlock;

	GetBound( m_start[1], m_start[0], firstBlock );
	GetBound( m_end[1], m_end[0], lastBlock );

	///< Inner offset
	double denominator	= (double)DEFAULT_TEXTURE_COVERAGE * magnification;
	m_texOffset[LEFT]	= (int)( (r->lo_x - firstBlock.m_lo[0])/denominator + 0.5 );
	m_texOffset[RIGHT]	= (int)( (lastBlock.m_hi[0] - r->hi_x)/denominator + 0.5 );
	m_texOffset[BOTTOM] = (int)( (r->lo_y - firstBlock.m_lo[1])/denominator + 0.5 );
	m_texOffset[TOP]	= (int)( (lastBlock.m_hi[1] - r->hi_y)/denominator + 0.5 );

	///< Texture size
	int blockSize		= (int)((BLOCK_DATA_SIZE * interval)/denominator + 0.5);
	m_texSize[0]		= Width() * blockSize - m_texOffset[LEFT] - m_texOffset[RIGHT];
	m_texSize[1]		= Height() * blockSize - m_texOffset[BOTTOM] - m_texOffset[TOP];
}

/*!
	\brief
		Split task
*/
void Splitter::Split(Region *r, deque<Region> &q)
{
	m_start[0] = (int)((r->lo_x - m_bound.m_lo[0])/m_blockDist);
	m_start[1] = (int)((r->lo_y - m_bound.m_lo[1])/m_blockDist);

	m_end[0]	= (int)((r->hi_x - m_bound.m_lo[0])/m_blockDist);
	m_end[1]	= (int)((r->hi_y - m_bound.m_lo[1])/m_blockDist);

	for (int i = m_start[1]; i <= m_end[1]; ++i)
	{
		for (int j = m_start[0]; j <= m_end[0]; ++j)
		{
			Region task;
			task.code = i*m_nHorizonElements + j;

			task.lo_x = (float)(m_bound.m_lo[0] + m_blockDist * j);
			task.lo_y = (float)(m_bound.m_lo[1] + m_blockDist * i);
			task.hi_x = task.lo_x + m_blockDist;
			task.hi_y = task.lo_y + m_blockDist;

			q.push_back( task );
		}
	}
}

/*!
	\brief
		Get width
*/
int	Splitter::Width() const
{
	return (m_end[0] - m_start[0] + 1);
}

/*!
	\brief
		Get height
*/
int	Splitter::Height() const
{
	return (m_end[1] - m_start[1] + 1);
}

/*!
	\brief
		Split task
*/
void Splitter::GetBound(int row, int col, OrthoRect<float> &bb, int *horizon_flip_coord)
{
	bb.m_lo[0] = (float)(m_bound.m_lo[0] + m_blockDist * col);
	bb.m_lo[1] = (float)(m_bound.m_lo[1] + m_blockDist * row);
	bb.m_hi[0] = bb.m_lo[0] + m_blockDist;
	bb.m_hi[1] = bb.m_lo[1] + m_blockDist;

	if ( horizon_flip_coord != NULL )
	{
		horizon_flip_coord[0] = col - m_start[0];
		horizon_flip_coord[1] = row - m_start[1];
		horizon_flip_coord[1] = (m_end[1] - m_start[1]) - horizon_flip_coord[1];
	}
}

/*!
	\brief
		Get the boundary recursively
*/
void Splitter::GetBound(int index, OrthoRect<float> &bb, int *horizon_flip_coord)
{
	int row = (int)(index / m_nHorizonElements);
	int col = index % m_nHorizonElements;

	GetBound(row, col, bb, horizon_flip_coord);
}