#include "pi_control.h"
#include <wiringPi.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void initLEDPins() {
    wiringPiSetupGpio(); // Initializes wiringPi using the Broadcom GPIO pin numbers
    softPwmCreate (BLUE_LED, 0, 100); // BCM 13 as output to Blue LED
    softPwmCreate (RED_LED, 0, 100); // BCM 19 as output to Red LED
    softPwmCreate (GREEN_LED, 0, 100); // BCM 26 as output to Green LED
}

void turnOnLED(int red, int green, int blue) {
    softPwmWrite (RED_LED, red);
    softPwmWrite (BLUE_LED, blue);
    softPwmWrite (GREEN_LED, green);
}
void turnOffLED() {
    softPwmWrite (RED_LED, 0);
    softPwmWrite (BLUE_LED, 0);
    softPwmWrite (GREEN_LED, 0);
}

int readDS18B20Temparature(const char *deviceId)
{
    int fd;
    char *fileName;

    // Allocate space for the filename
    if ((fileName = malloc(strlen(W1_PREFIX) + strlen(W1_POSTFIX) + strlen(deviceId) + 1)) == NULL)
        return -9999;

    sprintf(fileName, "%s%s%s", W1_PREFIX, deviceId, W1_POSTFIX);
    fd = open(fileName, O_RDONLY);
    free(fileName);
    if (fd < 0)
        return -9999;

    char buffer[4096];
    char *p;
    int temp, sign;

    // Rewind the file - we're keeping it open to keep things going
    //	smoothly
    lseek(fd, 0, SEEK_SET);

    // Read the file - we know it's only a couple of lines, so this ought to be
    //	more than enough
    if (read(fd, buffer, 4096) <= 0) // Read nothing, or it failed in some odd way
        return -9998;

    // Look for YES, then t=
    if (strstr(buffer, "YES") == NULL)
        return -9997;

    if ((p = strstr(buffer, "t=")) == NULL)
        return -9996;

    // p points to the 't', so we skip over it...
    p += 2;

    // and extract the number
    //	(without caring about overflow)
    if (*p == '-') // Negative number?
    {
        sign = -1;
        ++p;
    }
    else
        sign = 1;

    temp = 0;
    while (isdigit(*p))
    {
        temp = temp * 10 + (*p - '0');
        ++p;
    }

    // We know it returns temp * 1000, but we only really want temp * 10, so
    //	do a bit of rounding...
    temp = (temp + 50) / 100;

    close(fd);
    return temp * sign;
}