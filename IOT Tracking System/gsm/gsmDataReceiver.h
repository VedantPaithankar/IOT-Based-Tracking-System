#ifndef _GSMDATARECEIVER_H_
#define _GSMDATARECEIVER_H_

#include "constants.h"

template<class T, class U>
SC_MODULE(gsmDataReceiver)
{
  public:
    // GSM Ports
    sc_port<sc_fifo_in_if<U>> gsmCellID_in;
    sc_port<sc_fifo_in_if<U>> gsmPowerData_in;
    
    //GPS Ports
    sc_port<sc_fifo_in_if<U>> gpsLongitude_in;
    sc_port<sc_fifo_in_if<U>> gpsLatitude_in;
    sc_port<sc_fifo_in_if<U>> gpsTimeStamp_in;
    
    // Constructor
    SC_CTOR(gsmDataReceiver);
  private:
	
	//Method Declaration
	void readDataFromServer(void);
	void readMessage(float,float,float,float,float);
};
#endif
