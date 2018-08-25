

#include "Event.h"

Event event;

void *ServerMain(void *param)
{
  cout << "thread0 start" << endl;
  sleep(5);
  event.Set();
  cout << "thread0 end" << endl;
  
  return NULL;
}

void *ServerMain1(void *param)
{
  cout << "thread1 start " << endl;
  bool bWaited = event.Wait();
  cout << "thread1 end" << endl;
  
  
  cout << (int)bWaited << endl;
  return NULL;
}


int main(int argc, char *argv[])
{
  
  
 // event.Set();
 // event.Reset();
  
  int threadid[2];
  pthread_t tid[2];
  
  
  threadid[0] = pthread_create( &tid[0], NULL, ServerMain, NULL );
  threadid[1] = pthread_create( &tid[1], NULL, ServerMain1, NULL );
  
  
  
  pthread_join( tid[0], NULL );
  pthread_join( tid[1], NULL );
  
  return 0;
}