

#include <tchar.h>
#include <iostream>

using namespace std;

#define LOCALNAME "/home/seokcheol/"

class WNetwk
{
public:
  /* Constructor & Destructor */
  WNetwk();
  virtual ~WNetwk();
  
  ///> Prevent malfunction
  WNetwk( const WNetwk & );
  WNetwk &operator=( const WNetwk & );
  
};