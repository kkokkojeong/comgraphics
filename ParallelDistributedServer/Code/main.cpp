/*!
	\brief
		Parallel Computing Application using MPICH2(message passing interface), 
		WNetworking and WinSock

	\date	2015. 04. 10
*/

///< Include header files
#include <iostream>
#include "WSock.h"
#include "Processor.h"

#include "Transaction.h"

#define INFINITE            0xFFFFFFFF

///< Using namespace
using namespace std;

/*------------------------------------------------------------------------------
		Data Type Definition
------------------------------------------------------------------------------*/
struct ThreadParam {
  int    sock;        ///< Linux socket
  Event  event;       ///< Waiting for parallel computation
};

/*------------------------------------------------------------------------------
		Global Variables
------------------------------------------------------------------------------*/
MasterNode		*g_pMasterNode = NULL;			///< master node
SlaveNode		  *g_pSlaveNode  = NULL;	  	///< slave node
Processor     *g_processor   = NULL;      ///< processor

/*------------------------------------------------------------------------------
		Prototype for Routines
------------------------------------------------------------------------------*/
void InitApp(int &argc, char **&argv);	       ///< Initialize application
void CloseApp();							                 ///< Close application
void StoreTransaction( Transaction *t );       ///< Store transaction

/*------------------------------------------------------------------------------
		Prototype for Routines using MPI
------------------------------------------------------------------------------*/
void ControlMPI();							               ///< Control MPI parts

/*------------------------------------------------------------------------------
		Prototype for Routines using TCP/IP 
------------------------------------------------------------------------------*/
void ControlSocket();							             ///< Control WinSock part
void *ServerMain(void *param);                 ///< Accept client
void *ProcessClient(void *param);              ///< Create threads for communication
void *RecvThread(void *param);                 ///< Receive thread
void *SendThread(void *param);                 ///< Send thread
int SendDEM(int sock, Result *data);           ///< Send DEM data
int SendTexture(int sock, Result *data);       ///< Send Texture


/*------------------------------------------------------------------------------
		Implement for Parallel Computing using Multi-thread
------------------------------------------------------------------------------*/
/*!
  \brief
    main function
*/
int main(int argc, char *argv[])
{
	///< Initialize application
	InitApp( argc, argv );

	///< Control MPI(Message Passing Interface)
	ControlMPI();

	///< Control TCP/IP, master to node
	if(MPI::COMM_WORLD.Get_rank() == MASTER) {
		ControlSocket();
	}
  
  ///< Wait for signal
  pause();
  
	///< Close application
	CloseApp();
 
	return 0;
}

/*!
  \brief
    Initialize application
*/
void InitApp(int &argc, char **&argv)
{
  ///< Initialize MPI
  MPI::Init( argc, argv );
  //pthread_mutex_init( &gMutex, NULL ); 
}

/*!
  \brief
    Close application
*/
void CloseApp()
{
  if( g_processor!= NULL )
  {
    g_processor->SetCloseTransaction( new CloseTransaction(g_pMasterNode, NULL) );
    g_processor->Stop();
    g_processor->Join();
  }
  
  ///< Delete global variables
  if( g_pMasterNode != NULL )  delete g_pMasterNode;
  if( g_pSlaveNode != NULL )   delete g_pSlaveNode;
  if( g_processor != NULL )    delete g_processor;
  
  ///< Finalize MPI
  MPI::Finalize();
}

/*!
  \brief
    Store transaction
*/
void StoreTransaction( Transaction *t )
{
  if( t != NULL )
    g_processor->AddTransaction( t );
}


