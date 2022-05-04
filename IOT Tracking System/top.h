#ifndef _TOP_H_
#define _TOP_H_

#include <systemc.h>
#include "tlm.h"
#include "asProcPack.h"

#include "bus_system/asSystemBus.h"
#include "memory/asMemoryTop.h"
#include "cpu/asCPUTop.h"
#include "gsm/gsmTop.h"

template<class T, class U>
SC_MODULE(top)
{
  public:
 
	SC_CTOR(top);
 
    //SimpleBus<#Master, #Slave>
    asSystemBus<1, 2>   assysbus;
    asMemTop<T>*        asmem1;
    asMemTop<T>*        asmem2;
    asCPUTop<T, U>*     ascpu1; 
    gsmTop<unsigned int, float>* gsmtop;	
  
  private:
 
};
#endif
