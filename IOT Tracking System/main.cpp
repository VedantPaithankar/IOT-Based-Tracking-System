#include "systemc.h"
#include "top.cpp"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

int exit_status = 0; //global

int sc_main (int argc, char* argv[])
{
  //Direkte Instantiierung auf oberster Ebene; <Bus-Data-Type, FIFO-Type>
  top<unsigned int, char> topx("IoT-Top");

  //sc_clock clk {"clk", 1000000000, SC_NS};
  
  cout << "Simulation started: Time resolution: "
       << sc_get_time_resolution() << endl;
  //sc_set_time_resolution(1,SC_PS);


#if 0
  sc_trace_file* Tf;


  Tf = sc_create_vcd_trace_file("gsmTraces");
  Tf->set_time_unit(1, SC_NS);
  //sc_trace(Tf, clk, "Takt");
  sc_trace(Tf, topx.ascpu1->mbpi->cmd_s,     "Master-R/W");
  sc_trace(Tf, topx.ascpu1->mbpi->address_s, "Master-Address");
  sc_trace(Tf, topx.ascpu1->mbpi->dataWr_s,  "Master-Data-Write");
  sc_trace(Tf, topx.ascpu1->mbpi->data_o,    "Master-Data-Read");
  sc_trace(Tf, topx.asmem1->mbpi->cmd_s,     "Slave-R/W");
  sc_trace(Tf, topx.asmem1->mbpi->adr_s,     "Slave-Address");
  sc_trace(Tf, topx.asmem1->mbpi->daw_s,     "Slave-Data-Write");
  sc_trace(Tf, topx.asmem1->mbpi->dataRd_s,  "Slave-Data-Read");
  //sc_start(95, SC_SEC);
#endif

  sc_start();

  if(!sc_end_of_simulation_invoked())
  {
    //cout << "Simulation stopped: " << endl;
    //sc_stop();
  }

 // sc_close_vcd_trace_file(Tf);
  
  return exit_status;
}
