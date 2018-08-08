#define BLUE_LED 13
#define RED_LED 19
#define GREEN_LED 26

#define	W1_PREFIX	"/sys/bus/w1/devices/28-"
#define	W1_POSTFIX	"/w1_slave"


void initLEDPins();
void turnOnLED(int red, int green, int blue);
void turnOffLED();
int readDS18B20Temparature(const char *deviceId);