/*------------------------------------------------------------------------------

								MPI PART

------------------------------------------------------------------------------*/
/*!
  \brief
    Control MPI Part
*/
void ControlMPI()
{
  ///< Take on master
  if( MPI::COMM_WORLD.Get_rank() == MASTER )
  {
    ///< Create master node
    g_pMasterNode = new MasterNode;
    
    if( g_pMasterNode == NULL )
    {
      cerr << "Create master failed." << endl;
      CloseApp();
      exit(-1);
    }
    
    ///< Create processor for computation in parallel
    g_processor = new Processor;
    
    if( g_processor == NULL )
    {
      cerr << "Create processor failed." << endl;
      CloseApp();
      exit(-1);
    }
    
    g_processor->Start();
  }
  
  ///< Take on slave
  else
  {
     g_pSlaveNode = new SlaveNode;
     
     if( g_pSlaveNode == NULL )
     {
       cerr << "Create slave failed." << endl;
       CloseApp();
       exit(-1);
     }
     
     g_pSlaveNode->Run();
  } 
}

/*------------------------------------------------------------------------------

								WinSock PART

------------------------------------------------------------------------------*/
/*!
	\brief
		Control WinSock part
*/
void ControlSocket()
{
 	///< Create communication server thread
	int threadid;
  pthread_t tid;
  
  threadid = pthread_create( &tid, NULL, ServerMain, NULL );
  if( threadid < 0 ) {
    cerr << "[TCP Control] Create thread failed." << endl;
  }
}

/*!
	\brief
		Take on communication server
		Note : This is thread function.
*/
void *ServerMain(void *param)
{
  cout << "[TCP Control] Started!" << endl;

	WTCPSockServer winsock;

	winsock.Bind();					///< Bind
	winsock.Listen();				///< Listen
	winsock.ReuseAddre();

  while( true ) 
  {
    ///< Accept
		winsock.Accept();
   
		if( winsock.GetClientSocket() == -1 ) {
			cout << "Invalid Socket" << endl;
      continue;
    }
		cout << "[TCP Control] Client connected : IP = " <<
          inet_ntoa(winsock.GetClientAddr()->sin_addr) << ", PORT = " << ntohs(winsock.GetClientAddr()->sin_port) << endl;
          
    ///< Create thread for communication
    int iThreadID;
    pthread_t tid;
    int sockID = winsock.GetClientSocket();
    
    iThreadID = pthread_create( &tid, NULL, ProcessClient, (void *)&sockID );
    if( iThreadID < 0 ) {
      cerr << "[TCP Control] Create thread failed." << endl;
    }
    else {
      close( tid );
      //pthread_detach( tid );
    }
	}
 
  cout << "[TCP Control] Terminated!"<< endl;
  return NULL;
}

/*!
	\brief
		Take on communication with one client
		Note : This is thread function.
*/
void *ProcessClient(void *param)
{ 
  //cout << "Process Client" << endl;
  int client_sock = *((int *)param);
  struct sockaddr_in clientAddr;
  socklen_t addrLen; 
  
  ///< Getting client information
  addrLen = sizeof(clientAddr);
  getpeername( client_sock, (struct sockaddr *)&clientAddr, &addrLen );
  
  int iThread[2];              // thread
  pthread_t pThreadID[2];      // thread id
  
  ThreadParam *threadParam = new ThreadParam;
  threadParam->sock = client_sock;
  
  ///< Create read thread
  iThread[0] = pthread_create( &pThreadID[0], NULL, RecvThread, (void *)threadParam );
  
  ///> Create write thread
  iThread[1] = pthread_create( &pThreadID[1], NULL, SendThread, (void *)threadParam );
  
  if( iThread[0] < 0 || iThread[1] < 0 ) {
    cerr << "[TCP Control] CreateThread failed : " << errno << endl;
    close( client_sock );
    return NULL;
  }  
  
  ///< Waiting
 // pthread_join( iThread[0], NULL );
 // pthread_join( iThread[1], NULL );
  
 // if( WAIT_OBJECT_0 )s
 //   pthread_cancel( iThread[0] );
 // else if( WAIT_OBJECT_0 + 1 )
 //   pthread_cancel( iThread[1] );
  
 // close( iThread[0] );
 // close( iThread[1] );
 // close( client_sock );
  
//  cout << "[TCP Control] Client disconnected: IP = " << inet_ntoa(clientAddr.sin_addr) << ", PORT = " << ntohs(clientAddr.sin_port) << endl;
  
  ///< delele memory
   
  return NULL;
}

