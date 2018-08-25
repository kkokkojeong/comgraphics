
/*!
	\file
		WMPI.cpp

	\brief
		Inline routines in WMPI class
*/

///< Include header file
#include "WMPI.h"

/*------------------------------------------------------------------------------
	WMPI CLASS IMPLEMENTATION
------------------------------------------------------------------------------*/
/*!
	\brief
		Constructor
*/
WMPI::WMPI()
	: m_startTime(0), m_endTime(0)
{
	m_size = MPI::COMM_WORLD.Get_size();	///< find out how many processes
	m_rank = MPI::COMM_WORLD.Get_rank();	///< process rank

	int len;
	MPI::Get_processor_name( m_name, len );
}

/*!
	\brief
		Destructor
*/
WMPI::~WMPI()
{
	
}

/*!
	\brief
		Set start time
*/
double WMPI::TimeBegin()
{
	m_startTime = MPI::Wtime();
	return m_startTime;
}

/*!
	\brief
		Set end time, print elapsed time
*/
double WMPI::TimeEnd()
{
	m_endTime = MPI::Wtime();
	cout << "Elapsed Time : " << m_endTime - m_startTime << " s" << endl;
	return m_endTime;
}

/*------------------------------------------------------------------------------
	Node Class Implementation
------------------------------------------------------------------------------*
/*!
	\brief
		Constructor
*/
Node::Node()
  : WMPI()
{
  DefineDataType();				///< define data type for MPI
 	InitGridInfo();					///< init grid information
  
  ///< connect to file server
  //..
  m_binPath = "/fusionio1/seokcheol/ParallelComputing/bin";
  cout << "[App Control] " << m_name << "[" << m_rank << "] : Get Ready !" << endl; 
  

// 	DWORD error = AddConnection();	
//	if( error == NO_ERROR) {
//	}
// 	else{
// 		cout << "[App Control] "<< m_name << "[" << m_rank << "] : Error occurred - WNET[ "<< error<< " ]" << endl;
// 	}
}

/*!
	\brief
		Destructor
*/
Node::~Node()
{
  MPI_TYPE_REGION.Free();
}

/*!
  \brief
    Define data type for MPI
*/
void Node::DefineDataType()
{
  MPI::Datatype oldType[2]  = { MPI::FLOAT, MPI::INT };    ///< type of elements in each block
  int blen[2]               = { 4, 1 };                    ///< number of elements in each block
  MPI::Aint disp[2];                                       ///< byte displacement of each block
  
  disp[0] = 0;
  disp[1] = MPI::FLOAT.Get_size() * 4;
  
  MPI_TYPE_REGION = MPI::COMPLEX.Create_struct( 2, blen, disp, oldType );
  MPI_TYPE_REGION.Commit();
}

/*!
	\brief
		Initialize information of the grid that split entire region into patch
*/
void Node::InitGridInfo()
{
  ///< SEOGWI
  m_grids[INDEX_SEOGWI].Create(2, 2);
  
  m_grids[INDEX_SEOGWI].m_horizon[0] = 39707.0;
	m_grids[INDEX_SEOGWI].m_horizon[1] = 36934.0;
	m_grids[INDEX_SEOGWI].m_horizon[2] = 34161.0;

	m_grids[INDEX_SEOGWI].m_hLimit[0] = 155023.0;
	m_grids[INDEX_SEOGWI].m_hLimit[1] = 159676.0;

	m_grids[INDEX_SEOGWI].m_vertical[0] = 157350.0;
	m_grids[INDEX_SEOGWI].m_vertical[1] = 157350.0;

	m_grids[INDEX_SEOGWI].m_code[0] = 33611005;
	m_grids[INDEX_SEOGWI].m_code[1] = 33611006;
	m_grids[INDEX_SEOGWI].m_code[2] = 33611015;
	m_grids[INDEX_SEOGWI].m_code[3] = 33611016;
 
 ///< JEJU
	m_grids[INDEX_JEJU].Create(5, 2);

	m_grids[INDEX_JEJU].m_horizon[0] = 53589.0;
	m_grids[INDEX_JEJU].m_horizon[1] = 50804.0;
	m_grids[INDEX_JEJU].m_horizon[2] = 48031.0;
	m_grids[INDEX_JEJU].m_horizon[3] = 45258.0;
	m_grids[INDEX_JEJU].m_horizon[4] = 42486.0;
	m_grids[INDEX_JEJU].m_horizon[5] = 39714.0;

	m_grids[INDEX_JEJU].m_hLimit[0] = 153538.0;
	m_grids[INDEX_JEJU].m_hLimit[1] = 158149.0;

	m_grids[INDEX_JEJU].m_vertical[0] = 155875.0;
	m_grids[INDEX_JEJU].m_vertical[1] = 155855.0;
	m_grids[INDEX_JEJU].m_vertical[2] = 155842.0;
	m_grids[INDEX_JEJU].m_vertical[3] = 155830.0;
	m_grids[INDEX_JEJU].m_vertical[4] = 155817.0;

	m_grids[INDEX_JEJU].m_code[0] = 33607051;
	m_grids[INDEX_JEJU].m_code[1] = -1;
	m_grids[INDEX_JEJU].m_code[2] = 33607061;
	m_grids[INDEX_JEJU].m_code[3] = 33607062;
	m_grids[INDEX_JEJU].m_code[4] = 33607071;
	m_grids[INDEX_JEJU].m_code[5] = 33607072;
	m_grids[INDEX_JEJU].m_code[6] = 33607081;
	m_grids[INDEX_JEJU].m_code[7] = 33607082;
	m_grids[INDEX_JEJU].m_code[8] = 33607091;
	m_grids[INDEX_JEJU].m_code[9] = 33607092;
}

