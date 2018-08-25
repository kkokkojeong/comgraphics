
#include "Processor.h"

/*!
  \brief
    Constructor
*/
Processor::Processor()
: m_bRunning(false), m_closeTransaction(0)
{
}

/*!
  \brief
    Destructor
*/
Processor::~Processor()
{
  Clear();
}

/*!
  biref
    Set close transaction that should be executed when application closed
*/
void Processor::SetCloseTransaction(Transaction *closeTransaction)
{
  m_closeTransaction = closeTransaction;
}

/*!
  \brief
    Add transaction
*/
void Processor::AddTransaction(Transaction *transaction)
{
  m_mutex.Lock();
  m_jobQueue.push_back( transaction );
  m_mutex.UnLock();
  Resume();
}

/*!
  \brief
    Stop processor
*/
void Processor::Stop()
{
  m_mutex.Lock();
  m_bRunning = false;
  m_mutex.UnLock();
  Resume();
}

/*!
  \brief
    Clear every transaction in queue
*/
void Processor::Clear()
{
  m_mutex.Lock();
  
  deque<Transaction *>::iterator it = m_jobQueue.begin();
  while( it != m_jobQueue.end() )
  {
    delete (*it);
    ++it;
  }
  m_jobQueue.clear();
  
  m_mutex.UnLock();
}

/*!
  \brief
    Running with executing a transaction in queue sequentially
*/
void Processor::Run()
{
  m_bRunning = true;
  
  while( m_bRunning )
  {
  //?
    pthread_mutex_lock(& m_pMutex);
    while(m_iFlag > 0){
      pthread_cond_wait(&m_pCond, &m_pMutex);
    }
    pthread_mutex_unlock(&m_pMutex);
    //cout << "process run" << endl;
  
    Transaction *transaction = Take();
    if( transaction != NULL )
    {
      bool complete = transaction->Execute();
      
      if( complete == false )
        AddTransaction(transaction);
      else
        delete transaction;
    }
  }
  
  Clear();
  
  if( m_closeTransaction != NULL )
  {
    m_closeTransaction->Execute();
    delete m_closeTransaction;
    m_closeTransaction = NULL;
  }
}

/*!
  \brief
    The transaction is taken out of the queue
*/
Transaction *Processor::Take()
{
  Transaction *transaction = NULL;
  
  ///< Queue stands by between emptiness
  while( true )
  {
    m_mutex.Lock();
    bool empty = m_jobQueue.empty();
    m_mutex.UnLock();
    
    if( empty == false )
      break;
      
    Suspend();
    
    if( m_bRunning == false )
      return NULL;
  }
  
  m_mutex.Lock();
  
  transaction = m_jobQueue.front();
  m_jobQueue.pop_front();
  
  m_mutex.UnLock();
  
  return transaction;
}

/*!
  \brief
    Check whether queue is empty
*/
bool Processor::IsEmpty()
{
  m_mutex.Lock();
  bool retv = m_jobQueue.empty();
  m_mutex.UnLock();
  
  return retv;
}














