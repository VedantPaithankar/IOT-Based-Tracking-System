#ifndef _ASPROPACK_H_
#define _ASPROPACK_H_

#include <string>
#include "systemc.h"

//#define ADDR_WIDTH        7
//#define RAM_DEPTH         1 << ADDR_WIDTH

// Data Bus
#define BITWIDTH         32
#define BYTESPERMEMLINE   4

// Address Bus
// 1010_1010_1010_1010_1010_1010_1010_1010
#define BUSWIDTHS        32//8    // #address bits
// 1010_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx
#define BUSDECODEBITS     4//2    // #upper chip select decode bits
// xxxx_1010_1010_1010_1010_1010_1010_1010
#define BUSMEMINTBITS    28//6    //=32-4, #memory internal address bits
#define BUSTARGETMASK   0xF//0x3    // mask for the upper chip select decode bits
#define BUSDECODEMASK   0x0FFFFFFF//0x3F   // 7 Nibbles, mask for the memory internal address bits
// END: Address Bus

// Memories
#define MEM0WIDTH         4 // bytes, = 32 bits
#define MEM0BYTES        4*1024 // #bytes (or lines? I guess lines of 32 bits)
#define MEM1WIDTH         4 // bytes
#define MEM1BYTES        8*1024
#define MEM2WIDTH         4 // bytes
#define MEM2BYTES        64
#define MEM3WIDTH         4 // bytes
#define MEM3BYTES        64
// END: Memories

// Masters
// Words and Bytes in the Masters
#define MAS1WIDTH         4 // data width of master 1
#define MAS2WIDTH         4
// Total number of addresses needed in the masters
#define MAS1ADDRESSES   256 // = 4*64
#define MAS2ADDRESSES   256 // = 4*64
// END: Masters

#define CYPHERROT         1

#define DEBUG             0

//#define CPU1PRINTS        1
#define SLAVEBPIPRINTS    0
#define SLAVEFUNPRINTS    0
#define MASTERBPIPRINTS   0
#define MASTERFUNPRINTS   0
#define MASTER1FUNPRINTS  0
#define MASTER2FUNPRINTS  0
#define BUSPRINTS         0
#define CPUPRINTS         1

const sc_time cpuSleep(10, SC_NS);
const sc_time rwAccess_t(1, SC_NS);
const sc_time rwReadDelay_t(2, SC_NS);
const sc_time rwWriteDelay_t(3, SC_NS);

const sc_time memAccess00_c(10, SC_NS);
const sc_time memAccess01_c(10, SC_NS);
const sc_time memAccess02_c(10, SC_NS);
const sc_time memAccess03_c(10, SC_NS);
const sc_time memDMIInvalidate_c(3, SC_US);
const sc_time quantumKeeperSync_c(1, SC_US);
const sc_time quantumKeeperAdd_c(100, SC_NS);

#endif
