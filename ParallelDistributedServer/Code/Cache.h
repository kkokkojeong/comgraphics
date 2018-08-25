
#ifndef CACHE_H
#define CACHE_H

#include <google/dense_hash_map>
#include <deque>
#include <iostream>
#include <fstream>
#include <string>
#include "Topography.h"

#define DISK_CACHE_FILE_NAME "disk_cache.txt"

#define NUM_SEOGWI_DATA	4
#define NUM_JEJU_DATA	10

#define CacheHit		1
#define CacheMiss		0

#define SLAVE_CACHE_MEM_LIMIT	  838860800
#define MASTER_CACHE_MEM_LIMIT	1073741824	///< 1 GB

#define BLOCK_DATA_SIZE 64

using namespace google;
using namespace::std;

class DiskCache;
class SlaveCacheData;
class SlaveMemCache;
class MasterCacheData;
class MasterMemCache;
class Splitter;

/*------------------------------------------------------------------------------

							Cache on Slave

------------------------------------------------------------------------------*/

/*------------------------------------------------------------------
	CLASS FOR DISKCACHE
--------------------------------------------------------------------*/
/*!
  \brief
    a class for disk cache
*/
class DiskCache
{
public:
  /* Constructor */
  DiskCache();
  virtual ~DiskCache();
  
  ///< Prevent malfunction
  DiskCache( const DiskCache & );
  DiskCache &operator=( const DiskCache & );
  
  /* Member functions */
  bool Read();
  void Update();
  void Set(int region_code, bool exist);
  bool Check(int region_code);
  
protected:
  /* Member variables */
  string                    m_binPath;
  dense_hash_map<int, bool>	m_cache;
};

/*------------------------------------------------------------------
	CLASS FOR SLAVECACHEDATA
--------------------------------------------------------------------*/
class SlaveCacheData
{
public:
  /* Constructor & Destructor */
  SlaveCacheData() : hit(0) { }
  virtual ~SlaveCacheData() { }
  
  ///< Prevent malfunction
  SlaveCacheData( const SlaveCacheData & );
  SlaveCacheData &operator=( const SlaveCacheData & );
  
public:
  /* Member variables */
  int     hit;
  DEM     dem;
  Texture tex;
  
};

/*------------------------------------------------------------------
	CLASS FOR SLAVEMEMCACHE
--------------------------------------------------------------------*/
/**
	@brief
		A class for memory cache on slave
*/
class SlaveMemCache
{
public:
  /* Constructor & Destructor */
	SlaveMemCache();
	virtual ~SlaveMemCache();

	///< Prevent malfunction
	SlaveMemCache( const SlaveMemCache & );
	SlaveMemCache &operator=( const SlaveMemCache & );

  /* Member functions */
	int GetMemSize() const;
	bool Check(int region_code);
	SlaveCacheData *GetData(int region_code);
	void AddData(int region_code, SlaveCacheData *data);
	void DeleteData(int region_code);
	void DiscardData(int exception);

protected:
  /* Member variables */
	int m_memory;
	dense_hash_map<int, SlaveCacheData *>	m_cache;
};


/*------------------------------------------------------------------------------

							Cache on Master

------------------------------------------------------------------------------*/
/*------------------------------------------------------------------
	CLASS FOR MASTERCACHEDATA
--------------------------------------------------------------------*/
/*!
  \brief
    A class for master cache data
*/
class MasterCacheData
{
public:
  /* Constructor & Destructor */
  MasterCacheData(int dem_size, int tex_size);
  virtual ~MasterCacheData();
  
  ///< Prevent malfunction
  MasterCacheData( const MasterCacheData & );
  MasterCacheData &operator=( const MasterCacheData & );
  
public:
  /* Member variables */
  float  *m_heights;
  char   *m_pixels;
};

/*------------------------------------------------------------------
	CLASS FOR MASTERMEMCACHE
--------------------------------------------------------------------*/
/*!
	\brief
		A class for master cache
*/
class MasterMemCache
{
  ///< friend class
  friend class MasterNode;
  
public:
  /* Constructor & Destructor */
  MasterMemCache();
	virtual ~MasterMemCache();
 
  ///< Prevent malfunction
	MasterMemCache( const MasterMemCache & );
	MasterMemCache &operator=( const MasterMemCache & );
 
  /* Member functions */
  void Init(int m_cityCode, float m_interval, int m_magnification);
	void Release();
	bool Empty();
	void Insert(int index, MasterCacheData *data);
	void Discard();
	bool CheckCache(unsigned int index);
	MasterCacheData *GetData(unsigned int index);
 
protected:
  /* Member variables */
  ///< Complete data information
	OrthoRect<float>	m_boundary[2];
 
  ///< Current data information
	unsigned int		m_memory;			      ///< amount of memory
	int					    m_cityCode;			    ///< What city
	float			    	m_interval;			    ///< DEM interval
	int				    	m_magnification;	  ///< Texture magnification
	int					    m_texBlockSize;		  ///< Size of block data of texture
      
  ///< Data structure for cache
	dense_hash_map<unsigned int, MasterCacheData*> m_cacheMap;
	deque<int>			m_que;
	int					    m_w;
	int					    m_h;
};

enum LOCATION {
	BOTTOM = 0,
	TOP,
	LEFT,
	RIGHT
};

/*------------------------------------------------------------------
	CLASS FOR SPLITTER
--------------------------------------------------------------------*/
/*!
	\brief
		A class for dividing task
*/
class Splitter
{
public:
  /* Constructor & Destructor */
	Splitter();
	virtual ~Splitter();

	///< Prevent malfunction
	Splitter( const Splitter & );
	Splitter &operator=( const Splitter & );

  /* Member functions */
	void Init(Grid *grid, float interval, Region *r);
	void SetOffset(Region *r, float interval, int magnification);
	void Split(Region *r, deque<Region> &q);
	int	Width() const;
	int	Height() const;
	void GetBound(int row, int col, OrthoRect<float> &bb, int *horizon_flip_coord = NULL);
	void GetBound(int index, OrthoRect<float> &bb, int *horizon_flip_coord = NULL);

public:
  /* Member variables */
	int			      		m_start[2];			      ///< start block index
	int			      		m_end[2];			        ///< end block index
	OrthoRect<double>	m_bound;			        ///< specified city boundary
	float			      	m_interval;			      ///< interval
	int				      	m_nHorizonElements;	  ///< number of horizontal blocks
	int				      	m_texOffset[4];
	int				      	m_texSize[2];
	float			      	m_blockDist;		      ///< a block size(m)
};

#endif