/*------------------------------------------------------------------------------
		Evaluator Class Implementation
------------------------------------------------------------------------------*/
/*!
  \brief
    Constructor
  \param
    n - number of slaves
*/
Evaluator::Evaluator(int n)
: m_nodes(n), m_counts(0), m_elapsedTime(0)
{
  assert(m_nodes);

	m_counts		  = new int[m_nodes];
	m_elapsedTime	= new double[m_nodes];

	InitRecord();
}

/*!
	\brief
		Destructor
*/
Evaluator::~Evaluator()
{
	if(m_counts != 0)
		delete [] m_counts;
   
	if(m_elapsedTime != 0)
		delete [] m_elapsedTime;
}

/*!
	\brief
		Initialize record
*/
void Evaluator::InitRecord()
{
	memset(m_counts, 0, sizeof(int)*m_nodes);
	memset(m_elapsedTime, 0, sizeof(double)*m_nodes);
}

/*!
	\brief
		Increase number of task done by given slave
*/
void Evaluator::AddTaskCount(int index)
{
	assert(0 <= index);
	m_counts[index] += 1;
}

/*!
	\brief
		Add elapsed time
*/
void Evaluator::AddElapsedTime(int index, double t)
{
	assert(0 <= index);
	m_elapsedTime[index] += t;
}

/*!
	\brief
		Save the evaluation
*/
void Evaluator::SaveRecord(const char *fileName)
{
	static int num = 0;
	ofstream out(fileName, ios::app);
	out << num << " : ";
	for(int i = 0; i < m_nodes; ++i)
		out << m_counts[i] << " ";
	out << endl;
	out << num << " : ";
	for(int i = 0; i < m_nodes; ++i)
		out << m_elapsedTime[i] << " ";
	out << endl;
	out.close();
	++num;
}

/*------------------------------------------------------------------------------
	MasterNode Class Implementation
------------------------------------------------------------------------------*/

/*!
	\brief
		Constructor
*/
MasterNode::MasterNode()
	: Node(), m_evaluator(0)
{
	m_bStart	= false;
	m_client	= 0;
 
  m_evaluator = new Evaluator(m_size);
}

/*!
	\brief
		Destructor
*/
MasterNode::~MasterNode()
{
	if( m_evaluator != 0 )
		delete m_evaluator;
}

/*!
	\brief
		Manage compute nodes
*/
bool MasterNode::Run()
{
  m_evaluator->InitRecord();
  TimeBegin();
  
  ///< Split the task
  SplitTask();
  
  cout << "[MPI Control] Parallel Computing Start." << endl;
  cout << "[MPI Control] ";
  PrintRegion( &m_roi );
  cout << "[MPI Control] The whole task was split up into " << m_taskQue.size() << "tasks" << endl;
  
  ///< Allocate memory to put together
  AllocForDataProcessing();
  
  int n     = MIN( m_size - 1, (int)m_taskQue.size() );
  int nTask = 0;
  
  ///< First distribution
  for(int i = 1; i <= n; ++i) {
    ///< Check cache
    CheckCache();
    
    if( m_taskQue.empty() )
      break;
      
    ///< Send common conditions
    SendCondition( i );
    
    ///< Allocating task to i-th slave
    AllocateTask( i );
    
    ++nTask;
  }
  cout << "[MPI Control] Master Node - Complete to distribute tasks " << endl;
  
  ///< Redistribution
  while( nTask ) {
    ///< Receive data
    ReceiveData();
    //static int count = 0;
    //cout << nTask << ", received !!" << endl;
    --nTask;
    
    if( !m_taskQue.empty() ) {
      AllocateTask( m_status.Get_source() );
			++nTask;
    }
  
    cout << m_taskQue.size() << "\r";
    cout.flush();
  }
  
  ///< Adjust edge 
	AdjustEdge();
 
  ///< Save texture
  cvSaveImage( m_resultQue.back()->tex_file_path.c_str(), m_resultQue.back()->tex );
  cvReleaseImage( &m_resultQue.back()->tex );
  
  ///< Finalize
  m_taskQue.clear();
  
  cout << "[MPI Control] Master Node - Complete to redistribution tasks " << endl;
  
  cout << "[MPI Control] ";
  TimeEnd();
  
  m_evaluator->SaveRecord("log.txt");
  
  return true;
}

