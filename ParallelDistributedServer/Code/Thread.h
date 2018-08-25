
#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

enum THREAD_STATE {
        eREADY, eRUNNING, eTERMINATED, eZOMBIE, eABORTED,
};
enum THREAD_EXIT_STATE {
        eJOINABLE, eDETACHABLE,
};

/*!
  \brief
    a class for thread
*/
class Thread
{
public:
  /* Constructor & Destructor */
  Thread()
    : m_hThread(), m_bActive(false), m_iFlag(0)
  {
  }
  
  virtual ~Thread()
  {
    close( m_hThread );
  }
  
  /* Member functions */
  int Start()
  {
    pthread_create( &m_hThread, NULL, ThreadProc, reinterpret_cast<void *>(this) );
    return ( m_hThread != -1 );
  }
  
  void Join()
  {
    if( m_hThread )
    {
      pthread_join( m_hThread, NULL );
    }
  }
  
  void Sleep(int millisec)
  {
    if( m_hThread )
    {
      Sleep( millisec );
    }
  }
  
  void Resume()
	{
		if( m_hThread )
		{
			pthread_mutex_lock( &m_pMutex );
      m_iFlag--;
      pthread_cond_signal( &m_pCond );
      pthread_mutex_unlock( &m_pMutex );
		}
	}

	void Suspend()
	{
		if( m_hThread )
		{
		  pthread_mutex_lock( &m_pMutex );
      m_iFlag++;
      pthread_mutex_unlock( &m_pMutex );
		}
	}

	virtual void Run()=0;
  
protected:
  /* Thread entry */
  static void *ThreadProc(void *arg)
  {
    Thread *thread = reinterpret_cast<Thread *>(arg);
    
    if( thread != NULL )
    {
      thread->m_bActive = true;
      thread->Run();
      thread->m_bActive = false;
    }
    
    pthread_exit( (void *)NULL );
  }


  /* Member variables */
  pthread_t        m_hThread;      ///< thread handle
  pthread_mutex_t  m_pMutex;       ///< mutex
  pthread_cond_t   m_pCond;        ///< condition
  int              m_iFlag;
	bool	           m_bActive;
  
};

#endif