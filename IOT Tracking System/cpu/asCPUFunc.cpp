#include "asCPUFunc.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*-----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T, class U>
asCPUFunc<T,U>::asCPUFunc(sc_module_name nm, sc_event& mFuncDatRdEv)
  : sc_module(nm)
  , mFuncDatRdEv_e(mFuncDatRdEv)
{
  SC_HAS_PROCESS(asCPUFunc); // because no SC_CTOR()
  cout << left << setw(30) << "ConstructorCPP - asCPUFunc:   "
       << left << setw(36) << name()
       << endl;
    
  SC_THREAD(getDatThread);
  SC_THREAD(cpuSequenceThread);
   //GSM data thread
   SC_THREAD(writeGSMData);	   
  // binds the fifo to the export
  data_i.bind(dataIn_s);
}
/*-----------------------------------------------------------------------------
-- Methods
-----------------------------------------------------------------------------*/

// gets read data from MasterBPI (and so from the memory)
template<class T, class U>
void asCPUFunc<T,U>::getDatThread(void)
{
  while(true)
  {
    // wait on the information, that the read data is available
    wait(mFuncDatRdEv_e);
    // notify, that that the data is available - captured by the algorithm
    readMemRdy_e.notify(SC_ZERO_TIME);
    
    if(MASTERFUNPRINTS)
      cpuFuncPrint01(dataIn_s.read());

  }
}

/*-----------------------------------------------------------------------------
-- void cpuSequenceThread(void): This is the algorithm of the CPU.
-----------------------------------------------------------------------------*/
template<class T, class U>
void asCPUFunc<T,U>::cpuSequenceThread(void)
{
  T dataIndex_v;
  unsigned int addr_v;
  
  while(true)
  {
     // set the bus inactive
     cmd_o->write(tlm::TLM_IGNORE_COMMAND);
     // wait for wake-up
     wait(cpuSleep);
     /*-----------------------------------------------------------------------
     -- Write Section
      ----------------------------------------------------------------------*/
     cout << " " << endl;
     cout << "------------------------------------------------------" << endl;
     cout << "Write section ...." << endl;
     cout << "------------------------------------------------------" << endl;
     
     currentAddress_v = 0x00000100;
     
     // set the data for write
     //dataIndex_v = (T)0xDEADBEEF;
     //dataIndex_v = (T)0x40;
     
     addr_v = currentAddress_v;

     for(int index = INT_ZERO; index < INT_SEVEN; index++)
     {
     	//wait until data available for write
		wait(gsmevent);
		
		std::cout<<"GSM Register Values  : Cell ID : "
			<<cellID_out<<", Power Data : "
			<<powerData_out<<"dBm"<<std::endl;
        
        // print the address and data
        if(CPUPRINTS)
        cpuWr1Print(addr_v, cellID_out,false);
       
        // write mnemonic
        cpuWrCmd(addr_v, cellID_out);

        // set the new (next) address
        addr_v = addr_v + m_txn_data_size;
	   
	    // let the CPU sleep for a while
     	wait(cpuSleep);  	   
	   
	   //write Power_Data
	   if(CPUPRINTS)
	   {
	   	cpuWr1Print(addr_v, powerData_out,false);
	   }
	   
	   //write mnemonic
	   cpuWrCmd(addr_v, powerData_out);
	   
	   addr_v = addr_v + m_txn_data_size;
	   
       // let the CPU sleep for a while
       wait(cpuSleep);
       
       // Separator between the write operations
       cout << "------Write separator (Peripheral 0)------------------" << endl;
	   
	   //notify GSM to read next value
	   cpuevent.notify();
     }

	#if 0
    // set byte address; second salve
     currentAddress_v = 0x10000100;
     //dataIndex_v = (T)0x40;
     // set the data for write
     dataIndex_v = (T)0xBEEFDEAD;
     addr_v = currentAddress_v;

     for(int i = 0; i < 10; i++)
     {
       // print the address and data
       if(CPUPRINTS)
         cpuWr1Print(addr_v, dataIndex_v, true);
       
       // write mnemonic
       cpuWrCmd(addr_v, dataIndex_v);
       
       // set the new (next) address
       addr_v  = addr_v + m_txn_data_size;
       // set the new (next) data
       dataIndex_v = dataIndex_v + 1;
       // let the CPU sleep for a while
       wait(cpuSleep);
       // Separator between the write operations
       cout << "------Write separator (Peripheral 1)------------------" << endl;
     }
     #endif
     
     // let the CPU sleep for a while
     wait(cpuSleep);
     
     /*-----------------------------------------------------------------------
     -- Read Section
      ----------------------------------------------------------------------*/
     cout << " " << endl;
     cout << "------------------------------------------------------" << endl;
     cout << "Read section ...." << endl;
     cout << "------------------------------------------------------" << endl;
     // set address; lower slave
     // set the byte address
     currentAddress_v = 0x00000100;
     addr_v = currentAddress_v;
     
     for(int index = INT_ZERO; index < INT_FOURTEEN; index++)
     {
       // read mnemonic
       dataIndex_v = cpuRdCmd(addr_v); // starts the read; the data is old
       
       // print the address and data
       if(CPUPRINTS)
         cpuRd1Print(addr_v, dataIndex_v, false);
       
       // set the new (next) address
       addr_v      = addr_v + m_txn_data_size;
       // let the CPU sleep for a while
       wait(cpuSleep);
       // Separator between the write operations
       cout << "------Read separator (Peripheral 0)------------------" << endl;
     }

	 #if 0
     // set address; second slave
     // set the byte address
     currentAddress_v = 0x10000100;
     addr_v = currentAddress_v;
     
     for(int i = 0; i < 10; i++)
     {
       // read mnemonic
       dataIndex_v = cpuRdCmd(addr_v); // starts the read; the data is old
       
       // print the address and data
       if(CPUPRINTS)
         cpuRd1Print(addr_v, dataIndex_v, true);
       
       // set the new (next) address
       addr_v      = addr_v + m_txn_data_size;
       // let the CPU sleep for a while
       wait(cpuSleep);
       // Separator between the write operations
       cout << "------Read separator (Peripheral 1)------------------" << endl;
     }
	 
	 cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
     cout << "xx End of simulation! " << endl;
     cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
     sc_stop();
     #endif
  }
}

