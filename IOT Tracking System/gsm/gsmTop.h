#ifndef _GSMTOP_H_
#define _GSMTOP_H_

#include "gsmDataTransfer.h"
#include "gsmDataReceiver.h"

template<class T, class U>
SC_MODULE(gsmTop)
{
  public:
    
    gsmTop(sc_module_name nm);
    	
  private:
    sc_fifo<U> gsm_fifo;
    
    gsmDataTransfer<T,U>*   gsmDataTransferPtr;
    
    gsmDataReceiver<T,U>*   gsmDataReceiverPtr;
    
};
#endif
