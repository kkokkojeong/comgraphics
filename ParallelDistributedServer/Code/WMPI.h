/*!
	\file
		WMPI.h

	\brief
		a class for wrapping MPI routines and its derived classes
*/
#ifndef WMPI_H 
#define WMPI_H

#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <sys/param.h>
#include <deque>
#include <vector>

//#include "WNetwk.h"
#include "Topography.h"
#include "Cache.h"

using namespace std;

#define MASTER		0		///< rank of master node

enum COMMAND_TO_SLAVE{
	TERMINATE_PROCESS,
	SUSPEND_PROCESS,
	RECEIVE_WORKLOAD,
	RECEIVE_BROADCAST,
};

/*!
	\brief
		MPI wrapping class
*/
class WMPI
{
public:
	/* Constructor & Destructor */
	WMPI();
	virtual ~WMPI();

	///< Prevent malfunction
	WMPI( const WMPI & );
	WMPI &operator=( const WMPI & );

	/* Member functions */
	double	TimeBegin();
	double	TimeEnd();
 
  const char *GetName() const { return m_name;   }
  int GetRank() const         { return m_rank;   }
  int GetSize() const         { return m_size;   }
  int &GetTag()               { return m_tag;    }
  MPI::Status &GetStatus()    { return m_status; }
  bool IsMaster() const       { return m_rank == MASTER; }

protected:
	/* Member variables */
	char		          m_name[MPI_MAX_PROCESSOR_NAME];		///< process name
	int			          m_rank;								            ///< process id number
	int			          m_size;								            ///< number of process
	int			          m_tag;							            	///< tag
	MPI::Status	      m_status;						            	///< envelope information
	double		        m_startTime;						          ///< start time
	double		        m_endTime;						           	///< end time
};

/*!
	\brief
		a class for node
*/
class Node : public WMPI
{
public:
	/* Constructor & Destructor */
	Node();
	virtual ~Node();

	///< Prevent malfunction
	Node( const Node & );
	Node &operator=( const Node & );
 
  virtual bool Run() = 0; 

protected:
  /* Member functions */
  void DefineDataType();
  void InitGridInfo();
  
public:
  /* Member variables */
  ///< using on both master and slave
  string          m_binPath;              ///< bin directory path
  Grid            m_grids[NUM_CITY];      ///< grid information
  MPI::Datatype   MPI_TYPE_REGION;        ///< a user-defined MPI data type
  int             m_command;              ///< indicate that slave what to do
  int             m_cityCode;             ///< what city?
  Region          m_roi;                  ///< region of interest
  float           m_interval;             ///< interval of DEM
  int             m_magnification;        ///< magnification of the texture
};

/*!
	\brief
		The result of parallel tasking
*/
struct Result  
{
	unsigned int bound[4];			///< Region
	unsigned int dem_size[2];		///< Size of DEM
	float		*dem;				        ///< DEM
	IplImage	*tex;				      ///< Texture
	string		tex_file_path;		///< Path of the texture
};

/*!
  \brief
    Evaluate performace(load balance)
*/
class Evaluator
{
public:
  /* Constructor & Destructor */
  Evaluator(int n);
  virtual ~Evaluator();
  
  ///< Prevent malfunction
  Evaluator( const Evaluator & );
  Evaluator &operator=( const Evaluator & );
  
  /* Member functions */
  void InitRecord();
  void AddTaskCount(int index);
  void AddElapsedTime(int index, double t);
  void SaveRecord(const char *fileName);
  
protected:
  /* Member variables */
  int    m_nodes;
  int    *m_counts;      ///< number of task which is done by slave for each slave
  double *m_elapsedTime;  ///< elapsed time for each slave
};

/*!
	\brief
		a class for master node
*/
class MasterNode : public Node
{
public:
	/* Constructor & Destructor */
	MasterNode();
	~MasterNode();

	///< Prevent malfunction
	MasterNode( const MasterNode & );
	MasterNode &operator=( const MasterNode & );
 
  /* Member functions */
  virtual bool Run();
  bool CloseSlaves();
  void SetTaskInfo(Region *r, float interval, int magnification);
  void SplitTask();
	void AllocForDataProcessing();
	void AllocateTask(int slave_num);
	void ReceiveData();
	void SendCondition(int slave_num);
	void PutTogether(int blockNum, MasterCacheData *data);
	void AdjustEdge();
	void CheckCache();
	Result *GetResult();

	//inline void Test();

protected:
	/* Member variables */
	bool				    m_bStart;
	int					    m_client;
  Splitter        m_splitter;
  MasterMemCache  m_memCache;        ///< Cache manager
  deque<Region>   m_taskQue;         ///< Task queue
  deque<Result *> m_resultQue;       ///< Result queue
  Evaluator       *m_evaluator;
};

/*!
	\brief
		a class for slave node
*/
class SlaveNode : public Node
{
public:
	/* Constructor & Destructor */
	SlaveNode();
	~SlaveNode();

	///< Prevent malfunction
	SlaveNode( const SlaveNode & );
	SlaveNode &operator=( const SlaveNode & );
 
  /* Member functions */
  virtual bool Run();

	bool SearchBlock(float coord_x, float coord_y, int &row, int &col);
	void SplitBlock();
	bool CheckCache(int region_code);
	bool CreateProcessData();
	bool ExtractDEM(Region *r);
	bool ExtractTexture(Region *r);
	bool ScaleData();
	bool TransferData(bool nullData);

  //inline void Test();
  
public:
	/* Member variables */
  vector<Region>		m_partition;		   ///< divided region
	SlaveCacheData		*m_data;			     ///< Cache data
	SlaveCacheData		m_processedData;	 ///< Processed Data
	DiskCache			    m_diskCache;		   ///< Disk cache
	SlaveMemCache		  m_memCache;			   ///< Memory cache
	double				    m_elapsedTime;		 ///< elapsed time
};


#endif // WMPI_H