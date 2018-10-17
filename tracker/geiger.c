#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>

#include "gps.h"
#include "misc.h"

#define GEIGER_ADDRESS 0x20		// I2C address of geiger counter

void *GeigerLoop(void *some_void_ptr)
{
	struct TGPS *GPS;
	int fd;

	GPS = (struct TGPS *)some_void_ptr;

	// Initialize geiger counter
	
	while (1)
	{
		if ((fd = open_i2c(GEIGER_ADDRESS)) >= 0)
		{
			int countPerMinute = wiringPiI2CRead(fd);
			printf("Counts Per Minute = %d\n", countPerMinute);
			GPS->GeigerCount = countPerMinute;

			close(fd);
		}

		sleep(10);
	}

    return 0;
}
