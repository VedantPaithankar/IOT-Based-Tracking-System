#ifndef _ASMEMORYFUNC_H_
#define _ASMEMORYFUNC_H_

#include "../asProcPack.h"
#include "systemc.h"
#include <iomanip>

template<class T> // specifies data type: unsigned int or char
//SC_MODULE(asSlaveFunc)
class asMemFunc : public sc_module
{
  public:
    /*-------------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/
    // cmd_i: indicates the read/write direction; 0 for a read, 1 for a write,
    // 2 for ignore; read from the payload. Connected to the BPI block.
    sc_export<sc_signal_in_if<unsigned int> >  cmd_i;
    // address_i: is the memory or register address; read from the payload.
    // Connected to the BPI block.
    sc_export<sc_signal_in_if<unsigned int> >  address_i;
    // data_i: is the data in case of a write; read from the payload.
    // Connected to the BPI block.
    sc_export<sc_signal_in_if<T> >  data_i;
    // data_o: is the data in case of a read; written to the payload.
    // Connected to the BPI block.
    sc_out<T>                       data_o;
    /*-------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    //sc_export connected signals
    sc_signal<unsigned int> cmd_s;
    sc_signal<unsigned int> address_s;
    sc_signal<T> dataIn_s;
    // general signals
    /*-------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    //SC_CTOR(asMemFunc);
    asMemFunc(sc_module_name nm, uint64 memorySize, sc_event& sFuncDatRdEv);
  private:
    /*-------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    //enum{SIZE = 256};
    //unsigned int mem[256];
    //unsigned int *mem = 0;
    T *mem;
    /*-------------------------------------------------------------------------
    -- Events
    -------------------------------------------------------------------------*/
    sc_event gotCmd_e;
    /*-------------------------------------------------------------------------
    -- Methods/Threads
    -------------------------------------------------------------------------*/
    void getCmdThread(void);
    void putDatThread(void);
    void wrtDatThread(void);
    void memFuncPrint01(unsigned int cmd);
    void memFuncPrint02(unsigned int addr, T data);
    void memFuncPrint03(unsigned int addr, T data);
    /*-------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    uint64  asMemorySize;           //!< memory size (bytes)
  public:
    // The event fired when the data to be read will be available in the
    // functional block. The BPI waits for it to forward the data to the bus.
    sc_event& sFuncDatRdEv_e;
};
#endif
