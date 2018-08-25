

########################################################################
#                Parallel Computing Makefile                           #
########################################################################

# Compile wrapper
CXX = mpicxx

# Flag option
OPTFLAGS = -O3
CXXFLAGS = $(OPTFLAGS) -g

# Program name
all : TestProgram

# file that includes the main function
TestProgram : main.cpp

  # compile, flag -o / outputfilename, sourcefile 1, 2 ....  / libraries
#	$(CXX) $(CXXFLAGS) -o TestProgram main.cpp WMPI.cpp WMPI.h WSock.cpp WSock.h -lpthread
	$(CXX) $(CXXFLAGS) -I/usr/local/opencv/include/opencv -L/usr/local/opencv/lib  -o ParallelTest main.cpp WMPI.cpp WMPI.h Topography.h Topography.cpp Cache.h Cache.cpp Event.h Mutex.h Processor.h Processor.cpp Thread.h Transaction.h  WSock.h WSock.cpp -lcv -lcxcore -lhighgui -lpthread

#clean :
#	rm TestProgram*
