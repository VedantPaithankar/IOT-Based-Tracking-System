#include "asCPUTop.h"
#include "asCPUBPI.cpp"
#include "asCPUFunc.cpp"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*-----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T, class U>
asCPUTop<T,U>::asCPUTop(sc_module_name nm
, const unsigned int   ID                   // target ID
)
  : sc_module(nm)
  , asID                 (ID)               // init target ID
{
  //SC_HAS_PROCESS(asCPUTop);
  cout << left << setw(30) << "ConstructorCPP - asCPUTop:    "
       << left << setw(36) << name()
       << left << setw(14) << " with the ID: "
       << left << setw(5)  << dec << asID
       << endl;

  mbpi = new asCPUBPI<T>("IoT-CPU1-BPI", asID, masterDataReadEvent_e);
  mcpu = new asCPUFunc<T,U>("IoT-CPU1-Func", masterDataReadEvent_e);

  // sc_export binding
  mcpu->cmd_o(mbpi->cmd_s);
  mcpu->address_o(mbpi->address_s);
  mcpu->data_o(mbpi->dataWr_s);
  mbpi->data_o(mcpu->dataIn_s);

}
