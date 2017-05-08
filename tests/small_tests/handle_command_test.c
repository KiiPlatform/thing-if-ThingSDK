#include "kii_thing_if_environment_test.h"

#include <kii_thing_if.h>
#include <kii_thing_if_test.h>

#include <stdio.h>

#include <gtest/gtest.h>

static kii_bool_t action_handler_returning_true(
        const char* alias,
        const char* action_name,
        const char* action_value,
        char error[EMESSAGE_SIZE + 1])
{
    printf("HERE A: %s %s %s\n", alias, action_name, action_value);
    return KII_TRUE;
}

static kii_bool_t state_handler_returning_true(
        kii_t *kii,
        KII_THING_IF_WRITER writer)
{
    assert(0);
    return KII_TRUE;
}

static kii_socket_code_t test_connect(
        void* context,
        const char* host,
        unsigned int port)
{
    assert(0);
    return KII_SOCKETC_OK;
}

static kii_socket_code_t test_send(
        void* context,
        const char* buffer,
        size_t length)
{
    assert(0);
    return KII_SOCKETC_OK;
}

static kii_socket_code_t test_recv(
        void* context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length)
{
    assert(0);
    return KII_SOCKETC_OK;
}

static kii_socket_code_t test_close(void* context)
{
    assert(0);
    return KII_SOCKETC_OK;
}

TEST(kiiThingIfTest, handle_command)
{
    kii_thing_if_t kii_thing_if;
    kii_thing_if_command_handler_resource_t command_handler_resource;
    kii_thing_if_state_updater_resource_t state_updater_resource;
    char command_handler_buff[1024];
    char state_updater_buff[1024];
    char mqtt_buff[1024];

    char command_payload[] = "{\"commandID\":\"XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX\",\"actions\":[{\"AirConditionerAlias\":[{\"turnPower\":true},{\"setPresetTemperature\":25}]},{\"HumidityAlias\":[{\"setPresetHumidity\":45}]}],\"issuer\":\"user:XXXXXXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXX\"}";

    kii_socket_test_context_t test_context;
    test_context.CONNECT = test_connect;
    test_context.SEND = test_send;
    test_context.RECV = test_recv;
    test_context.CLOSE = test_close;

    command_handler_resource.buffer = command_handler_buff;
    command_handler_resource.buffer_size =
        sizeof(command_handler_buff) / sizeof(command_handler_buff[0]);
    command_handler_resource.mqtt_buffer = mqtt_buff;
    command_handler_resource.mqtt_buffer_size =
        sizeof(mqtt_buff) / sizeof(mqtt_buff[0]);
    command_handler_resource.action_handler = action_handler_returning_true;
    command_handler_resource.state_handler = state_handler_returning_true;

    state_updater_resource.buffer = state_updater_buff;
    state_updater_resource.buffer_size =
        sizeof(state_updater_buff) / sizeof(state_updater_buff[0]);
    state_updater_resource.period = 10;
    state_updater_resource.state_handler = state_handler_returning_true;

    ASSERT_EQ(
        KII_TRUE,
        init_kii_thing_if(
            &kii_thing_if,
            "app_id",
            "app_key",
            "JP",
            &command_handler_resource,
            &state_updater_resource,
            NULL));

    kii_thing_if.command_handler.kii_core.http_context.
        socket_context.app_context = &test_context;

    strcpy(kii_thing_if.command_handler.kii_core.author.author_id, "owenr");
    strcpy(kii_thing_if.state_updater.kii_core.author.author_id, "owenr");
    strcpy(kii_thing_if.command_handler.kii_core.author.access_token, "token");
    strcpy(kii_thing_if.state_updater.kii_core.author.access_token, "token");

    test_handle_command(&(kii_thing_if.command_handler),
            command_payload, strlen(command_payload));

}