/*!
  \brief
    Put together data
*/
void MasterNode::PutTogether(int blockNum, MasterCacheData *data)
{
  int blockIndex[2];
  OrthoRect<float> block;
  OrthoRect<float> overlap;
  
  m_splitter.GetBound( blockNum, block, blockIndex );
  
  ///< Calculate overlap region
  overlap.Set(
      MAX( m_roi.lo_x, block.m_lo[0] ),
		  MAX( m_roi.lo_y, block.m_lo[1] ),
		  MIN( m_roi.hi_x + m_interval, block.m_hi[0] ),
		  MIN( m_roi.hi_y + m_interval, block.m_hi[1] )
  );
  
  ///< Set copy information
  
  ///< -------------------------- DEM ------------------------------
  float	*dem_src			    = data->m_heights;
	float	*dem_dst			    = m_resultQue.back()->dem;
	int		dem_dst_width		  = (int)((m_roi.hi_x -m_roi.lo_x)/m_interval) + 1;
	int		dem_src_coord[4]	= {0, 0, BLOCK_DATA_SIZE, BLOCK_DATA_SIZE};
	int		dem_dst_coord[2]	= {0, 0};
	int		dem_copy_size[2]	= {0, 0};

	dem_dst_coord[0] = (int)((overlap.m_lo[0] - m_roi.lo_x) / m_interval);
	dem_dst_coord[1] = (int)((overlap.m_lo[1] - m_roi.lo_y) / m_interval);
  
  ///< -------------------------- Texture --------------------------
  char	*tex_src			    = data->m_pixels;
	char	*tex_dst			    = m_resultQue.back()->tex->imageData;
	int		tex_src_width	  	= m_memCache.m_texBlockSize * 3;
	int		tex_dst_width		  = m_resultQue.back()->tex->widthStep;
	int		tex_src_coord[2]	= {0, 0};
	int		tex_dst_coord[2]	= {0, 0};
	int		tex_copy_size[2]	= { m_memCache.m_texBlockSize,  m_memCache.m_texBlockSize };
 
  ///< Left edge
  if (blockIndex[0] == 0) {
		dem_src_coord[0] = (int)((overlap.m_lo[0] - block.m_lo[0]) / m_interval);
		dem_src_coord[2] = (int)((overlap.m_hi[0] - block.m_lo[0]) / m_interval);

		tex_src_coord[0] = m_splitter.m_texOffset[LEFT];

		if (m_splitter.Width() == 1)
			tex_copy_size[0] = m_splitter.m_texSize[0];
		else
			tex_copy_size[0] = m_memCache.m_texBlockSize - m_splitter.m_texOffset[LEFT];
	}
  
  ///< Right edge
  else if ( blockIndex[0] == m_splitter.Width() - 1 ) {
		dem_src_coord[2] = (int)((overlap.m_hi[0] - block.m_lo[0]) / m_interval);
		tex_copy_size[0] = m_memCache.m_texBlockSize - m_splitter.m_texOffset[RIGHT];
	}
  
  ///< Top edge
  if (blockIndex[1] == 0) {
		dem_src_coord[1] = (int)((overlap.m_lo[1] - block.m_lo[1]) / m_interval);
		dem_src_coord[3] = (int)((overlap.m_hi[1] - block.m_lo[1]) / m_interval);

		tex_src_coord[1] = m_splitter.m_texOffset[TOP];

		if (m_splitter.Height() == 1)
			tex_copy_size[1] = m_splitter.m_texSize[1];
		else
			tex_copy_size[1] = m_memCache.m_texBlockSize - m_splitter.m_texOffset[TOP];
	}
  
  ///< Bottom edge
  else if ( blockIndex[1] == m_splitter.Height() - 1 ) {
		dem_src_coord[1] = (int)((overlap.m_lo[1] - block.m_lo[1]) / m_interval);
		tex_copy_size[1] = m_memCache.m_texBlockSize - m_splitter.m_texOffset[BOTTOM];
	}
 
  tex_dst_coord[0] = MAX((blockIndex[0] - 1), 0) * m_memCache.m_texBlockSize + (blockIndex[0]!=0) * (m_memCache.m_texBlockSize - m_splitter.m_texOffset[LEFT]);
	tex_dst_coord[1] = MAX((blockIndex[1] - 1), 0) * m_memCache.m_texBlockSize + (blockIndex[1]!=0) * (m_memCache.m_texBlockSize - m_splitter.m_texOffset[TOP]);

	dem_copy_size[0] = dem_src_coord[2] - dem_src_coord[0];
	dem_copy_size[1] = dem_src_coord[3] - dem_src_coord[1];
  
  ///< Copy DEM data
  dem_src	+= dem_src_coord[1] * BLOCK_DATA_SIZE + dem_src_coord[0];
	dem_dst += dem_dst_coord[1] * dem_dst_width + dem_dst_coord[0];

	for (int i = 0; i < dem_copy_size[1]; ++i)
	{
		memcpy( dem_dst, dem_src, sizeof(float) * dem_copy_size[0] );
		dem_src += BLOCK_DATA_SIZE;
		dem_dst += dem_dst_width;
	}
  
  ///< Copy texture data
  int bytes = tex_copy_size[0] * 3;
	tex_src += tex_src_coord[1] * tex_src_width + tex_src_coord[0] * 3;
	tex_dst += tex_dst_coord[1] * tex_dst_width + tex_dst_coord[0] * 3;

	for (int i = 0; i < tex_copy_size[1]; ++i)
	{
		memcpy( tex_dst, tex_src, bytes );
		tex_src += tex_src_width;
		tex_dst += tex_dst_width;
	}
}

