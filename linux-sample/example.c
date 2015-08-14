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
    kii_iot_command_handler_data_t command_handler_data;
    kii_iot_state_updater_data_t state_updater_data;
    char command_handler_buff[EX_COMMAND_HANDLER_BUFF_SIZE];
    char state_updater_buff[EX_STATE_UPDATER_BUFF_SIZE];
    char mqtt_buff[EX_MQTT_BUFF_SIZE];
    kii_iot_t kii_iot;

    command_handler_data.buffer = command_handler_buff;
    command_handler_data.buffer_size =
        sizeof(command_handler_buff) / sizeof(command_handler_buff[0]);
    command_handler_data.mqtt_buffer = mqtt_buff;
    command_handler_data.mqtt_buffer_size =
        sizeof(mqtt_buff) / sizeof(mqtt_buff[0]);
    command_handler_data.action_handler = action_handler;

    state_updater_data.buffer = state_updater_buff;
    state_updater_data.buffer_size =
        sizeof(state_updater_buff) / sizeof(state_updater_buff[0]);
    state_updater_data.period = EX_STATE_UPDATE_PERIOD;
    state_updater_data.state_handler = state_handler;

    init_kii_iot(&kii_iot, EX_APP_ID, EX_APP_KEY, EX_APP_SITE,
            &command_handler_data, &state_updater_data, NULL);

    onboard_with_vendor_thing_id(&kii_iot, EX_AUTH_VENDOR_ID,
            EX_AUTH_VENDOR_PASS, NULL, NULL);

    while (1) {}
    return 0;
}
