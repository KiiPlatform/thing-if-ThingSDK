#include "pi_control.h"
#include <wiringPi.h>


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