#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>

#include "gps.h"
#include "misc.h"

#define GEIGER_ADDRESS 0x20		// I2C address of geiger counter

void *GeigerLoop(void *some_void_ptr)
{
	struct TGPS *GPS;
	int fd;
	FILE *log;

	GPS = (struct TGPS *)some_void_ptr;

	// Initialize geiger counter
	
	while (1)
	{
		if ((fd = open_i2c(GEIGER_ADDRESS)) >= 0)
		{
			char bytes[4];
			read(fd, bytes, 4);
			int countPerMinute = bytes[0] | ( (int)bytes[1] << 8 ) | ( (int)bytes[2] << 16 ) | ( (int)bytes[3] << 24 );
			//int countPerMinute = wiringPiI2CRead(fd);

			printf("Counts Per Minute = %d\n", countPerMinute);

			if (countPerMinute >= 0)
			{
				//printf("Counts Per Minute = %d\n", countPerMinute);
				GPS->GeigerCount = countPerMinute;

				log = fopen("geigerlog.csv", "a");
				time_t currentTime = time(NULL);
				char timestamp[26];
				strftime(timestamp, 26, "%Y-%m-%d %H:%M:%S", localtime(&currentTime));

				fprintf(log, "%s, %d, cpm\n", timestamp, countPerMinute);
				fclose(log);
			}

			close(fd);
		}

		sleep(10);
	}

    return 0;
}
