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

  FILE *log;
  char fileName[50];
  sprintf(fileName, "logs/serialLog.txt");

  int fd1; // particle board
  int fd2; // arduino

  while (1)
  {
    if ((fd1 = serialOpen("/dev/ttyACM0", 9600)) < 0)
    {
      fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
      delay(30000); // delay 30 seconds
    }
    else
    {
      break;
    }
  }

  while (1)
  {
    if ((fd2 = serialOpen("/dev/ttyACM1", 9600)) < 0)
    {
      fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
      delay(30000); // delay 30 seconds
    }
    else
    {
      break;
    }
  }

  if (wiringPiSetup() == -1)
  {
    fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
    return 1;
  }

  while (1)
  {
    // send out data via particle board
    char *msgOut;
    if (asprintf(&msgOut, "{\"Altitude\": %d, \"Temperature\": %f, \"Humidity\": %f, \"Pressure\": %f, \"Geiger\": %d}",
                 GPS->Altitude, GPS->DS18B20Temperature[Config.ExternalDS18B20], GPS->Humidity, GPS->Pressure,
                 GPS->GeigerCount) >= 0 &&
        msgOut != NULL)
    {
      serialPuts(fd1, msgOut);
      free(msgOut);
    }

    // receive and log data from arduino
    if (serialDataAvail(fd2))
    {
      log = fopen(fileName, "a");
      fprintf(log, "%3d", serialGetchar(fd2));
      while (serialDataAvail(fd2))
      {
        fprintf(log, "%3d", serialGetchar(fd2));
      }
      fprintf(log, "\n");
      fclose(log);
    }

    delay(30000); // delay 30 seconds
  }
  return 0;
}