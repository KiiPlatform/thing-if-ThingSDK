#ifndef __example
#define __example

#ifdef __cplusplus
extern 'C' {
#endif

const char EX_APP_ID[] = "a4c6c6a7";
const char EX_APP_KEY[] = "1154dcfd901bf1fdf5606d3b3d92e095";
const char EX_APP_SITE[] = "JP";

/* If two applications use same EX_AUTH_VENDOR_ID, receiving
 * notification does not work well. To avoid this situation, 3 vendor
 * thing ids are created. If you have some troubles in receiving
 * notification, the troubles may be resolved by changing vendor thing
 * id.
 *
 * If you change vendor thing id, you also need to change THING_ID in
 * script/smart-light-push.sh
 */
const char EX_AUTH_VENDOR_ID[] = "4649";
/* const char EX_AUTH_VENDOR_ID[] = "46490"; */
/* const char EX_AUTH_VENDOR_ID[] = "46491"; */
const char EX_AUTH_VENDOR_PASS[] = "1234";

#define EX_COMMAND_HANDLER_BUFF_SIZE 4096
#define EX_STATE_UPDATER_BUFF_SIZE 4096
#define EX_MQTT_BUFF_SIZE 2048

#ifdef __cplusplus
}
#endif

#endif
