#ifndef __example
#define __example

#ifdef __cplusplus
extern 'C' {
#endif

const char EX_APP_ID[] = "00959619";
const char EX_APP_KEY[] = "1ae0bcded44365e4f83c2daa2f4ca237";
const char EX_APP_SITE[] = "api-development-jp.internal.kii.com";

const char EX_AUTH_VENDOR_ID[] = "4649";
const char EX_AUTH_VENDOR_PASS[] = "1234";

#define EX_COMMAND_HANDLER_BUFF_SIZE 4096
#define EX_STATE_UPDATER_BUFF_SIZE 4096
#define EX_MQTT_BUFF_SIZE 2048
#define EX_STATE_UPDATE_PERIOD 60000

#ifdef __cplusplus
}
#endif

#endif
