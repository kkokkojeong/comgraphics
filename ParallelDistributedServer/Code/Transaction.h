
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "WMPI.h"
#include "Event.h"

/*------------------------------------------------------------------
	CLASS FOR TRANSACTION
--------------------------------------------------------------------*/
/*!
  \brief
    a class for unit of parallel work
*/
class Transaction
{

public:
  /* Constructor & Destructor */
  Transaction( MasterNode *p_core, Event *p_event )
    : m_core( p_core ), m_event( p_event )
  {};
  virtual ~Transaction()  
  {};
  
  /* Member functions */
  ///< Executre transaction
  virtual bool Execute() = 0;
  void NotifyCompletion()
  {
    if( m_event != NULL )
    {
      ///< Set event to signaled
      //printf("before %d\n", m_event);
      m_event->Set();
      //printf("after %d\n", m_event);
    }
  }
  
protected:
  /* Member variables */
  MasterNode *m_core;    ///< core for working
  Event      *m_event;   ///< notify a waiting thread of the occurrence of transaction complete
};

/*------------------------------------------------------------------
	CLASS FOR EXTRACTTRANSACTION
--------------------------------------------------------------------*/
/*!
  \brief
    a class for extracting region of interestd from multiple DEM data
*/
class ExtractTransaction : public Transaction
{

public:
  /* Constructor & Destructor */
  ExtractTransaction(MasterNode *p_core, Event *p_event, Region &region, float interval, int magnification)
    : Transaction( p_core, p_event ), m_region( region ), m_interval( interval ), m_magnification( magnification )
  {
  }
  
  virtual ~ExtractTransaction()
	{
	}
 
  /* Member functions */
  virtual bool Execute()
  {
    ///< Executes computation using MPI in parallel
    m_core->SetTaskInfo( &m_region, m_interval, m_magnification );
    
    m_core->Run();
    
    ///< Notify completion
    NotifyCompletion();
    
    return true;
  }

protected:
  /* Member variables */
  Region  m_region;
  float   m_interval;
  int     m_magnification;
};

/*------------------------------------------------------------------
	CLASS FOR CLOSETRANSACTION
--------------------------------------------------------------------*/
/*!
  \brief
    a class that terminates every computation node
*/
class CloseTransaction : public Transaction
{

public:
  /* Constructor & Destructor */
  CloseTransaction(MasterNode *p_core, Event *p_event)
    : Transaction( p_core, p_event )
  {
  }
  
  virtual ~CloseTransaction()
  {
  }
  
  /* Member functions */
  virtual bool Execute()
  {
    return m_core->CloseSlaves();
  }
  
};

#endif