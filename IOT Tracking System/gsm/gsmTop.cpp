#include "gsmTop.h"
#include "gsmDataTransfer.cpp"
#include "gsmDataReceiver.cpp"

/*-----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T, class U>
gsmTop<T,U>::gsmTop(sc_module_name nm) : sc_module(nm)
{
  cout << "ConstructorCPP - GSMtop: " << name() << endl;
  
  //Object Instantiation for GSM Data Transfer
  gsmDataTransferPtr = new gsmDataTransfer<T,U>("GSMDataTransfer");
  
  //Object Instantiation for GSM Data Receiver
  gsmDataReceiverPtr = new gsmDataReceiver<T,U>("GSMDataReceiver");
  
 
  //Port Binding 
  gsmDataTransferPtr->gsmCellID_out(gsm_fifo);
  gsmDataReceiverPtr->gsmCellID_in(gsm_fifo);
 
  gsmDataTransferPtr->gsmPowerData_out(gsm_fifo);
  gsmDataReceiverPtr->gsmPowerData_in(gsm_fifo);
  
  gsmDataTransferPtr->gpsLongitude_out(gsm_fifo);
  gsmDataReceiverPtr->gpsLongitude_in(gsm_fifo);
  
  gsmDataTransferPtr->gpsLatitude_out(gsm_fifo);
  gsmDataReceiverPtr->gpsLatitude_in(gsm_fifo);
  
  gsmDataTransferPtr->gpsTimeStamp_out(gsm_fifo);
  gsmDataReceiverPtr->gpsTimeStamp_in(gsm_fifo);
 
  //Deleting the object and avoid Memory leak
  if(gsmDataTransferPtr)
  {
     //Free Memory
   	 delete gsmDataTransferPtr;
   	 //Pointed Dangling Pointer to null
	 gsmDataTransferPtr = nullptr;
  }
  
  if(gsmDataReceiverPtr)
  {
  	 //Free Memory
   	 delete gsmDataReceiverPtr;
   	 //Pointed Dangling Pointer to null
	 gsmDataReceiverPtr = nullptr;
  }
  
}

