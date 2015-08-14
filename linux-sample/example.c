#include "example.h"

#include <kii_iot.h>

#include <string.h>
#include <stdio.h>

static kii_bool_t action_handler(
        const char* schema,
        int schema_version,
        const char* action_name,
        const char* action_params,
        char error[EMESSAGE_SIZE + 1])
{
    printf("schema=%s, schema_version=%d, action name=%s, action params=%s\n",
            schema, schema_version, action_name, action_params);
    return KII_TRUE;
}

static kii_bool_t state_handler(
        kii_t* kii,
        KII_IOT_WRITER writer)
{
    // TODO: implement me.
    return KII_TRUE;
}

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
            sizeof(state_updater_buff) / sizeof(state_updater_buff[0]),
            60000,
            NULL,
            action_handler,
            state_handler);

    onboard_with_vendor_thing_id(&kii_iot, EX_AUTH_VENDOR_ID,
            EX_AUTH_VENDOR_PASS, NULL, NULL);

    while (1) {}
    return 0;
}
