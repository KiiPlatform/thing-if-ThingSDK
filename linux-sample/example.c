#include "example.h"

#include <kii_iot.h>

#include <string.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    char command_handler_buff[EX_COMMAND_HANDLER_BUFF_SIZE];
    char state_updater_buff[EX_STATE_UPDATER_BUFF_SIZE];
    char mqtt_buff[EX_MQTT_BUFF_SIZE];
    kii_iot_t kii_iot;

    init_kii_iot(&kii_iot, EX_APP_ID, EX_APP_KEY, EX_APP_SITE,
            mqtt_buff,
            sizeof(mqtt_buff) / sizeof(mqtt_buff[0]),
            command_handler_buff,
            sizeof(command_handler_buff) / sizeof(command_handler_buff[0]),
            state_updater_buff,
            sizeof(state_updater_buff) / sizeof(state_updater_buff[0]), NULL);

    onboard_with_vendor_thing_id(&kii_iot, EX_AUTH_VENDOR_ID,
            EX_AUTH_VENDOR_PASS, NULL, NULL);

    return 0;
}