/*!
  \brief
    Adjust edge
*/
void MasterNode::AdjustEdge()
{
  int index		    = CITY_CODE_TO_INDEX(m_cityCode);
	float *dem_dst	= m_resultQue.back()->dem;
	int dem_w		    = (int)((m_roi.hi_x - m_roi.lo_x)/m_interval) + 1;
	int dem_h		    = (int)((m_roi.hi_y - m_roi.lo_y)/m_interval) + 1;

	if ( m_grids[index].m_hLimit[1] < m_roi.hi_x + m_interval )
	{
		for (int i = 0; i < dem_h; ++i)
			dem_dst[(i+1)*dem_w - 1] = dem_dst[(i+1)*dem_w - 2];
	}

	if ( m_grids[index].m_horizon[0] < m_roi.hi_y + m_interval )
		memcpy(dem_dst + (dem_h - 1) * dem_w, dem_dst + (dem_h - 2) * dem_w, sizeof(float)*dem_w );
}

/*!
	\brief
		Check cache
*/
void MasterNode::CheckCache()
{
	while ( !m_taskQue.empty() && !m_memCache.Empty() && m_memCache.CheckCache(m_taskQue.front().code) )
	{
		PutTogether(m_taskQue.front().code, m_memCache.GetData(m_taskQue.front().code) );
		m_taskQue.pop_front();
	}
}

/**
	\brief
		Allocating memory to put together
*/
void MasterNode::AllocForDataProcessing()
{
	int dem_w = (int)((m_roi.hi_x - m_roi.lo_x)/m_interval) + 1;
	int dem_h = (int)((m_roi.hi_y - m_roi.lo_y)/m_interval) + 1;

	///< Create result data
	m_resultQue.push_back( new Result );

	///< Create DEM
	m_resultQue.back()->dem = new float[dem_w*dem_h];
	
	m_resultQue.back()->dem_size[0] = dem_w;
	m_resultQue.back()->dem_size[1] = dem_h;

	m_resultQue.back()->bound[0] = (unsigned int)m_roi.lo_x;
	m_resultQue.back()->bound[1] = (unsigned int)m_roi.lo_y;
	m_resultQue.back()->bound[2] = (unsigned int)m_roi.hi_x;
	m_resultQue.back()->bound[3] = (unsigned int)m_roi.hi_y;

	///< Create texture
	m_resultQue.back()->tex = cvCreateImage( cvSize(m_splitter.m_texSize[0], m_splitter.m_texSize[1]), IPL_DEPTH_8U, 3 );

	///< Set texture name
	struct tm *t;
	time_t timer;

  time(&timer);
	t = localtime( &timer );

	char tex_file_name[20];
	sprintf( tex_file_name, "%04d%02d%02d%02d%02d%02d.jpg",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec );

	m_resultQue.back()->tex_file_path = m_binPath;
	m_resultQue.back()->tex_file_path += "/";
	m_resultQue.back()->tex_file_path += tex_file_name;
}

/*!
	\brief
		Allocating task to slave
*/
void MasterNode::AllocateTask(int slave_num)
{
	m_command = RECEIVE_WORKLOAD;
	MPI::COMM_WORLD.Send( &m_command, 1, MPI::INT, slave_num, 0 );
	MPI::COMM_WORLD.Send( &(m_taskQue.front()), 1, MPI_TYPE_REGION, slave_num, 0 );

	m_taskQue.pop_front();
}

