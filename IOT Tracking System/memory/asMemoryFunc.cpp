#include "asMemoryFunc.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*-----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T>
asMemFunc<T>::asMemFunc(sc_module_name nm
		            , uint64       memorySize
			    , sc_event&    sFuncDatRdEv)
  : sc_module(nm)
  , asMemorySize(memorySize)
  , sFuncDatRdEv_e(sFuncDatRdEv)
{
  SC_HAS_PROCESS(asMemFunc); // because no SC_CTOR()
  cout << left  << setw(30) <<  "ConstructorCPP - asMemFunc: "
       << left  << setw(56) << name()
       << left  << setw(8)  << ", Size: "
       << right << setw(5)  << dec << asMemorySize
       << endl;
  SC_THREAD(getCmdThread);
  SC_THREAD(putDatThread);
  SC_THREAD(wrtDatThread);
  
  // binds the fifo to the export
  cmd_i.bind(cmd_s);
  address_i.bind(address_s);
  data_i.bind(dataIn_s);

  // initialize memory; a little bit random
  //mem = new unsigned int [asMemorySize/4];
  mem = new T [asMemorySize/BYTESPERMEMLINE];
  for (unsigned int i = 0; i < (asMemorySize/BYTESPERMEMLINE); i++)
    mem[i] = 0xAA000000 | (rand() % 256);
}
/*-----------------------------------------------------------------------------
-- 2do:
-- - nothing
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
-- Methods
-----------------------------------------------------------------------------*/
// get the cmd from the BPI (bus to sc_export)
template<class T>
void asMemFunc<T>::getCmdThread(void)
{
  unsigned int v;
  while(true)
  {
    wait(cmd_s.value_changed_event());
    v = cmd_s.read();
    
    if(SLAVEFUNPRINTS)
      memFuncPrint01(v);

    // cmd has been changed
    gotCmd_e.notify(SC_ZERO_TIME);
  }//while
}

// write data to mem
template<class T>
void asMemFunc<T>::wrtDatThread(void)
{
  while(true)
  {
    // wait on cmd
    wait(gotCmd_e);
    // read cmd and see if it is "WRITE"
    if(cmd_s.read() == 1)
    {
      if(SLAVEFUNPRINTS)
	memFuncPrint02(address_s.read(), dataIn_s.read());

      // write the data
      mem[address_s.read()] = dataIn_s.read();
    }
  }//while
}

// read data from mem
template<class T>
void asMemFunc<T>::putDatThread(void)
{
  while(true)
  {
    // wait on cmd
    wait(gotCmd_e);
    // read cmd and see if it is "READ"
    if(cmd_s.read() == 0)
    {
      if(SLAVEFUNPRINTS)
	memFuncPrint03(address_s.read(), mem[address_s.read()]);

      // write the data, read from the memory, to the BPI
      data_o->write(mem[address_s.read()]);

      // give a notice to the BPI that the data is ready
      sFuncDatRdEv_e.notify(SC_ZERO_TIME);
    }
  }//while
}

/*-----------------------------------------------------------------------------
-- Helper functions
-----------------------------------------------------------------------------*/
// prints
template<class T>
void asMemFunc<T>::memFuncPrint01(unsigned int cmd)
{
  cout << left  << setw(11) << "Slave-Func "
       << left  << setw(20) << "- Got R/W "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(10) << ""
       << right << hex << setw(9) << ""
       << left  << setw(7) << ""
       << right << hex << setw(9) << ""
       << left  << setw(11) << ", cmd "
       << dec << setw(4) << cmd
       << endl;
}

template<class T>
void asMemFunc<T>::memFuncPrint02(unsigned int addr, T data)
{
  cout << left  << setw(11) << "Slave-Func "
       << left  << setw(20) << "- write it: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(10) << ", wordAdr."
       << right << hex << setw(9) << addr
       << left  << setw(7)  << ", data "
       << right << hex << setw(9) << data
       << endl;
}

template<class T>
void asMemFunc<T>::memFuncPrint03(unsigned int addr, T data)
{
  cout << left  << setw(11) << "Slave-Func "
       << left  << setw(20) << "- read it: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(10) << ", wordAdr."
       << right << hex << setw(9) << addr
       << left  << setw(7)  << ", data "
       << right << hex << setw(9) << data
       << endl;
}
