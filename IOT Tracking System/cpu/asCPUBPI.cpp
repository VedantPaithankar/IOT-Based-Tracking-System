#include "asCPUBPI.h"

/*-----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T>
asCPUBPI<T>::asCPUBPI(sc_module_name nm
, const unsigned int   ID                   // target ID
, sc_event& mBPIDatRdEv
)
: sc_module            (nm)                 // init module name
, ID_r                 (ID)                 // ID register
, mBPIDatRdEv_e                (mBPIDatRdEv)
, asMaster1Socket("IoT-Master1-Socket")
{
  SC_HAS_PROCESS(asCPUBPI);
  cout << left << setw(30) << "ConstructorCPP - asCPUBPI:    "
       << left << setw(36) << name()
       << left << setw(14) << " with the ID: "
       << left << setw(5)  << dec << ID_r
       << endl;
  SC_THREAD(initThread);
  SC_THREAD(cmdThread);
  
  // binds the fifo to the export
  data_i.bind(dataWr_s);
  cmd_i.bind(cmd_s);
  address_i.bind(address_s);
}
/*-----------------------------------------------------------------------------
-- Methods
-----------------------------------------------------------------------------*/
template<class T>
void asCPUBPI<T>::initThread(void)
{
  tlm::tlm_generic_payload *trans_p = new tlm::tlm_generic_payload;
  //sc_time delay = sc_time(6, SC_NS);
  sc_time delay = sc_time(SC_ZERO_TIME);
  while(true)
  {
    // wait on changes on the bus ...
    if(MASTERBPIPRINTS)
      cpuBPIPrint01();

    // ... here is the wait
    wait(cmd_e);
    
    if(MASTERBPIPRINTS)
      cpuBPIPrint02();

    // read cmd from export
    // if write: read data from export and write it to the payload
    // if read:  read data from bus - write to port (to functional block)
    if(cmd_s.read() == 1) // write; cmd=1
    {
      data_r = dataWr_s.read();
    }
    tlm::tlm_command cmd = static_cast<tlm::tlm_command>(cmd_s.read());
    trans_p->set_command(cmd);
    trans_p->set_address(address_s.read());
    trans_p->set_data_ptr(reinterpret_cast<unsigned char*>(&data_r));
    trans_p->set_data_length(BYTESPERMEMLINE);
    // = data_length => no streaming
    trans_p->set_streaming_width(BYTESPERMEMLINE);
    trans_p->set_byte_enable_ptr(0); // 0 indicates unused
    trans_p->set_dmi_allowed(false); // Mandatory initial value
    trans_p->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    // send the payload on its journey
    asMaster1Socket->b_transport(*trans_p, delay); // Blocking transport call

    if (trans_p->get_command() == tlm::TLM_READ_COMMAND) // read; cmd=0
    {
      // write the read data to the functional block
      data_o->write(data_r);
      // notify the functional block, that new data is available
      mBPIDatRdEv_e.notify(SC_ZERO_TIME);
    }
    
    // Check back channel
    // Initiator obliged to check response status and delay
    backChannelProc(*trans_p);

    if(MASTERBPIPRINTS)
      cpuBPIPrint03(address_s.read(), data_r, cmd);
    
    wait(delay);
    
    if(MASTERBPIPRINTS)
      cpuBPIPrint04();
      
  }//while
}

template<class T>
void asCPUBPI<T>::backChannelProc(tlm::tlm_generic_payload &pl)
{
  if(pl.is_response_error())
  {
    char txt[100];
    sprintf(txt, "Master-BPI - Error from b_transport, response status = %s",
            pl.get_response_string().c_str());
    SC_REPORT_ERROR("TLM-2", txt);
  }
  else
  {
    // no error
    if(MASTERBPIPRINTS)
      cpuBPIPrint05(*reinterpret_cast<T*>(pl.get_data_ptr()));
  }
}

// Cmd, generated in MasterFunc, will be got here. For bus action.
template<class T>
void asCPUBPI<T>::cmdThread(void)
{
  while(true)
  {
    // wait, until something happens on the bus ...
    wait(cmd_s.value_changed_event());
    
    if(MASTERBPIPRINTS)
      cpuBPIPrint06(cmd_s.read());

    // notify, that something happens on the bus; the init_thread waits for it
    cmd_e.notify(SC_ZERO_TIME);
  }
}

/*-----------------------------------------------------------------------------
-- Not needed things, just printouts
-----------------------------------------------------------------------------*/
template<class T>
void asCPUBPI<T>::cpuBPIPrint01(void)
{
  cout << left  << setw(11) << "Master-BPI "
       << left  << setw(20) << "- Waiting ..... "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << endl;
}

template<class T>
void asCPUBPI<T>::cpuBPIPrint02(void)
{
  cout << left  << setw(11) << "Master-BPI "
       << left  << setw(20) << "- Waiting ... end "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << endl;
}

template<class T>
void asCPUBPI<T>::cpuBPIPrint03(unsigned int addr,T data,tlm::tlm_command cmd)
{
  cout << left  << setw(11) << "Master-BPI "
       << left  << setw(20) << "- Op.: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(10) << ", byteAdr."
       << right << hex << setw(9) << addr
       << left  << setw(7)  << ", data "
       << right << hex << setw(9) << data
       << left  << setw(7)  << ", cmd "
       << dec   << setw(4)  << (cmd ? 'W' : 'R')
       << endl;
}

template<class T>
void asCPUBPI<T>::cpuBPIPrint04(void)
{
  cout << left  << setw(11) << "Master-BPI "
       << left  << setw(20) << "- Delay end ... "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << endl;
}

template<class T>
void asCPUBPI<T>::cpuBPIPrint05(T data)
{
  cout << left  << setw(11) << "Master-BPI "
       << left  << setw(20) << "- BackChannel: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(19) << ", NO ERROR "
       << left  << setw(7)  << ", data "
       << right << hex << setw(9) << data
       << endl;
}

template<class T>
void asCPUBPI<T>::cpuBPIPrint06(unsigned int cmd)
{
  cout << left  << setw(11) << "Master-BPI "
       << left  << setw(20) << "- Cmd change: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(35) << ""
       << left  << setw(7)  << ", cmd "
       << dec   << setw(4)  << cmd
       << endl;
}