//reads the GSM data from text file(GSM satellite)
template<class T, class U>
void asCPUFunc<T,U>::writeGSMData(void)
{

	//Variable Declaration
	sc_time t_DELAY(INT_ONE,SC_NS);
	std::string splitString[INT_EIGHT];
	
	// wait for wake-up
	wait(cpuSleep);
	
	//Input Stream Power Data file
 	std::ifstream file(POWER_DATA_TEXT_FILE);
 	
 	try
 	{
 		//Check if File is open
 		if(file.is_open())
 		{
 			std::string line;
 		
 			//Store the text file data into string
 			for(int dataElements = INT_ZERO; dataElements < INT_SEVEN ; dataElements++ )
 			{
 				for(int index = INT_ZERO ; index < INT_EIGHT ; index ++ )
    			{	
    				//Delimit text file line using comma 
 					std::getline(file,line,',');    		
    		
    				//Store the line into array
    				splitString[index] = line;
    			}
    			
    			//Convert the string Cell ID to int		
    			stringstream stringToIntCellID(splitString[INT_FOUR]);
    			stringToIntCellID >> cellID_out; 
    		
    			//Convert the string Power Data to int	
    			stringstream stringToIntPowerData(splitString[INT_SIX]);
    			stringToIntPowerData >> powerData_out;
    		
    		
    			cout<<endl<<"GSM Text File Values : Cell ID : "
    					  <<cellID_out
    					  <<", Power Data : "
    					  <<powerData_out<<"dBm"<<endl;
    	
     			//notify to CPU that GSM data has been read from satellite
				gsmevent.notify();
			
				//wait for CPU to write the read data to BPI
				wait(cpuevent);	
    		}
    	
  	 	}
    	else
    	{	
    		//File cannot be Opened due to permissions
    		cout<<"Unable to open the GSM TextFile"<<endl;
 		}
	}
	catch(...)
	{
		std ::cout <<"Exception Occured.. writeGSMData"<<std::endl;
	
	}
	
	//Close the file
	file.close(); 
	
}

