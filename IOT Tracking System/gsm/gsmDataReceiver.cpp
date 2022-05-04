#include "gsmDataReceiver.h"
/*-----------------------------
-- Constructor
-----------------------------*/
template<class T, class U>
gsmDataReceiver<T,U>::gsmDataReceiver(sc_module_name nm) : sc_module(nm)
{
  cout << "ConstructorCPP - gsmDataReceiver: " << name() << endl;
  
  //Thread Instantiation
  SC_THREAD(readDataFromServer);
 
}

template<class T, class U>
void gsmDataReceiver<T,U>::readDataFromServer(void)
{
	
	//Variable Declaration
	float cellID_in    = INT_ZERO;
   	float powerData_in = INT_ZERO;
   	float longitude_in = INT_ZERO;
	float latitude_in  = INT_ZERO;
	float timeStamp_in = INT_ZERO;
	
	for(;;)
	{    
		
		try
   		{
   
			for(int index = 0 ; index < 7 ; index++ )
   			{	
   	
			//Read Cell ID 
			cellID_in = gsmCellID_in->read();
	
			//Read Power Data
			powerData_in = gsmPowerData_in->read();
		
			//Write the Longitude 
    		longitude_in = gpsLongitude_in->read();
    		
    		//Write the Latitude
    		latitude_in  = gpsLatitude_in->read(); 
    
    		//Write the Time Stamp
    		timeStamp_in = gpsTimeStamp_in->read(); 
	
			//Print Cell ID and Power Data
			readMessage(cellID_in,powerData_in,
					longitude_in,latitude_in,
					timeStamp_in);	
					
			}	
			
		}
		catch(...)
		{
			std::cout <<"Exception Occured.. readDataFromServer"<<std::endl;
		}
					
		cout << endl;		
		cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
     	cout << "xx End of simulation! " << endl;
     	cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
     	sc_stop();	
    }
    
}

template<class T, class U>
void gsmDataReceiver<T,U>::readMessage(float Cell_ID,float Power_Data,
									   float Longitude, float Latitude,
									   float Timestamp)
{
	cout << endl;
	cout << left  << setw(INT_ELEVEN) << "GSM "
       << left  << setw(INT_FIVE) << "- Read Data(FIFO Implementation): "
       << left  << setw(INT_FIVE) << " Cell ID ="
       << right << setw(INT_THREE) << Cell_ID 
       << left  << setw(INT_FIVE)  << ", Power Data = "
       << right << setw(INT_THREE) << Power_Data
       << right << setw(INT_TWO) << "dBm"
       << endl;
    cout << left  << setw(INT_ELEVEN) << "GPS "
       << left  << setw(INT_FIVE) << "- Read Data(FIFO Implementation): "
       << left  << setw(INT_FIVE) << " Longitude ="
       << right << setw(INT_THREE) << Longitude 
       << left  << setw(INT_FIVE)  << ", Latitude = "
       << right << setw(INT_THREE) << Latitude
       << left  << setw(INT_FIVE)  << ", TimeStamp = "
       << right << setw(INT_THREE) << Timestamp
       << endl;
    cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
}

