#include "kii_thing_if_environment_test.h"

#include <kii_thing_if.h>
#include <kii_thing_if_test.h>

#include <stdio.h>
#include <string.h>

#include <gtest/gtest.h>

typedef struct sended_requests_t {
    int num;
    char** data_array;
} sended_requests_t;

typedef struct test_context_t {
    sended_requests_t sended_requests;
} test_context_t;

typedef struct expected_action_t {
    const char* alias;
    const char* action_name;
    const char* action_value;
} expected_action_t;

static int handle_command_expected_index;
static expected_action_t handle_command_expected[3];

static kii_bool_t action_handler_returning_true(
        const char* alias,
        const char* action_name,
        const char* action_value,
        char error[EMESSAGE_SIZE + 1])
{
    EXPECT_STREQ(
        handle_command_expected[handle_command_expected_index].alias,
        alias);
    EXPECT_STREQ(
        handle_command_expected[handle_command_expected_index].action_name,
        action_name);
    EXPECT_STREQ(
        handle_command_expected[handle_command_expected_index].action_value,
        action_value);
    handle_command_expected_index++;
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
    sended_requests_t* sended_requests =
      &(((test_context_t*)context)->sended_requests);
    int num = sended_requests->num + 1;
    char** data_array = (char**)realloc(
            sended_requests->data_array,
            sizeof(char**) * num);
    char* data = (char*)malloc(sizeof(char));
    data[0] = '\0';
    data_array[num - 1] = data;
    sended_requests->num = num;
    sended_requests->data_array = data_array;
    return KII_SOCKETC_OK;
}

static kii_socket_code_t test_send(
        void* context,
        const char* buffer,
        size_t length)
{
    sended_requests_t* sended_requests =
        &(((test_context_t*)context)->sended_requests);
    int index = sended_requests->num - 1;
    sended_requests->data_array[index] = (char*)realloc(
            sended_requests->data_array[index],
            strlen(sended_requests->data_array[index]) + length + 1);
    strncat(sended_requests->data_array[index], buffer, length);

    return KII_SOCKETC_OK;
}

static kii_socket_code_t test_recv(
        void* context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length)
{
    return KII_SOCKETC_OK;
}

static kii_socket_code_t test_close(void* context)
{
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
    int i;

    char command_payload[] = "{\"commandID\":\"XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX\",\"actions\":[{\"AirConditionerAlias\":[{\"turnPower\":true},{\"setPresetTemperature\":25}]},{\"HumidityAlias\":[{\"setPresetHumidity\":45}]}],\"issuer\":\"user:XXXXXXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXX\"}";

    kii_socket_test_context_t socket_test_context;
    test_context_t test_context;
    memset(&socket_test_context, 0, sizeof(socket_test_context));
    memset(&test_context, 0, sizeof(test_context));

    socket_test_context.CONNECT = test_connect;
    socket_test_context.SEND = test_send;
    socket_test_context.RECV = test_recv;
    socket_test_context.CLOSE = test_close;
    socket_test_context.context = &test_context;

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
        socket_context.app_context = &socket_test_context;

    strcpy(kii_thing_if.command_handler.kii_core.author.author_id, "owenr");
    strcpy(kii_thing_if.state_updater.kii_core.author.author_id, "owenr");
    strcpy(kii_thing_if.command_handler.kii_core.author.access_token, "token");
    strcpy(kii_thing_if.state_updater.kii_core.author.access_token, "token");

    handle_command_expected_index = 0;
    handle_command_expected[0].alias = "AirConditionerAlias";
    handle_command_expected[0].action_name = "turnPower";
    handle_command_expected[0].action_value = "true";
    handle_command_expected[1].alias = "AirConditionerAlias";
    handle_command_expected[1].action_name = "setPresetTemperature";
    handle_command_expected[1].action_value = "25";
    handle_command_expected[2].alias = "HumidityAlias";
    handle_command_expected[2].action_name = "setPresetHumidity";
    handle_command_expected[2].action_value = "45";

    test_handle_command(&(kii_thing_if.command_handler),
            command_payload, strlen(command_payload));

    ASSERT_STREQ(
        test_context.sended_requests.data_array[0],
        "PUT https://api-jp.kii.com/thing-if/apps/app_id/targets/thing:owenr/commands/XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX/action-results HTTP/1.1\r\n"
        "host:api-jp.kii.com\r\n"
        "x-kii-appid:app_id\r\n"
        "x-kii-appkey:app_key\r\n"
        "x-kii-sdk:sn=tic;sv=0.9.6\r\n"
        "content-type:application/json\r\n"
        "authorization:bearer token\r\n"
        "content-length:137\r\n"
        "\r\n"
        "{\"actionResults\":[{\"turnPower:{\"succeeded\":true}}\",{\"setPresetTemperature:{\"succeeded\":true}}\",{\"setPresetHumidity:{\"succeeded\":true}}\"]}");

}
