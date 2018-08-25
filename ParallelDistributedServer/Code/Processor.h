#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <deque>
#include "Thread.h"
#include "Mutex.h"
#include "Transaction.h"

using std::deque;

/*!
  \brief
    a class for processor
*/
class Processor : public Thread
{
public:
  /* Constructor & Destructor */
  Processor();
  virtual ~Processor();
  
  /* Member functions */
  void SetCloseTransaction(Transaction *closeTransaction);
  void AddTransaction(Transaction *transaction);
  void Stop();
  void Clear();
  
  virtual void Run();
  
protected:
  Transaction *Take();
  bool IsEmpty();
  
protected:
  /* Member variables */
  bool                 m_bRunning;          ///< whether it is executing it or not
  Mutex                m_mutex;             ///< synchronization object
  deque<Transaction *> m_jobQueue;          ///< job queue
  Transaction          *m_closeTransaction; ///< transaction for ending
  
};

#endif // PROCESSOR_H