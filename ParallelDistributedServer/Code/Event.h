#ifndef EVENT_H
#define EVENT_H


#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

using namespace std;

class Event
{
public:
  /* Constructor */
  Event( bool bManulReset = false )
  {
    memset( &m_pThread, 0, sizeof(pthread_t) );
    
    pthread_mutexattr_t mAttr;
    pthread_mutexattr_init( &mAttr );
    pthread_mutex_init( &m_pMutex, &mAttr );
    pthread_cond_init( &m_pCondition, NULL );
  }
  
  /* Destructor */
  virtual ~Event()
  {
    pthread_cond_destroy( &m_pCondition );
	  pthread_mutex_destroy( &m_pMutex );
  }
  
  /*!
    \brief
      Set an event to signaled
  */
  void Set()
  {
    //if( m_pCondition > 0 )
    //{
      pthread_mutex_lock( &m_pMutex );
      pthread_cond_signal( &m_pCondition );
      pthread_mutex_unlock( &m_pMutex );
    //}
  }
  
  /*!
    \brief
      Reset an event flag to unsignaled
  */
  void Reset()
  {
    //if( m_pCondition != NULL )
    //{
      memset( &m_pThread, 0, sizeof(pthread_t) );
      pthread_mutex_unlock( &m_pMutex );
    //}
  
  }
  
  /*!
    \brief
      Wait for an event - wait for an event object to be set to signaled
  */
  bool Wait()
  {
    //if( m_pCondition != NULL )
    //{
      pthread_mutex_lock( &m_pMutex );
		  pthread_cond_wait( &m_pCondition, &m_pMutex);
      pthread_mutex_unlock( &m_pMutex );
      
      return true;
    //}
  }
  
  /* Get */
  //pthread_t GetHandle()     { return m_pThread;  }

protected:
  pthread_mutex_t  m_pMutex;
  pthread_cond_t   m_pCondition;
  pthread_t        m_pThread;
  bool             m_bSignal;
};

#endif // EVENT_H