/*!
  \brief
    Receive data from slave nodes
*/
void MasterNode::ReceiveData()
{
  bool nullData;
  double t;
  
  MPI::COMM_WORLD.Recv( &nullData, 1, MPI::BOOL, MPI::ANY_SOURCE, MPI::ANY_TAG, m_status );
  MPI::COMM_WORLD.Recv( &t, 1, MPI::DOUBLE, MPI::ANY_SOURCE, MPI::ANY_TAG, m_status );
  
  m_evaluator->AddTaskCount( m_status.Get_source() );
  m_evaluator->AddElapsedTime( m_status.Get_source(), t );
  
  if( nullData ) {
    m_memCache.Insert( m_status.Get_tag(), NULL );
    return;
  }
  
  ///< Allocate memory
  MasterCacheData *data = new MasterCacheData( 
		BLOCK_DATA_SIZE*BLOCK_DATA_SIZE*sizeof(float),			  ///< memory size for DEM
		m_memCache.m_texBlockSize*m_memCache.m_texBlockSize*3	///< memory size for texture
  );
  
  ///< Transfer DEM
  MPI::COMM_WORLD.Recv(
      data->m_heights,
      sizeof(float)*BLOCK_DATA_SIZE*BLOCK_DATA_SIZE,
      MPI::CHAR,
      m_status.Get_source(),
      m_status.Get_tag(),
      m_status 
  );
  
  ///< Transfer texture
  int bytes = m_memCache.m_texBlockSize * 3;
  for(int i = 0; i < m_memCache.m_texBlockSize; ++i) {
    MPI::COMM_WORLD.Recv(
      data->m_pixels + i * bytes,
      bytes,
      MPI::CHAR,
      m_status.Get_source(),
      m_status.Get_tag(),
      m_status);
  }
  
  m_memCache.Insert( m_status.Get_tag(), data );
  
  PutTogether( m_status.Get_tag(), data );
}

/*!
  \brief
    Sending common conditions
*/
void  MasterNode::SendCondition(int slave_num)
{
  m_command = RECEIVE_BROADCAST;
  MPI::COMM_WORLD.Send( &m_command, 1, MPI::INT, slave_num, 0 );
	MPI::COMM_WORLD.Send( &m_cityCode, 1, MPI::INT, slave_num, 0 );			    ///< City code
	MPI::COMM_WORLD.Send( &m_interval, 1, MPI::FLOAT, slave_num, 0 );		    ///< Interval
	MPI::COMM_WORLD.Send( &m_magnification, 1, MPI::INT, slave_num, 0 );	  ///< Magnification
}

/*!
  \brief
    Terminate all slave
*/
bool MasterNode::CloseSlaves()
{
  m_command = TERMINATE_PROCESS;
  for(int i = 1; i < m_size; ++i) {
    MPI::COMM_WORLD.Send( &m_command, 1, MPI::INT, i, 0 );
  }
  
  return true;
}

/*!
  \brief
    Set information of the task
*/
void MasterNode::SetTaskInfo(Region *r, float interval, int magnification)
{
  if( m_bStart ) {
    
    ///< If condition changed, cache is initailized
    if( r->code != m_roi.code || m_interval != interval || m_magnification != magnification )
    {
      m_memCache.Release();
      m_memCache.Init( r->code, interval, magnification );
    }
  }
  else {
    m_bStart = true;
    m_memCache.Init( r->code, interval, magnification );
  }
  cout << interval << ", " << magnification << endl;
  
  m_roi           = *r;             ///< region
  m_cityCode      = r->code;        ///< what city
  m_interval      = interval;       ///< interval of the elevation map
  m_magnification = magnification;  ///< magnification of the texture
}

/*!
  \brief
    Split task to compute in parallel
*/
void MasterNode::SplitTask()
{
  int index = CITY_CODE_TO_INDEX(m_cityCode);
  
  m_splitter.Init( &(m_grids[index]), m_interval, &m_roi );
  m_splitter.Split( &m_roi, m_taskQue );
  m_splitter.SetOffset( &m_roi, m_interval, m_magnification );
}

/*!
  \brief
    Get a result in parallel
*/
Result *MasterNode::GetResult()
{
  Result *r = m_resultQue.front();
  m_resultQue.pop_front();
  return r;
}

/*------------------------------------------------------------------------------
	SlaveNode Class Implementation
------------------------------------------------------------------------------*/

/*!
	\brief
		Constructor
*/
SlaveNode::SlaveNode()
	: Node(), m_diskCache(), m_memCache()
{
	m_data = 0;
}

/*!
	\brief
		Destructor
*/
SlaveNode::~SlaveNode()
{

}

