#define BLUE_LED 13
#define RED_LED 19
#define GREEN_LED 26

#define	W1_PREFIX	"/sys/bus/w1/devices/"
// if DS18B20T connected to pi, you should be able to find a directory to present
// connected DS18B20 sensor under "/sys/bus/w1/devices/" and start with "28-"
// First you need to enable 1-Wire interface in pi, please check
// https://www.waveshare.com/wiki/Raspberry_Pi_Tutorial_Series:_1-Wire_DS18B20_Sensor
#define W1_FILE_NAME "28-0416925607ff"
#define	W1_POSTFIX	"/w1_slave"


void initLEDPins();
// red/green/blue should be in range 0~255
void turnOnLED(int red, int green, int blue);
void turnOffLED();
// readDS18B20Temparature return measured temperature * 1000, you should divide it
// whether by 1000.0 to get float temperature or by 1000 to get integer temperature.
int readDS18B20Temparature();