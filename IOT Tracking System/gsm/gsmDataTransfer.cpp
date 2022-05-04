#include "gsmDataTransfer.h"
/*-----------------------------
-- Constructor
-----------------------------*/
template<class T, class U>
gsmDataTransfer<T,U>::gsmDataTransfer(sc_module_name nm) : sc_module(nm)
{
  cout << "ConstructorCPP - gsmDataTransfer: " << name() << endl;
  
  
  //Thread Instantiation
  SC_THREAD(writeDataToServer); 
}

#if 0
template<class T, class U>
void gsmDataTransfer<T,U>::ReadMCUData(float &longitude,float &latitude,
									   float &timestamp,float &cellID, 
									   float &powerData)
{
	//Need to be implemented by MCU side which should be called in 
	//writeDataToServer.
	//The Received GPS and GSM data is further written over FIFO to Server 
	//as per the discussion
}
#endif

template<class T, class U>
void gsmDataTransfer<T,U>::writeDataToServer(void)
{
	sc_time DELAY(1,SC_NS);
	sc_time tranfer_DELAY(1000,SC_NS);

	float cellID_out    = 100;
   	float powerData_out = 100;
   	float longitude_out = 100;
	float latitude_out  = 100;
	float timeStamp_out = 100;
   	
   	try
   	{
   		for(int index = 0 ; index < 7 ; index++ )
   		{
   			//Delay
    		wait(tranfer_DELAY);
    	
			//Write the Cell ID 
    		gsmCellID_out->write(cellID_out);
    		
    		//Write the Power Data
    		gsmPowerData_out->write(powerData_out); 
    
    		//Write the Longitude 
    		gpsLongitude_out->write(longitude_out);
    			
    		//Write the Latitude
    		gpsLatitude_out->write(latitude_out); 
    
    		//Write the Time Stamp
    		gpsTimeStamp_out->write(timeStamp_out); 
    		
    		//Print the GPS and GSM Values
    		WriteMessage(cellID_out,powerData_out,
    			 longitude_out,latitude_out,
    			 timeStamp_out);
    			 
    		cellID_out    = cellID_out + 100;
   			powerData_out = powerData_out+ 100;
   			longitude_out = longitude_out+ 100;
			latitude_out  = latitude_out+ 100;
			timeStamp_out = timeStamp_out+ 100;
    			 
    		//Delay
    		wait(DELAY);
    	}	
    }
    catch(...)
    {
    	std::cout <<"Exception Occured.. writeDataToServer"<<std::endl;
    }
    		
}

template<class T, class U>
void gsmDataTransfer<T,U>::WriteMessage(float Cell_ID,float Power_Data,
										float Longitude, float Latitude,
										float Timestamp)
{
	cout << endl;
	cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
	cout << left  << setw(INT_ELEVEN) << "GSM "
       << left  << setw(INT_FIVE) << "- Write Data(FIFO Implementation): "
       << left  << setw(INT_FIVE) << " Cell ID = "
       << right << setw(INT_THREE) << Cell_ID 
       << left  << setw(INT_FIVE)  << ", Power Data = "
       << right << setw(INT_THREE) << Power_Data
       << right << setw(INT_TWO) << "dBm"
       << endl;
    cout << left  << setw(INT_ELEVEN) << "GPS "
       << left  << setw(INT_FIVE) << "- Write Data(FIFO Implementation): "
       << left  << setw(INT_FIVE) << " Longitude = "
       << right << setw(INT_THREE) << Longitude 
       << left  << setw(INT_FIVE)  << ", Latitude = "
       << right << setw(INT_THREE) << Latitude
       << left  << setw(INT_FIVE)  << ", TimeStamp = "
       << right << setw(INT_THREE) << Timestamp
       << endl;
}



