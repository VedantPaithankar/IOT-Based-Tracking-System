#include "asMemoryTop.h"
#include "asMemoryBPI.cpp"
#include "asMemoryFunc.cpp"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*-----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T>
asMemTop<T>::asMemTop(sc_module_name nm
, const unsigned int   ID                   // target ID
, const char           *memorySocket        // socket name
, uint64               memorySize           // memory size (bytes)
, unsigned int         memoryWidth          // memory width (bytes)
, const sc_time        acceptDelay          // accept delay
, const sc_time        readResponseDelay    // read response delay
, const sc_time        writeResponseDelay   // write response delay
)
  : sc_module(nm)
, asID                 (ID)                 // init target ID
, asMemorySize         (memorySize)         // e.g., 1024 lines = 4*1024
, asMemoryWidth        (memoryWidth)        // e.g., 4 bytes per line
, asAcceptDelay        (acceptDelay)        // init accept delay
, asReadResponseDelay  (readResponseDelay)  // init read response delay
, asWriteResponseDelay (writeResponseDelay) // init write response delay 
{
  //SC_HAS_PROCESS(asMemTop);
  cout << left  << setw(30) << "ConstructorCPP - asMemTop:    "
       << left  << setw(36) << name()
       << left  << setw(15) << " with the ID: "
       << right << setw(5)  << dec << asID
       << left  << setw(8)  << ", Size: "
       << right << setw(5)  << dec << asMemorySize
       << left  << setw(15) << " bytes, Width: "
       << right << setw(5)  << dec << asMemoryWidth
       << " bytes"
       << endl;

  // generate the sub-blocks
  mbpi = new asMemBPI<T>("IoT-Memory-BPI",asID,
			  memorySocket,asMemorySize,
			  asMemoryWidth,
			  asAcceptDelay,asReadResponseDelay,
			  asWriteResponseDelay,slaveDataReadEvent_e);
  mfun = new asMemFunc<T>("IoT-Memory-Func",asMemorySize,slaveDataReadEvent_e);

  // sc_export, port binding
  mbpi->cmd_o(mfun->cmd_s);
  mbpi->address_o(mfun->address_s);
  mbpi->data_o(mfun->dataIn_s);
  mfun->data_o(mbpi->dataRd_s);
}
