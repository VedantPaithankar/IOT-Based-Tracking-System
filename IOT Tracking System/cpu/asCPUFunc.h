#ifndef _ASCPUFUNC_H_
#define _ASCPUFUNC_H_

#include "../asProcPack.h"
#include "systemc.h"
#include "tlm.h"
#include <iomanip>
#include "gsm/constants.h"

// T: data type for "data"; U: data type for GPIOs
template<class T, class U>
SC_MODULE(asCPUFunc)
{
  public:
    /*-------------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/
    // data_i: is the data in case of a read; read from the payload.
    sc_export<sc_signal_in_if<T> >  data_i;
    // cmd_o: indicates the read/write direction; 0 for a read, 1 for a write,
    // 2 for ign.; write it to the payload. Connected to the BPI.
    sc_out<unsigned int>            cmd_o;
    // address_o: is the memory or register address; write it to the payload.
    // Connected to the BPI.
    sc_out<unsigned int>            address_o;
    // data_o: is the data in case of a write; write it to the payload.
    // Connected to the BPI.
    sc_out<T>                       data_o;
    /*-------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    //sc_export connected signals
    sc_signal<T> dataIn_s;
    /*-------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    //SC_CTOR(asSlaveFunc);
    asCPUFunc(sc_module_name nm, sc_event& mFuncDatRdEv);
    /*-------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    //static const unsigned int  m_txn_data_size = 4; // transaction size
    static const unsigned int  m_txn_data_size = BYTESPERMEMLINE;
    unsigned int currentAddress_v;
	int powerData_out = INT_ZERO;
	int cellID_out = INT_ZERO;
	
    // event, that indicates that the read data is ready; read here,
    // notified in the BPI
    sc_event& mFuncDatRdEv_e;
	sc_event gsmevent;
	sc_event cpuevent;
  private:
    /*-------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    unsigned int IR; // Instruction register
    /*-------------------------------------------------------------------------
    -- Events
    -------------------------------------------------------------------------*/
    // when the information from the BPI comes, that the read data is
    // available, this event will be notified and captured by the thread which
    // requests the data.
    sc_event readMemRdy_e;
    /*-------------------------------------------------------------------------
    -- Methods/Threads
    -------------------------------------------------------------------------*/
    // ... for the bus actions
	void writeGSMData(void);
	void WriteMessage(int, int);
    void getDatThread(void);
    void cpuSequenceThread(void); // This is the algorithm
    void cpuWr1Print(unsigned int addr, T data, bool typecast);
    void cpuWrCmd(unsigned int addr,  T data);
    T cpuRdCmd(unsigned int addr);
    void cpuRd1Print(unsigned int addr, T data, bool typecast);
    void cpuFuncPrint01(T data);
    /*-------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
};
#endif
