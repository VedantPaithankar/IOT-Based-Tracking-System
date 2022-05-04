#ifndef _ASMEMORYBPI_H_
#define _ASMEMORYBPI_H_

#include "../asProcPack.h"
#include "systemc.h"
#include <iomanip>

using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
//#include "tlm_utils/simple_initiator_socket.h"

template<class T>
//SC_MODULE(asSlaveBPI)
class asMemBPI : public sc_module
{
  public:
    /*-------------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/
    // cmd_o: indicates the read/write direction; 0 for a read, 1 for a write,
    // 2 for ignore; read from the payload. Connected to the functional block.
    sc_out<unsigned int>            cmd_o;
    // address_o: is the memory or register address; read from the payload.
    // Connected to the functional block.
    sc_out<unsigned int>            address_o;
    // data_o: is the data in case of a write; read from the payload.
    // Connected to the functional block.
    sc_out<T>                       data_o;
    // data_i: is the data in case of a read; written to the payload.
    // Connected to the functional block.
    sc_export<sc_signal_in_if<T> >  data_i;
    /*-------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    //SC_CTOR(asMemBPI);
    asMemBPI(sc_module_name   nm                 //!< SC module name
      , const unsigned int      ID                 //!< target ID
      , const char              *memorySocket      //!< socket name
      , uint64                  memorySize         //!< memory size (bytes)
      , unsigned int            memoryWidth        //!< memory width(bytes)
      , const sc_time           acceptDelay        //!< accept delay
      , const sc_time           readResponseDelay  //!< read response delay
      , const sc_time           writeResponseDelay //!< write response delay
      , sc_event&               sBPIDatRdEv        //!< event, that indicates
	                                           //!< that the read data is
                                                   //!< ready to be transmitted
                                                   //!< to the CPU
      );
    /*-------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    //sc_export connected signals
    sc_signal<T>            dataRd_s;
    // general signals
    sc_signal<unsigned int> cmd_s;
    sc_signal<unsigned int> adr_s;
    sc_signal<T>            daw_s;
    /*-------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    tlm_utils::simple_target_socket<asMemBPI> asSlave1Socket;//!< target socket
  private:
    /*-------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    /*-------------------------------------------------------------------------
    -- Events
    -------------------------------------------------------------------------*/
    sc_event dataRd_e;
    /*-------------------------------------------------------------------------
    -- Methods/Threads
    -------------------------------------------------------------------------*/
    virtual void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay);
    void cmdThread(void);
    void adrThread(void);
    void dawThread(void);
    void darThread(void);
    void memBPIPrint01(unsigned int addr);
    void memBPIPrint02(unsigned int addr, T data);
    void memBPIPrint03(unsigned int addr, T data);
    void memBPIPrint04(unsigned int addr, T data, tlm::tlm_command cmd);
    void memBPIPrint05(T data);
    /*-------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    const unsigned int asID;                   //!< target ID
    uint64             asMemorySize;           //!< memory size (bytes)
    unsigned int       asMemoryWidth;          //!< word size (bytes)
    const sc_time      asAcceptDelay;          //!< accept delay
    const sc_time      asReadResponseDelay;    //!< read response delay
    const sc_time      asWriteResponseDelay;   //!< write response delays
  public:
    sc_event& sBPIDatRdEv_e;
};
#endif