/*!
  \brief
    Compute given task in parallel
*/
bool SlaveNode::Run()
{
  while( true ) 
  {
  
    ///< Receive command
    MPI::COMM_WORLD.Recv( &m_command, 1, MPI::INT, MASTER, MPI::ANY_TAG, m_status );
    
    ///< Run process
    if( m_command == RECEIVE_WORKLOAD )
    {
      m_elapsedTime = MPI::Wtime();
      
      //cout << "[MPI Control] "<< "Slave Node[ " << m_rank << " ] is running..." << endl;
      
      ///< Receive region
      MPI::COMM_WORLD.Recv( &m_roi, 1, MPI_TYPE_REGION, MASTER, MPI::ANY_TAG, m_status );
      
      ///< Split region
      SplitBlock();
      
      ///< Allocate memory to pull together
      if( !CreateProcessData() )
      {
        cout << "[MPI Control] " << "Slave Node[ " << m_rank << " ] : Unable to create processing data." << endl;
        return false;
      }
      
      bool nullData = true;
      for( size_t i = 0; i < m_partition.size(); ++i )
      {
        if( m_partition[i].code == -1 )
        {
          nullData &= true;
          continue;
        }
        else
        {
          nullData = false;
        }
        
        ///< Check cache
        if( !CheckCache( m_partition[i].code ) )
        {
          cout << "[MPI Control] "<< "Slave Node[ " << m_rank << " ] : Unable to read data." << endl;
          return false;
        }
        
        ///< Do extraction
        ExtractDEM( &(m_partition[i]) );
        ExtractTexture( &(m_partition[i]) );
      }
      
      ///< Data scaling
      ScaleData();
    
      m_elapsedTime = MPI::Wtime() - m_elapsedTime;
    
      ///< Transfer data to master
      TransferData( nullData );
    
      ///< Release processed data
      m_processedData.dem.Release();
      m_processedData.tex.Destroy();
      //m_processedData.tex.GetImage()
      
    }
    
    ///< Broadcasting
    else if( m_command == RECEIVE_BROADCAST ) 
    {
      MPI::COMM_WORLD.Recv( &m_cityCode, 1, MPI::INT, MASTER, MPI::ANY_TAG, m_status );
      MPI::COMM_WORLD.Recv( &m_interval, 1, MPI::FLOAT, MASTER, MPI::ANY_TAG, m_status );
      MPI::COMM_WORLD.Recv( &m_magnification, 1, MPI::INT, MASTER, MPI::ANY_TAG, m_status );
    }
  
    ///< Terminate process
    else if( m_command == TERMINATE_PROCESS ) 
    {
      cout << "[MPI Control] " << "Slave Node[ " << m_rank << " ] is terminated." << endl;
      break; 
    }
  }
  
  return true;
}

/*!
  \brief
    Search block index including given point
*/
bool SlaveNode::SearchBlock(float coord_x, float coord_y, int &row, int &col)
{
  int index = CITY_CODE_TO_INDEX( m_cityCode );
  
  ///< Vertical
  for( row = 0; row <= m_grids[index].m_r; ++row )
  {
    if( m_grids[index].m_horizon[row] <= coord_y )
      break;
  }
  
  --row;
  
  ///< Horizontal
  if( m_grids[index].m_hLimit[1] < coord_x )
  {
    col = m_grids[index].m_c;
    return false;
  }
  
  int correction_row = row < 0 ? 0 : row;
	int rowOffset = (m_grids[index].m_c - 1) * correction_row;
  for (col = 0; col < m_grids[index].m_c - 1; ++col)
	{
		if ( coord_x <= m_grids[index].m_vertical[rowOffset + col] )
			break;
	}

	return (row < 0);
}

/*!
  \brief
    Split block according to the grid of the patches
*/
void SlaveNode::SplitBlock()
{
  int index = CITY_CODE_TO_INDEX( m_cityCode );
  m_partition.clear();
  
  int  lower_r, lower_c;    ///< lower row, column index
  int  upper_r, upper_c;    ///< upper row, column index
  
  SearchBlock( m_roi.lo_x, m_roi.lo_y, lower_r, lower_c );
  SearchBlock( m_roi.hi_x, m_roi.hi_y, upper_r, upper_c );
  
  ///< The block is in the one patch
  if( lower_c == upper_c && lower_r == upper_r )
  {
    Region r = { m_roi.lo_x, m_roi.lo_y, m_roi.hi_x, m_roi.hi_y, m_grids[index].GetCode( lower_r, lower_c) };
    m_partition.push_back(r);
  }
  
  ///< The block is in the four patches.
  else if ( lower_c != upper_c && lower_r != upper_r)
	{
		float tCut_x = (m_grids[index].m_c <= upper_c) ? m_grids[index].m_hLimit[1] : m_grids[index].m_vertical[upper_r*(m_grids[index].m_c-1) + lower_c];
		float bCut_x = (m_grids[index].m_c <= upper_c) ? m_grids[index].m_hLimit[1] : m_grids[index].m_vertical[lower_r*(m_grids[index].m_c-1) + lower_c];
		float cut_y  = m_grids[index].m_horizon[lower_r];

		Region tl = { m_roi.lo_x, cut_y, tCut_x, m_roi.hi_y, m_grids[index].GetCode( upper_r, lower_c ) };
		Region tr = { tCut_x, cut_y, m_roi.hi_x, m_roi.hi_y, m_grids[index].GetCode( upper_r, upper_c ) };
		Region bl = { m_roi.lo_x, m_roi.lo_y, bCut_x, cut_y, m_grids[index].GetCode( lower_r, lower_c ) };
		Region br = { bCut_x, m_roi.lo_y, m_roi.hi_x, cut_y, m_grids[index].GetCode( lower_r, upper_c ) };

		m_partition.push_back( tl );
		m_partition.push_back( tr );
		m_partition.push_back( bl );
		m_partition.push_back( br );

	}
  
  ///< The block is in the two patches horizontally.
  else if ( lower_c != upper_c )
	{
		float cut_x = m_grids[index].m_c <= upper_c ? m_grids[index].m_hLimit[1] : m_grids[index].m_vertical[lower_r*(m_grids[index].m_c-1)+lower_c];
		Region l = { m_roi.lo_x, m_roi.lo_y, cut_x, m_roi.hi_y, m_grids[index].GetCode( lower_r, lower_c) };
		Region r = { cut_x, m_roi.lo_y, m_roi.hi_x, m_roi.hi_y, m_grids[index].GetCode( upper_r, upper_c) };

		m_partition.push_back( l );
		m_partition.push_back( r );
	}
  
  ///< The block is in the two patches vertically.
  else
	{
		float cut_y = m_grids[index].m_horizon[lower_r];
		Region t = { m_roi.lo_x, cut_y, m_roi.hi_x, m_roi.hi_y, m_grids[index].GetCode( upper_r, upper_c) };
		Region b = { m_roi.lo_x, m_roi.lo_y, m_roi.hi_x, cut_y, m_grids[index].GetCode( lower_r, lower_c) };

		m_partition.push_back( t );
		m_partition.push_back( b );
	}
}

