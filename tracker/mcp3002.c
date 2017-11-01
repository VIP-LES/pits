#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <wiringPiSPI.h>
#include <gertboard.h>

#include "gps.h"
#include "misc.h"
#include "mcp3002.h"

int readMCP3002()
{
    // Edit
    int RawValue;
    double Result;
    int chan = 1;
    
    unsigned char spiData [2] ;
    
    spiData[0] = 0x68;
    spiData[1] = 0;
    
    wiringPiSPIDataRW (0, spiData, 2) ;
    
    RawValue = ((spiData [0] << 8) | spiData [1]) & 0x3FF ;
    
    Result = (double)RawValue * Config.MCP3002Ref / 1024.0;
    
    return Result;
}


void *MCP3002Loop(void *some_void_ptr)
{
    double analogValue;
    struct TGPS *GPS;
    
    GPS = (struct TGPS *)some_void_ptr;
    
    while (1) {
        GPS->MCP3002 = ReadADC();
    }
    
    return 0;
}

