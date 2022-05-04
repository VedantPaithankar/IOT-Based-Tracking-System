#ifndef _GSMDATATRANSFER_H
#define _GSMDATATRANSFER_H

#include "constants.h"

template<class T, class U>
SC_MODULE(gsmDataTransfer)
{
  public:
  
  	//Constructor
    SC_CTOR(gsmDataTransfer);
    
    //GSM Ports
    sc_port<sc_fifo_out_if<U>> gsmCellID_out;
    sc_port<sc_fifo_out_if<U>> gsmPowerData_out;
    
    //GPS Ports
    sc_port<sc_fifo_out_if<U>> gpsLongitude_out;
    sc_port<sc_fifo_out_if<U>> gpsLatitude_out;
    sc_port<sc_fifo_out_if<U>> gpsTimeStamp_out;
      
  private:
  
   //Method Declaration
   void writeDataToServer(void);
   void WriteMessage(float,float,float,float,float);
};
#endif
