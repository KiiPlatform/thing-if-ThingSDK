#include "kii_thing_if_environment_test.h"

#include <kii_thing_if.h>
#include "kii_thing_if_test.h"

#include <stdio.h>
#include <string.h>

#include <gtest/gtest.h>

typedef struct test_context_t {
    int num;
    char** sended_requests;
    const char** receiving_requests;
    int receiving_count;
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
    return (*writer)(kii, "{\"power\":true}");
}

static kii_socket_code_t test_connect(
        void* context,
        const char* host,
        unsigned int port)
{
    test_context_t* test_context = (test_context_t*)context;
    int num = test_context->num + 1;
    char** sended_requests = (char**)realloc(
            test_context->sended_requests,
            sizeof(char**) * num);
    char* data = (char*)malloc(sizeof(char));
    data[0] = '\0';
    sended_requests[num - 1] = data;
    test_context->num = num;
    test_context->sended_requests = sended_requests;
    test_context->receiving_count = 0;
    return KII_SOCKETC_OK;
}

static kii_socket_code_t test_send(
        void* context,
        const char* buffer,
        size_t length)
{
    test_context_t* test_context = (test_context_t*)context;
    int index = test_context->num - 1;
    test_context->sended_requests[index] = (char*)realloc(
            test_context->sended_requests[index],
            strlen(test_context->sended_requests[index]) + length + 1);
    strncat(test_context->sended_requests[index], buffer, length);

    return KII_SOCKETC_OK;
}

static kii_socket_code_t test_recv(
        void* context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length)
{
    test_context_t* test_context = (test_context_t*)context;
    int index = test_context->num - 1;
    const char* receiving_requests = test_context->receiving_requests[index];
    int start_index = test_context->receiving_count;
    int actual_read_length = length_to_read;
    int rest_length = strlen(receiving_requests) - start_index;

    if (actual_read_length > rest_length) {
        actual_read_length = rest_length;
    }

    memcpy(buffer, receiving_requests + start_index, actual_read_length);
    test_context->receiving_count += actual_read_length;
    *out_actual_length = actual_read_length;

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
    const char* receiving_requests[2];
    receiving_requests[0] = "HTTP/1.1 201\r\n\r\n";
    receiving_requests[1] = "HTTP/1.1 201\r\n\r\n";

    memset(&socket_test_context, 0, sizeof(socket_test_context));
    memset(&test_context, 0, sizeof(test_context));

    socket_test_context.CONNECT = test_connect;
    socket_test_context.SEND = test_send;
    socket_test_context.RECV = test_recv;
    socket_test_context.CLOSE = test_close;
    socket_test_context.context = &test_context;
    test_context.receiving_requests = receiving_requests;

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

    kii_thing_if_system_cb_t sys_cb;
    sys_cb.task_create_cb = task_create_cb_impl;
    sys_cb.delay_ms_cb = delay_ms_cb_impl;
    sys_cb.delay_ms_cb = delay_ms_cb_impl;
    sys_cb.log_cb = logger_cb_impl;
    sys_cb.socket_connect_cb = socket_connect_cb_impl;
    sys_cb.socket_send_cb = socket_send_cb_impl;
    sys_cb.socket_recv_cb = socket_recv_cb_impl;
    sys_cb.socket_close_cb = socket_close_cb_impl;
    sys_cb.mqtt_socket_connect_cb = mqtt_connect_cb_impl;
    sys_cb.mqtt_socket_send_cb = mqtt_send_cb_impl;
    sys_cb.mqtt_socket_recv_cb = mqtt_recv_cb_impl;
    sys_cb.mqtt_socket_close_cb = mqtt_close_cb_impl;

    ASSERT_EQ(
        KII_TRUE,
        init_kii_thing_if(
            &kii_thing_if,
            "app_id",
            "app_key",
            "JP",
            &command_handler_resource,
            &state_updater_resource,
            &sys_cb,
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
        test_context.sended_requests[0],
        "PUT https://api-jp.kii.com/thing-if/apps/app_id/targets/thing:owenr/commands/XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX/action-results HTTP/1.0\r\n"
        "host:api-jp.kii.com\r\n"
        "x-kii-appid:app_id\r\n"
        "x-kii-appkey:app_key\r\n"
        "x-kii-sdk:sn=tic;sv=1.0.1\r\n"
        "content-type:application/json\r\n"
        "authorization:bearer token\r\n"
        "content-length:137\r\n"
        "\r\n"
        "{\"actionResults\":[{\"turnPower\":{\"succeeded\":true}},{\"setPresetTemperature\":{\"succeeded\":true}},{\"setPresetHumidity\":{\"succeeded\":true}}]}");
    ASSERT_STREQ(
        test_context.sended_requests[1],
        "PUT https://api-jp.kii.com/thing-if/apps/app_id/targets/thing:owenr/states HTTP/1.0\r\n"
        "host:api-jp.kii.com\r\n"
        "x-kii-appid:app_id\r\n"
        "x-kii-appkey:app_key\r\n"
        "x-kii-sdk:sn=tic;sv=1.0.1\r\n"
        "content-type:application/vnd.kii.MultipleTraitState+json\r\n"
        "authorization:bearer token\r\n"
        "content-length:14\r\n"
        "\r\n"
        "{\"power\":true}");
    for (i = 0; i < test_context.num; ++i) {
        free(test_context.sended_requests[i]);
    }
    free(test_context.sended_requests);
}