/*!
	\brief
		Check whether data exist in the cache.
		If memory cache hit, fetch the data.
		Otherwise, read data from disk or file server.
*/
bool SlaveNode::CheckCache(int region_code)
{
	///< Check memory cache
	if ( m_memCache.Check( region_code ) == CacheHit )
	{
		m_data = m_memCache.GetData( region_code );
		return true;
	}

	///< Memory cache miss
	m_data = new SlaveCacheData;
	if( m_data == NULL )
	{
		cout << "[MPI Control] "<< "Slave Node[ " << m_rank << " ] can't create data." << endl;
		return false;
	}

	bool	hit		= m_diskCache.Check( region_code );
	string	path	= hit ? m_binPath : "";

	///< If hit is true, then read data from local disk.
	///< Otherwise, read data from file server.

	///< Read DEM
	if ( !m_data->dem.Read( GetFilePath( region_code, m_cityCode, FILE_DEM, path ).c_str(), true ) )
	{
		cout << "[MPI Control] "<< "Slave Node[ " << m_rank << " ] : Unable to open DEM file." << endl;
		return false;
	}

	///< Read world file
	if ( !m_data->tex.GetWorldInfo().Read( GetFilePath( region_code, m_cityCode, FILE_WORLD, path  ).c_str() ) )
	{
		cout << "[MPI Control] "<< "Slave Node[ " << m_rank << " ] : Unable to open world file." << endl;
		return false;
	}

	///< Read texture
	if( !m_data->tex.Load( GetFilePath( region_code, m_cityCode, FILE_TEXTURE, path ).c_str(), CV_LOAD_IMAGE_COLOR ) )
	{
		cout << "[MPI Control] "<< "Slave Node[ " << m_rank << " ] : Unable to open texture file." << endl;
		return false;
	}

	///< Update information of the cache
	if( !hit )
	{
		m_data->dem.Write( GetFilePath( region_code, m_cityCode, FILE_DEM, m_binPath ).c_str() );
		m_data->tex.GetWorldInfo().Write( GetFilePath( region_code, m_cityCode, FILE_WORLD, m_binPath  ).c_str() );
		m_data->tex.Save( GetFilePath( region_code, m_cityCode, FILE_TEXTURE, m_binPath ).c_str() );

		m_diskCache.Set( region_code, true );
	}

	m_memCache.AddData( region_code, m_data );

	return true;
}

/*!
	\brief
		Allocates memory to processing data
*/
bool SlaveNode::CreateProcessData()
{
	///< Create DEM
	int w = (int)(m_roi.hi_x - m_roi.lo_x) + 1;
	int h = (int)(m_roi.hi_y - m_roi.lo_y) + 1;
	if ( !m_processedData.dem.Create( w, h ) )
		return false;

	///< Create texture
	w = (int)((m_roi.hi_x - m_roi.lo_x)/DEFAULT_TEXTURE_COVERAGE + 0.5);
	h = (int)((m_roi.hi_y - m_roi.lo_y)/DEFAULT_TEXTURE_COVERAGE + 0.5);
	
	if ( !m_processedData.tex.Create( w, h, 24) )
		return false;

	return true;
}

