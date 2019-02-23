#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#include "gps.h"
#include "misc.h"

int *SerialMain(void *some_void_ptr)
{
  struct TGPS *GPS;
	GPS = (struct TGPS *)some_void_ptr;

  int fd ;

  if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

  if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }

  while (1)
  {

    char *msgOut ;
    if (asprintf(&msgOut, "{\"Altitude\": %d, \"Temperature\": %f, \"Humidity\": %f, \"Pressure\": %f, \"Geiger\": %d}",
      GPS->Altitude, GPS->DS18B20Temperature[Config.ExternalDS18B20], GPS->Humidity, GPS->Pressure,
      GPS->GeigerCount) >= 0 && msgOut != NULL)
      {
        serialPuts(fd, msgOut) ;
        free(msgOut) ;
      }

    delay (30000) ; // delay 30 seconds

    // while (serialDataAvail (fd))
    // {
    //   printf (" -> %3d", serialGetchar (fd)) ;
    //   fflush (stdout) ;
    // }
  }
  return 0 ;
}