/*!
  \brief
    Receive thread
    Note : This is thread function that is created each client
           This should be synchronized with write thread
*/
void *RecvThread(void *param)
{
  //cout << "recv thread" << endl;
  ThreadParam *arg = (ThreadParam *)param;
  int message_type;                         ///< message type
  int retval;                               ///< return value
  
  struct sockaddr_in clientAddr;            ///< address of client
  socklen_t addrLen;
  
  ///< Getting client information
  addrLen = sizeof(clientAddr);
  getpeername( arg->sock, (struct sockaddr *)&clientAddr, &addrLen );
  
  while( true ) {
    ///< Receive message type
    retval = Recvn( arg->sock, &message_type, 4, 0 );
    //cout << message_type << endl;
    
    if( retval == 0 || retval == INVALID_SOCKET ) {
      cout << "[TCP Control] ";
      DisplayError( "recv()", false );
      break;
    }
    
    ///< Receive extent of region of interest
    if( message_type == MESSAGE_REQUEST_ROI ) {
      Region roi;                    ///< region
      float interval;                ///< interval
      unsigned int magnification;    ///< magnification of texture
      
      retval = Recvn( arg->sock, &interval, sizeof(interval), 0 );
      if( retval == 0 || retval == INVALID_SOCKET ) {
        cout << "[TCP Control] ";
        DisplayError( "recv()", false );
        break;
      }
      
      retval = Recvn( arg->sock, &magnification, sizeof(magnification), 0 );
      if( retval == 0 || retval == INVALID_SOCKET ) {
        cout << "[TCP Control] ";
        DisplayError( "recv()", false );
        break;
      }
      
      unsigned int region[5];
      retval = Recvn( arg->sock, &(region[0]), sizeof(unsigned int), 0 );
			retval = Recvn( arg->sock, &(region[1]), sizeof(unsigned int), 0 );
			retval = Recvn( arg->sock, &(region[2]), sizeof(unsigned int), 0 );
			retval = Recvn( arg->sock, &(region[3]), sizeof(unsigned int), 0 );
			retval = Recvn( arg->sock, &(region[4]), sizeof(unsigned int), 0 );

      roi.code =   (int)region[0];
			roi.lo_x = (float)region[1];
			roi.lo_y = (float)region[2];
			roi.hi_x = (float)region[3];
			roi.hi_y = (float)region[4];  
      
      if( retval == 0 || retval == INVALID_SOCKET )
			{
				cout << "[TCP Control] ";
				DisplayError("recv()", false);
				break;
			}
      //cout << roi.code << "," << roi.lo_x  << "," << roi.lo_y << "," << roi.hi_x << "," << roi.hi_y << endl;
      StoreTransaction( new ExtractTransaction(g_pMasterNode, &(arg->event), roi, interval, magnification) );
    }
  }
  // tip : man thread_self() == getcurrentthreadid()
  cout << "[TCP Control] Recv Thread "<< pthread_self() << " is terminated." << endl;
  
  return NULL;
}

