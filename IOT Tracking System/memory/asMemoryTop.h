#ifndef _ASMEMORYTOP_H_
#define _ASMEMORYTOP_H_

#include "../asProcPack.h"
#include <systemc.h>
#include "asMemoryBPI.h"
#include "asMemoryFunc.h"

template<class T>
SC_MODULE(asMemTop)
{
  public:
    /*-------------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/
    /*-------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    //SC_CTOR(asMemTop);
    asMemTop(sc_module_name nm
      , const unsigned int      ID                 //!< target ID
      , const char              *memorySocket      //!< socket name
      , uint64                  memorySize         //!< memory size (bytes)
      , unsigned int            memoryWidth        //!< memory width(bytes)
      , const sc_time           acceptDelay        //!< accept delay
      , const sc_time           readResponseDelay  //!< read response delay
      , const sc_time           writeResponseDelay //!< write response delay
      );
    /*-------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    // The event fired when the data to be read will be available in the
    // functional block. The BPI waits for it to forward the data to the bus.
    sc_event slaveDataReadEvent_e;
    /*-------------------------------------------------------------------------
    -- Hierarchy
    -------------------------------------------------------------------------*/
    asMemBPI<T>*      mbpi;
  private:
    /*-------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    /*-------------------------------------------------------------------------
    -- Events
    -------------------------------------------------------------------------*/
    /*-------------------------------------------------------------------------
    -- Methods/Threads
    -------------------------------------------------------------------------*/
    /*-------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    const unsigned int asID;                   //!< target ID
    uint64             asMemorySize;           //!< memory size (bytes)
    unsigned int       asMemoryWidth;          //!< word size (bytes)
    const sc_time      asAcceptDelay;          //!< accept delay
    const sc_time      asReadResponseDelay;    //!< read response delay
    const sc_time      asWriteResponseDelay;   //!< write response delays
    /*-------------------------------------------------------------------------
    -- Hierarchy
    -------------------------------------------------------------------------*/
    //dataGen              dgen;  // So ....     ; Stack allocation
    //asSlaveBPI*      mbpi;    // Heap
    asMemFunc<T>*      mfun;
};
#endif