/*!
	\brief
		Extracts image
*/
bool SlaveNode::ExtractDEM(Region *r)
{
	///< Calculate extent of necessary area
	int start[2], end[2], w, h;

	m_data->dem.GetCell( r->lo_x, r->lo_y, start );
	m_data->dem.GetCell( r->hi_x, r->hi_y, end );
	
	w = end[0] - start[0] + 1;
	h = end[1] - start[1] + 1;

	/*
	m_data->dem.PrintInfo();
	cout << "Alloc region: ";
	PrintRegion( &m_roi );
	cout << "Split region[" << m_roi.code << "] :";
	PrintRegion( r );
	cout << "SRC: "<< start[0] << " " << start[1] << " " << end[0] << " " << end[1] << endl;
	cout << "DST: "<< (int)(r->lo_x - m_roi.lo_x) << " " << (int)(r->lo_y - m_roi.lo_y) << " " << w << " " << h << endl;
	*/
	

	///< Crop DEM
	m_processedData.dem.SetROI( (int)(r->lo_x - m_roi.lo_x), (int)(r->lo_y - m_roi.lo_y), w, h );
	m_data->dem.Crop( start[0], start[1], w, h, m_processedData.dem );
	m_processedData.dem.ResetROI();

	return true;
}

/*!
	\brief
		Extracts image
*/
bool SlaveNode::ExtractTexture(Region *r)
{
	///< Size of the region of interest
	float	roi_size[2] = { m_roi.hi_x - m_roi.lo_x, m_roi.hi_y - m_roi.lo_y };
	///< start index of the source 
	double	src_start[2];
	///< start index of the destination
	int		dst_start[2] = { (int)( (r->lo_x - m_roi.lo_x)/roi_size[0] * m_processedData.tex.Width() + 0.5),
							 (int)( (m_roi.hi_y - r->hi_y)/roi_size[1] * m_processedData.tex.Height() + 0.5) };

	/// Extent of image to be extracted
	int		w = (int)( m_processedData.tex.Width() * (r->hi_x - r->lo_x) / roi_size[0] + 0.5 );
	int		h = (int)( m_processedData.tex.Height() * (r->hi_y - r->lo_y) / roi_size[1] + 0.5 );

	m_data->tex.GetWorldInfo().GetImagePt( r->lo_x, r->hi_y, src_start );

	///< Check destination boundary
	if ( m_processedData.tex.Width() < dst_start[0] + w)
		w -= dst_start[0] + w - m_processedData.tex.Width();

	if ( m_processedData.tex.Height() < dst_start[1] + h)
		h -= dst_start[1] + h - m_processedData.tex.Height();

	///< Check source boundary
	if ( m_data->tex.Width() < (int)(src_start[0]) + w)
		src_start[0] -= (int)(src_start[0]) + w - m_data->tex.Width();

	if ( m_data->tex.Height() < (int)(src_start[1]) + h)
		src_start[1] -= (int)(src_start[1]) + h - m_data->tex.Height();

	///< Crop texture
	cvSetImageROI( m_processedData.tex.GetImage(), cvRect( dst_start[0], dst_start[1], w, h) );

	m_data->tex.Crop( (int)(src_start[0]), (int)(src_start[1]), w, h, m_processedData.tex );

	cvResetImageROI( m_processedData.tex.GetImage() );

	return true;
}

/*!
	\brief
		Scale data
*/
bool SlaveNode::ScaleData()
{
	int w, h;

	///< Resize DEM
	w = (int)((float)(m_roi.hi_x - m_roi.lo_x)/m_interval);
	h = (int)((float)(m_roi.hi_y - m_roi.lo_y)/m_interval);
 // cout << "w,h : " << w << ", " << h <<endl;
	m_processedData.dem.Resize(w, h);
	
	///< Resize texture
	w = (int)((float)(m_roi.hi_x - m_roi.lo_x)/(DEFAULT_TEXTURE_COVERAGE * m_magnification) + 0.5);
	h = (int)((float)(m_roi.hi_y - m_roi.lo_y)/(DEFAULT_TEXTURE_COVERAGE * m_magnification) + 0.5);
	
	m_processedData.tex.Resize(w, h);

	return true;
}

/*!
	\brief
		Transfer data
*/
bool SlaveNode::TransferData(bool nullData)
{

	MPI::COMM_WORLD.Send( &nullData, 1, MPI::BOOL, MASTER, m_roi.code );
	MPI::COMM_WORLD.Send( &m_elapsedTime, 1, MPI::DOUBLE, MASTER, m_roi.code );

	if ( nullData )
		return true;

	///< Transfer DEM
	MPI::COMM_WORLD.Send( 
		m_processedData.dem.GetData(),
		m_processedData.dem.GetDataSize(),
		MPI::CHAR, 
		MASTER, 
		m_roi.code );

	///< Transfer texture
	int bytes = m_processedData.tex.Width() * 3;
	for( int i = 0; i < m_processedData.tex.Height(); ++i )
	{
		MPI::COMM_WORLD.Send( 
			m_processedData.tex.GetData( i ), 
			bytes,
			MPI::CHAR, 
			MASTER, 
			m_roi.code );
	}

	return true;
}