/*!
  \brief
    Send thread
    Note : This is thread function that is created each client
           This should be synchronized with read thread
*/
void *SendThread(void *param)
{
  //cout << "send thread" << endl;
  ThreadParam *arg = (ThreadParam *)param;  ///< arguments
  int         message_type;                 ///< message type
  int         retval;                       ///< return value
  bool		    bWaitResult = false;			    ///< waiting result
  int			    total_bytes = 0;
  
  struct sockaddr_in   clientAddr;          ///< address of client
  socklen_t            addrLen; 
  
  ///< Getting client information
  addrLen = sizeof(clientAddr);
  getpeername( arg->sock, (struct sockaddr *)&clientAddr, &addrLen );
  
  while( true )
  {
    ///> Wait
    bWaitResult = arg->event.Wait();
    
    if( bWaitResult == true )
    {
        Result *result = g_pMasterNode->GetResult();
        
        if( result == NULL )
        {
          cerr << "[TCP Control] Not exist work result." << endl;
          return NULL;
        }
        
        double start = MPI::Wtime();
        
        ///< Send DEM data
        retval = SendDEM( arg->sock, result );
        
        if( retval == 0 )
        {
          cerr << "[TCP Control] Error occurred during sending DEM." << endl;
          break;
        }
        
        total_bytes += retval;
        
        ///< Send texture
        retval = SendTexture( arg->sock, result );
        
        if( retval == 0 )
        {
          cerr << "[TCP Control] Error occurred during sending texture." << endl;
          break;
        }
        
        total_bytes += retval;
        
        double end = MPI::Wtime();
        
        ///< Release memory
        delete result->dem;
        delete result;
        
        cout << "[TCP Control] Thread " << pthread_self() << " transfer "<< total_bytes <<" bytes to ";
        cout << inet_ntoa(clientAddr.sin_addr) << "( " << end - start << " ms )." << endl;
        
    }
    //else
    //{
    //  cerr << "[TCP Control] Wait error." << endl;
    //  return NULL; 
    //}
  }
  
  cout << "[TCP Control] Send thread " << pthread_self() << "  is terminated." << endl;
  return NULL;
}

/*!
  \brief
    Send DEM data results for parallel computing to client
*/
int SendDEM( int sock, Result *data )
{
  int retv;          ///< return value
  int total_bytes = 0;
  
  retv = send( sock, &(data->bound[0]), sizeof(unsigned int), 0 );
  retv = send( sock, &(data->bound[1]), sizeof(unsigned int), 0 );
  retv = send( sock, &(data->bound[2]), sizeof(unsigned int), 0 );
  retv = send( sock, &(data->bound[3]), sizeof(unsigned int), 0 );
  
  if( retv == -1 )
  {
    cerr << "[TCP Control] Error occurred during sending bounds." << endl;
    return 0;
  }
  
  total_bytes = sizeof(float) * data->dem_size[0] * data->dem_size[1];
  
  ///< Send a file size
  retv = send( sock, &(total_bytes), sizeof(total_bytes), 0 );
  if( retv == -1 )
  {
    cerr << "Error occurred during send heights size." << endl;
    return 0;
  }
  
  ///< Send a dem data
  retv = send( sock, (data->dem), total_bytes, 0 );
  if( retv == -1 )
  {
    cerr << "Error occurred during send heights file." << endl;
    return 0;
  }
  
  return total_bytes;
}

/*!
  \brief
    Send texture results for parallel computing to client
*/
int SendTexture( int sock, Result *data )
{
  int	retv;						           ///< return value
	unsigned int total_bytes = 0;	 ///< total bytes

	ifstream in( data->tex_file_path.c_str(), ios_base::binary );

	if( in == NULL )
	{
		cerr << "Error occurred during open file." << endl;
		return 0;
	}
 
 ///< Get file size
	in.seekg( 0, ios_base::end );
	total_bytes = in.tellg();
	in.seekg( 0, ios_base::beg );
 
   ///< First. Send file size
	retv = send( sock, (char *)&total_bytes, sizeof(total_bytes), 0 );
	if(retv == -1)
	{
		cerr << "Error occurred during send image size." << endl;
		return 0;
	}
 
 ///< Second. Send data
	unsigned char *buf = new unsigned char[total_bytes];

	in.read( (char *)buf, total_bytes );

	retv = send( sock, (char *)buf, total_bytes, 0 );

	if( retv == -1 )
	{
		cerr << "Error occurred during send texture file." << endl;
		return 0;
	}

	delete [] buf;

	in.close();

	return total_bytes;
}



		
