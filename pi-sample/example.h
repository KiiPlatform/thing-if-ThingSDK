#ifndef __example
#define __example

#ifdef __cplusplus
extern 'C' {
#endif

/* Go to https:/developer.kii.com and create app for you! */
const char EX_APP_ID[] = "ex4liedteote";
const char EX_APP_KEY[] = "d787de623f15483fa887815017f010e6";
/* JP: "api-jp.kii.com" */
/* US: "api.kii.com" */
/* SG: "api-sg.kii.com" */
/* CN: "api-cn3.kii.com" */
const char EX_APP_SITE[] = "api.kii.com";

#define EX_COMMAND_HANDLER_BUFF_SIZE 4096
#define EX_STATE_UPDATER_BUFF_SIZE 4096
#define EX_MQTT_BUFF_SIZE 2048
#define EX_STATE_UPDATE_PERIOD 10

#ifdef __cplusplus
}
#endif

#endif
