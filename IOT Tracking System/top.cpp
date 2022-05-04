#include "top.h"

#include "bus_system/asSystemBus.cpp"
#include "memory/asMemoryTop.cpp"
#include "cpu/asCPUTop.cpp"
#include "gsm/gsmTop.cpp"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*-----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T, class U>
top<T,U>::top(sc_module_name nm)
  : sc_module(nm)
  , assysbus("IoT-System-Bus")
{
  cout << left << setw(30) << "ConstructorCPP - top:         "
       << left << setw(35) << name()
       << endl;

  // Construct the CPUs
  ascpu1    = new asCPUTop<T, U>("IoT-CPU1", 218);
  //amast2    = new asMasterxTop<T, U>("MeinMaster2", 101);

  // Construct the periperals
  asmem1    = new asMemTop<T>("IoT-Memory1", 216, "IoT-Mem1Socket1",
                              MEM0BYTES, MEM0WIDTH,
                              rwAccess_t, rwReadDelay_t, rwWriteDelay_t);
  asmem2    = new asMemTop<T>("IoT-Memory2", 214, "IoT-Mem2Socket1",
			      MEM1BYTES, MEM1WIDTH,
                              rwAccess_t, rwReadDelay_t, rwWriteDelay_t);
                              
  gsmtop = new gsmTop<unsigned int, float>("GSM-Top");
 
 
  /*---------------------------------------------------------------------------

    -- Connect blocks
  ---------------------------------------------------------------------------*/
  // Connect the bus; bind TLM2 initiators to TLM2 target sockets on SimpleBus
  // CPU master socket to bus slave socket
  ascpu1->mbpi->asMaster1Socket(assysbus.slaveSocket[0]);
 
  // Bus master socket to memory slave socket
  assysbus.masterSocket[0](asmem1->mbpi->asSlave1Socket);
  assysbus.masterSocket[1](asmem2->mbpi->asSlave1Socket);

}
