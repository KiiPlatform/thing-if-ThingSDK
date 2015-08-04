#include "kii_iot.h"

#include <kii.h>

#include <stdio.h>
#include <string.h>

/* If your environment does not have assert, you must set NOASSERT define. */
#ifdef NOASSERT
  #define M_KII_IOT_ASSERT(s)
#else
  #include <assert.h>
  #define M_KII_IOT_ASSERT(s) assert(s)
#endif

#define CONST_STRLEN(str) sizeof(str) - 1
#define APP_PATH "iot-api/apps"
#define ONBOARDING_PATH "onboardings"
#define CONTENT_TYPE_VENDOR_THING_ID "application/vnd.kii.onboardingWithVendorThingIDByThing+json"

#define M_KII_IOT_APPEND_CONST_STR(kii, str)  \
    { \
        size_t size = CONST_STRLEN(str); \
        if (kii_api_call_append_body(kii, str, size) != 0) {  \
            M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n")); \
            return KII_FALSE; \
        } \
    }

#define M_KII_IOT_APPEND_STR(kii, str) \
    { \
        size_t size = strlen(str); \
        if (kii_api_call_append_body(kii, str, size) != 0) {  \
            M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n")); \
            return KII_FALSE; \
        } \
    }


#define M_KII_IOT_APPEND_STR_WITH_LEN(kii, str, size)   \
    { \
        if (kii_api_call_append_body(kii, str, size) != 0) {  \
            M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n")); \
            return KII_FALSE; \
        } \
    }

static kii_json_parse_result_t prv_kii_json_read_object(
        kii_t* kii,
        const char* json_string,
        size_t json_string_size,
        kii_json_field_t *fields)
{
    kii_json_t kii_json;
    kii_json_parse_result_t retval;
    char error_message[50];

    memset(&kii_json, 0, sizeof(kii_json));
    kii_json.error_string_buff = error_message;
    kii_json.error_string_length =
        sizeof(error_message) / sizeof(error_message[0]);

    kii_json.resource = &(kii->kii_json_resource);
    retval = kii_json_read_object(&kii_json, json_string, json_string_size,
                fields);

    if (retval != KII_JSON_PARSE_SUCCESS) {
        M_KII_LOG(kii->kii_core.logger_cb(
                "fail to parse json: result=%d, message=%s\n",
                retval, kii_json.error_string_buff));
    }
    return retval;
}


static int prv_iot_parse_onboarding_response(kii_t* kii)
{
    kii_json_field_t fields[3];

    M_KII_IOT_ASSERT(kii != NULL);

    memset(fields, 0, sizeof(fields));
    fields[0].name = "thingID";
    fields[0].type = KII_JSON_FIELD_TYPE_STRING;
    fields[0].field_copy.string = kii->kii_core.author.author_id;
    fields[0].field_copy_buff_size = sizeof(kii->kii_core.author.author_id) /
            sizeof(kii->kii_core.author.author_id[0]);
    fields[1].name = "accessToken";
    fields[1].type = KII_JSON_FIELD_TYPE_STRING;
    fields[1].field_copy.string = kii->kii_core.author.access_token;
    fields[1].field_copy_buff_size = sizeof(kii->kii_core.author.access_token) /
            sizeof(kii->kii_core.author.access_token[0]);
    fields[2].name = NULL;

    if (prv_kii_json_read_object(kii, kii->kii_core.response_body,
                    strlen(kii->kii_core.response_body), fields) !=
            KII_JSON_PARSE_SUCCESS) {
        return -1;
    }

    return 0;
}

kii_bool_t init_kii_iot(
        kii_iot_t* kii_iot,
        const char* app_id,
        const char* app_key,
        const char* app_host,
        char* mqtt_buff,
        size_t mqtt_buff_size,
        char* command_handler_buff,
        size_t command_handler_buff_size,
        char* state_updater_buff,
        size_t state_updater_buff_size,
        KII_IOT_ACTION_HANDLER action_handler)
{
    M_KII_IOT_ASSERT(kii_iot != NULL);
    M_KII_IOT_ASSERT(app_id != NULL);
    M_KII_IOT_ASSERT(app_key != NULL);
    M_KII_IOT_ASSERT(app_host != NULL);
    M_KII_IOT_ASSERT(mqtt_buff != NULL);
    M_KII_IOT_ASSERT(command_handler_buff != NULL);
    M_KII_IOT_ASSERT(state_updater_buff != NULL);

    memset(kii_iot, 0x00, sizeof(kii_iot_t));
    memset(mqtt_buff, 0x00, mqtt_buff_size);
    memset(command_handler_buff, 0x00, command_handler_buff_size);
    memset(state_updater_buff, 0x00, state_updater_buff_size);

    // Initialize command_handler
    if (kii_init(&kii_iot->command_handler, app_host, app_id, app_key) != 0) {
        return KII_FALSE;
    }
    kii_iot->command_handler.kii_core.http_context.buffer =
        command_handler_buff;
    kii_iot->command_handler.kii_core.http_context.buffer_size =
        command_handler_buff_size;

    kii_iot->command_handler.mqtt_buffer = mqtt_buff;
    kii_iot->command_handler.mqtt_buffer_size = mqtt_buff_size;

    kii_iot->command_handler.app_context = (void*)action_handler;

    // Initialize state_updater
    if (kii_init(&kii_iot->state_updater, app_host, app_id, app_key) != 0) {
        return KII_FALSE;
    }
    kii_iot->state_updater.kii_core.http_context.buffer = state_updater_buff;
    kii_iot->state_updater.kii_core.http_context.buffer_size =
        state_updater_buff_size;

    return KII_TRUE;
}

static void received_callback(kii_t* kii, char* buffer, size_t buffer_size) {
    // TODO: implement me.
    return;
}

kii_bool_t onboard_with_vendor_thing_id(
        kii_iot_t* kii_iot,
        const char* vendor_thing_id,
        const char* password,
        const char* thing_type,
        const char* thing_properties
        )
{
    kii_t* kii = &kii_iot->command_handler;
    char resource_path[64];

    if (sizeof(resource_path) / sizeof(resource_path[0]) <=
            CONST_STRLEN(APP_PATH) + CONST_STRLEN("/") +
            strlen(kii->kii_core.app_id) + CONST_STRLEN(ONBOARDING_PATH)) {
        M_KII_LOG(kii->kii_core.logger_cb(
                "resource path is longer than expected.\n"));
        return KII_FALSE;
    }
    sprintf(resource_path, "%s/%s/%s", APP_PATH, kii->kii_core.app_id,
            ONBOARDING_PATH);

    if (kii_api_call_start(kii, "POST", resource_path,
                    CONTENT_TYPE_VENDOR_THING_ID, KII_FALSE) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb(
            "fail to start api call.\n"));
    }
    M_KII_IOT_APPEND_CONST_STR(kii, "{\"vendorThingID\":\"");
    M_KII_IOT_APPEND_STR(kii, vendor_thing_id);
    M_KII_IOT_APPEND_CONST_STR(kii, "\",\"thingPassword\":\"");
    M_KII_IOT_APPEND_STR(kii, password);
    M_KII_IOT_APPEND_CONST_STR(kii, "\"");
    if (thing_type != NULL) {
        M_KII_IOT_APPEND_CONST_STR(kii, ",\"thingType\":\"");
        M_KII_IOT_APPEND_STR(kii, thing_type);
        M_KII_IOT_APPEND_CONST_STR(kii, "\"");
    }
    if (thing_properties != NULL) {
        M_KII_IOT_APPEND_CONST_STR(kii, ",\"thingProperties\":\"");
        M_KII_IOT_APPEND_STR(kii, thing_properties);
        M_KII_IOT_APPEND_CONST_STR(kii, "\"");
    }
    M_KII_IOT_APPEND_CONST_STR(kii, "}");

    if (kii_api_call_run(kii) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("fail to run api.\n"));
        return KII_FALSE;
    }

    if (prv_iot_parse_onboarding_response(kii) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("fail to parse resonse.\n"));
        return KII_FALSE;
    }

    if (kii_push_start_routine(kii, 0, 0, received_callback) != 0) {
        return KII_FALSE;
    }

    return KII_TRUE;
}

kii_bool_t onboard_with_thing_id(
        kii_iot_t* kii_iot,
        const char* thing_id,
        const char* password
        )
{
    // TODO: implement me.
    return KII_FALSE;
}
