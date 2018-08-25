#ifndef MUTEX_H
#define MUTEX_H


#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <errno.h>

using namespace std;

/*!
  \brief
    a class for mutex
*/
class Mutex
{
public:
  /* Constructor & Destructor */
  inline Mutex()
  {
    Create();
  }
  
  virtual ~Mutex()
  {
    Lock();
    UnLock();
    
    int retv = pthread_mutex_destroy( &m_hMutex );
    if( retv )
    {
      cerr << "Failed to destory mutex : " << strerror(retv) << endl;
    }
  }
  
  ///< Prevent malfunction
  Mutex( const Mutex & );
  Mutex &operator=( const Mutex & );
  
  /* Member function - Create */
  void Create()
  {
    int retv = pthread_mutex_init( &m_hMutex, NULL );
    if( retv )
    {
      cerr << "Failed to create mutex : " << strerror(retv) << endl;
    }
  }
  
  /* Member function - Lock */
  int Lock()
  {
    int retv = pthread_mutex_lock( &m_hMutex );
    if( retv )
    {
      cerr << "Failed to lock mutex : " << strerror(retv) << endl;
    }
    return retv;
  }
  /* Member function - UnLock */
  int UnLock()
  {
    int retv = pthread_mutex_unlock( &m_hMutex );
    if( retv )
    {
      cerr << "Failed to unlock mutex : " << strerror(retv) << endl;
    }
    return retv;
  }
  
  /* Member function - IsLocked */
  bool IsLocked()
  {
    int retv = pthread_mutex_trylock( &m_hMutex );
    if( retv == EBUSY )
      return true;
    else if( retv == EINVAL )
      cerr << "Failed to try lock mutex : " << strerror(retv) << endl;
    else
      pthread_mutex_unlock( &m_hMutex );
      
    return false;
  }
  
protected:
  /* Member variables */
  pthread_mutex_t    m_hMutex;
  
};

#endif // MUTEX_H