template<class T, class U>
void asCPUFunc<T,U>::cpuWr1Print(unsigned int addr, T data, bool typecast)
{
	if(!typecast)
	{
		short int temp ;
  		temp = (short int)data;
  		
  		cout << left  << setw(11)  << "CPU "
       	<< left  << setw(20) << "- writes: "
       	<< left  << setw(3)  << " @ "
       	<< right << setw(9)  << sc_time_stamp()
       	<< left  << setw(10) << ", byteAdr."
       	<< right << hex << setw(9) << addr
       	<< left  << setw(7)  << ", data "
       	<< right << dec << setw(9) <<temp
       	<< endl;
  	}
  	else
  	{
  		cout << left  << setw(11)  << "CPU "
       	<< left  << setw(20) << "- writes: "
       	<< left  << setw(3)  << " @ "
       	<< right << setw(9)  << sc_time_stamp()
       	<< left  << setw(10) << ", byteAdr."
       	<< right << hex << setw(9) << addr
       	<< left  << setw(7)  << ", data "
       	<< right << hex << setw(9) <<data
       	<< endl;
  	}
  	
 
}

template<class T, class U>
void asCPUFunc<T,U>::cpuWrCmd(unsigned int addr, T data)
{
  // write the generated data to the BPI, to send it to the slave
  data_o->write(data);
  // write the corresponding address
  address_o->write(addr);
  // send the WRITE command to the BPI
  cmd_o->write(tlm::TLM_WRITE_COMMAND);
  // wait for the write delay
  wait(rwAccess_t + rwWriteDelay_t);
  // remove the WRITE command
  cmd_o->write(tlm::TLM_IGNORE_COMMAND);
}

template<class T, class U>
T asCPUFunc<T,U>::cpuRdCmd(unsigned int addr)
{
  // write the address to the BPI for the data to be read
  address_o->write(addr);
  // send the READ command to the BPI
  cmd_o->write(tlm::TLM_READ_COMMAND);
  // wait for data from BPI - Bus - Memory
  wait(readMemRdy_e);
  // wait for the read delay
  wait(rwAccess_t + rwReadDelay_t);
  // remove the READ command
  cmd_o->write(tlm::TLM_IGNORE_COMMAND);
  // return the read data to the algorithm
  return dataIn_s.read();
}

template<class T, class U>
void asCPUFunc<T,U>::cpuRd1Print(unsigned int addr, T data, bool typecast)
{
	if(!typecast)
	{
		short int temp ;
  		temp = (short int)data;
  	
  		cout << left  << setw(11) << "CPU "
       	<< left  << setw(20) << "- reads: "
       	<< left  << setw(3)  << " @ "
       	<< right << setw(9)  << sc_time_stamp()
       	<< left  << setw(10) << ", byteAdr."
       	<< right << hex << setw(9) << addr
       	<< left  << setw(7)  << ", data "
       	<< right << dec << setw(9) << temp
       	<< endl;
    }
    else
    {
    	cout << left  << setw(11) << "CPU "
       	<< left  << setw(20) << "- reads: "
       	<< left  << setw(3)  << " @ "
       	<< right << setw(9)  << sc_time_stamp()
       	<< left  << setw(10) << ", byteAdr."
       	<< right << hex << setw(9) << addr
       	<< left  << setw(7)  << ", data "
       	<< right << hex << setw(9) << data
       	<< endl;
    }
}

/*-----------------------------------------------------------------------------
-- Not needed things, just printouts
-----------------------------------------------------------------------------*/
template<class T, class U>
void asCPUFunc<T,U>::cpuFuncPrint01(T data)
{
  cout << left  << setw(11) << "Master-Func"
       << left  << setw(20) << "- Data read: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(19) << ""
       << left  << setw(7)  << ", data "
       << right << hex << setw(9) << data
       << endl;
}
