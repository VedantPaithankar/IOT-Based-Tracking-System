#ifndef _ASCPUBPI_H_
#define _ASCPUBPI_H_

#include "../asProcPack.h"
#include "systemc.h"
#include <iomanip>

using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
//#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"

//SC_MODULE(asCPUBPI)
template<class T>
class asCPUBPI : public sc_module
{
  public:
    /*-------------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/
    // cmd_i: indicates the read/write direction; 0 for a read, 1 for a write,
    // 2 for ign.; write it to the payload. Connected to the functional block.
    sc_export<sc_signal_in_if<unsigned int> >  cmd_i;
    // address_i: is the memory or register address; write it to the payload.
    // Connected to the functional block.
    sc_export<sc_signal_in_if<unsigned int> >  address_i;
    // data_i: is the data in case of a write; write it to the payload.
    // Connected to the functional block.
    sc_export<sc_signal_in_if<T> >  data_i;
    // data_o: is the data in case of a read; read from the payload.
    sc_out<T>                       data_o;
    /*-------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    //SC_CTOR(asSlaveBPI);
    unsigned int ID_r; // ID register
    // event, that indicates that the read data is ready; notified here,
    // read in the functional block
    sc_event& mBPIDatRdEv_e;
    asCPUBPI(sc_module_name   nm                 //!< SC module name
        , const unsigned int      ID             //!< target ID
	, sc_event&               mBPIDatRdEv
      );
    /*-------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    // Socket of this module
    tlm_utils::simple_initiator_socket<asCPUBPI> asMaster1Socket;
    // data register
    unsigned int data_r;
    //sc_export connected signals
    sc_signal<unsigned int> cmd_s;
    sc_signal<unsigned int> address_s;
    sc_signal<T> dataWr_s;
    // general signals
    /*-------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
  private:
    /*-------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    /*-------------------------------------------------------------------------
    -- Events
    -------------------------------------------------------------------------*/
    // indicates, that there is a new action on the bus
    sc_event cmd_e;
    /*-------------------------------------------------------------------------
    -- Methods/Threads
    -------------------------------------------------------------------------*/
    //virtual void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay);
    void cmdThread(void);
    void initThread (void);
    void backChannelProc(tlm::tlm_generic_payload &pl);
    void cpuBPIPrint01(void);
    void cpuBPIPrint02(void);
    void cpuBPIPrint03(unsigned int addr, T data, tlm::tlm_command cmd);
    void cpuBPIPrint04(void);
    void cpuBPIPrint05(T data);
    void cpuBPIPrint06(unsigned int cmd);
    /*-------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    //const unsigned int asID;                   //!< target ID
};
#endif
