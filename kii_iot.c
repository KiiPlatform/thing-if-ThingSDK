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

#define EVAL(f, v) f(v)
#define TOSTR(s) #s
#define ULONG_MAX_STR EVAL(TOSTR, ULONG_MAX)
#define ULONGBUFSIZE (sizeof(ULONG_MAX_STR) / sizeof(char))

#define CONST_STRLEN(str) sizeof(str) - 1
#define APP_PATH "api/apps"
#define OAUTH_PATH "oauth2/token"
#define IOT_APP_PATH "iot-api/apps/"
#define ONBOARDING_PATH "/onboardings"
#define TARGET_PART "/targets/thing:"
#define COMMAND_PART "/commands/"
#define RESULTS_PART "/action-results"
#define CONTENT_TYPE_VENDOR_THING_ID "application/vnd.kii.OnboardingWithVendorThingIDByThing+json"
#define CONTENT_TYPE_THING_ID "application/vnd.kii.OnboardingWithThingIDByThing+json"

static kii_json_parse_result_t prv_kii_iot_json_read_object(
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

    switch (retval) {
        case KII_JSON_PARSE_SUCCESS:
        case KII_JSON_PARSE_PARTIAL_SUCCESS:
            break;
        default:
            M_KII_LOG(kii->kii_core.logger_cb(
                    "fail to parse json: result=%d, message=%s\n",
                    retval, kii_json.error_string_buff));
            break;
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

    if (prv_kii_iot_json_read_object(kii, kii->kii_core.response_body,
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
        kii_json_resource_t* command_handler_json_resource,
        char* state_updater_buff,
        size_t state_updater_buff_size,
        kii_json_resource_t* state_updater_json_resource,
        int state_update_period,
        KII_IOT_ACTION_HANDLER action_handler,
        KII_IOT_STATE_HANDLER state_handler,
        void* app_context)
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

    kii_iot->action_handler = action_handler;

    kii_iot->command_handler.app_context = (void*)kii_iot;

    // Initialize state_updater
    if (kii_init(&kii_iot->state_updater, app_host, app_id, app_key) != 0) {
        return KII_FALSE;
    }
    kii_iot->state_updater.kii_core.http_context.buffer = state_updater_buff;
    kii_iot->state_updater.kii_core.http_context.buffer_size =
        state_updater_buff_size;

    kii_iot->state_handler = state_handler;

    kii_iot->state_updater.app_context = (void*)kii_iot;

    return KII_TRUE;
}

static int prv_kii_iot_get_key_and_value_from_json(
        kii_t* kii,
        const char* json_string,
        size_t json_string_len,
        char** out_key,
        char** out_value,
        size_t* out_key_len,
        size_t* out_value_len)
{
    jsmn_parser parser;
    int parse_result = JSMN_ERROR_NOMEM;
#ifdef KII_JSON_FIXED_TOKEN_NUM
    jsmntok_t tokens[KII_JSON_FIXED_TOKEN_NUM];
    size_t tokens_num = sizeof(tokens) / sizeof(tokens[0]);
#else
    jsmntok_t* tokens = kii->kii_json_resource.tokens;
    size_t tokens_num = kii->kii_json_resource.tokens_num;
#endif

    jsmn_init(&parser);

    parse_result = jsmn_parse(&parser, json_string, json_string_len, tokens,
            tokens_num);
    if (parse_result >= 0) {
        if (tokens[0].type != JSMN_OBJECT) {
            M_KII_LOG(kii->kii_core.logger_cb("action must be json object.\n"));
            return -1;
        }
        if (tokens[1].type != JSMN_STRING) {
            M_KII_LOG(kii->kii_core.logger_cb("invalid json object.\n"));
            return -1;
        }
        *out_key = (char*)(json_string + tokens[1].start);
        *out_key_len = tokens[1].end - tokens[1].start;
        *out_value = (char*)(json_string + tokens[2].start);
        *out_value_len = tokens[2].end - tokens[2].start;
        return 0;
    } else if (parse_result == JSMN_ERROR_NOMEM) {
        M_KII_LOG(kii->kii_core.logger_cb(
                "Not enough tokens were provided.\n"));
        return -1;
    } else if (parse_result == JSMN_ERROR_INVAL) {
        M_KII_LOG(kii->kii_core.logger_cb(
                "Invalid character inside JSON string.\n"));
        return -1;
    } else if (parse_result == JSMN_ERROR_PART) {
        M_KII_LOG(kii->kii_core.logger_cb(
                "The string is not a full JSON packet, more bytes expected.\n"));
        return -1;
    } else {
        M_KII_LOG(kii->kii_core.logger_cb("Unexpected error.\n"));
        return -1;
    }
}

static void received_callback(kii_t* kii, char* buffer, size_t buffer_size) {
    kii_json_field_t fields[6];
    kii_json_field_t action[2];
    const char* schema = NULL;
    int schema_version = 0;
    char* actions_str = NULL;
    size_t actions_len = 0;
    char index[ULONGBUFSIZE];
    size_t i = 0;
    char resource_path[256];

    memset(fields, 0x00, sizeof(fields));
    fields[0].path = "/schema";
    fields[0].type = KII_JSON_FIELD_TYPE_STRING;
    fields[0].field_copy.string = NULL;
    fields[1].path = "/schemaVersion";
    fields[1].type = KII_JSON_FIELD_TYPE_INTEGER;
    fields[1].field_copy.string = NULL;
    fields[2].path = "/commandID";
    fields[2].type = KII_JSON_FIELD_TYPE_STRING;
    fields[2].field_copy.string = NULL;
    fields[3].path = "/actions";
    fields[3].type = KII_JSON_FIELD_TYPE_ARRAY;
    fields[3].field_copy.string = NULL;
    fields[4].path = "/when";
    fields[4].type = KII_JSON_FIELD_TYPE_LONG;
    fields[5].path = NULL;

    switch(prv_kii_iot_json_read_object(kii, buffer, buffer_size, fields)) {
        case KII_JSON_PARSE_SUCCESS:
        case KII_JSON_PARSE_PARTIAL_SUCCESS:
            break;
        default:
            M_KII_LOG(kii->kii_core.logger_cb(
                    "fail to parse received message.\n"));
            return;
    }

    if (sizeof(resource_path) / sizeof(resource_path[0]) <=
            CONST_STRLEN(IOT_APP_PATH) +
            strlen(kii->kii_core.app_id) + CONST_STRLEN(TARGET_PART) +
            strlen(kii->kii_core.author.author_id) +
            CONST_STRLEN(COMMAND_PART) + (fields[2].end - fields[2].start - 1) +
            CONST_STRLEN(RESULTS_PART)) {
        M_KII_LOG(kii->kii_core.logger_cb(
                "resource path is longer than expected.\n"));
        return;
    }

    resource_path[0] = '\0';
    strcat(resource_path, IOT_APP_PATH);
    strcat(resource_path, kii->kii_core.app_id);
    strcat(resource_path, TARGET_PART);
    strcat(resource_path, kii->kii_core.author.author_id);
    strcat(resource_path, COMMAND_PART);
    strncat(resource_path, buffer + fields[2].start,
            fields[2].end - fields[2].start);
    strcat(resource_path, RESULTS_PART);
    // TODO: Check properties.

    if (kii_api_call_start(kii, "PUT", resource_path, "application/json",
                    KII_TRUE) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("fail to start api call.\n"));
        return;
    }
    if (kii_api_call_append_body(kii, "{\"actionResults\":[",
                    sizeof("{\"actionResults\":[") - 1) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
    }

    schema = buffer + fields[0].start;
    buffer[fields[0].end] = '\0';
    schema_version = fields[1].field_copy.int_value;
    actions_str = buffer + fields[3].start;
    actions_len = fields[3].end - fields[3].start;
    memset(action, 0x00, sizeof(action));
    action[0].path = index;
    action[0].type = KII_JSON_FIELD_TYPE_OBJECT;
    action[0].field_copy.string = NULL;
    action[0].result = KII_JSON_FIELD_PARSE_SUCCESS;
    action[1].path = NULL;
    for (i = 0; action[0].result == KII_JSON_FIELD_PARSE_SUCCESS; ++i) {
        sprintf(index, "/[%lu]", i);
        switch (prv_kii_iot_json_read_object(kii, actions_str, actions_len,
                        action)) {
            case KII_JSON_PARSE_SUCCESS:
            {
                KII_IOT_ACTION_HANDLER handler =
                    ((kii_iot_t*)kii->app_context)->action_handler;
                void* app_context = ((kii_iot_t*)kii->app_context)->app_context;
                char* key;
                char* value;
                size_t key_len, value_len;
                char key_swap, value_swap;
                char error[EMESSAGE_SIZE + 1];
                if (i >= 1) {
                    if (kii_api_call_append_body(kii, ",", sizeof(",") - 1)
                            != 0) {
                        M_KII_LOG(kii->kii_core.logger_cb(
                                "request size overflowed.\n"));
                        return;
                    }
                }
                if (prv_kii_iot_get_key_and_value_from_json(kii,
                            actions_str + action[0].start,
                            action[0].end - action[0].start, &key, &value,
                            &key_len, &value_len) != 0) {
                    *(actions_str + action[0].end) = '\0';
                    M_KII_LOG(kii->kii_core.logger_cb(
                            "fail to parse action: %s.\n",
                            actions_str + action[0].start));
                    return;
                }
                key_swap = key[key_len];
                value_swap = value[value_len];
                key[key_len] = '\0';
                value[value_len] = '\0';
                if ((*handler)(app_context, schema, schema_version, key, value,
                                error) != KII_FALSE) {
                    if (kii_api_call_append_body(kii,
                                    "{\"", sizeof("{\"") - 1) != 0) {
                        M_KII_LOG(kii->kii_core.logger_cb(
                                "request size overflowed.\n"));
                        return;
                    }
                    if (kii_api_call_append_body(kii, key, strlen(key)) != 0) {
                        M_KII_LOG(kii->kii_core.logger_cb(
                                "request size overflowed.\n"));
                        return;
                    }
                    if (kii_api_call_append_body(kii, "\":{\"result\":true}}",
                                    sizeof("\":{\"result\":true}}") - 1) != 0) {
                        M_KII_LOG(kii->kii_core.logger_cb(
                                "request size overflowed.\n"));
                        return;
                    }
                } else {
                    if (kii_api_call_append_body(kii,
                                    "{\"", sizeof("{\"") - 1) != 0) {
                        M_KII_LOG(kii->kii_core.logger_cb(
                                "request size overflowed.\n"));
                        return;
                    }
                    if (kii_api_call_append_body(kii, key, strlen(key)) != 0) {
                        M_KII_LOG(kii->kii_core.logger_cb(
                                "request size overflowed.\n"));
                        return;
                    }
                    if (kii_api_call_append_body(kii,
                                    "\":{\"result\":false}, \"errorMessage\":\"",
                                    sizeof("\":{\"result\":false}, \"errorMessage\":\"") - 1)
                            != 0) {
                        M_KII_LOG(kii->kii_core.logger_cb(
                                "request size overflowed.\n"));
                        return;
                    }
                    if (kii_api_call_append_body(kii, error, strlen(error))
                            != 0) {
                        M_KII_LOG(kii->kii_core.logger_cb(
                                "request size overflowed.\n"));
                        return;
                    }
                    if (kii_api_call_append_body(kii,
                                    "\"}}", sizeof("\"}}") - 1) != 0) {
                        M_KII_LOG(kii->kii_core.logger_cb(
                                "request size overflowed.\n"));
                        return;
                    }
                }
                key[key_len] = key_swap;
                value[value_len] = value_swap;
            }
            case KII_JSON_PARSE_PARTIAL_SUCCESS:
                // This must be end of array.
                break;
            case KII_JSON_PARSE_ROOT_TYPE_ERROR:
            case KII_JSON_PARSE_INVALID_INPUT:
            default:
                M_KII_LOG(kii->kii_core.logger_cb("unexpected error.\n"));
                return ;
        }
    }

    if (kii_api_call_append_body(kii, "]}", sizeof("]}") - 1) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return;
    }
    if (kii_api_call_run(kii) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("fail to run api.\n"));
        return;
    }

    return;
}

static int prv_kii_iot_get_anonymous_token(kii_t* kii)
{
    char resource_path[64];
    kii_json_field_t fields[2];

    M_KII_IOT_ASSERT(kii);

    if (sizeof(resource_path) / sizeof(resource_path[0]) <=
            CONST_STRLEN(APP_PATH) + CONST_STRLEN("/") +
            strlen(kii->kii_core.app_id) + CONST_STRLEN(OAUTH_PATH)) {
        M_KII_LOG(kii->kii_core.logger_cb(
                "resource path is longer than expected.\n"));
        return -1;
    }
    sprintf(resource_path, "%s/%s/%s", APP_PATH, kii->kii_core.app_id,
            OAUTH_PATH);

    if (kii_api_call_start(kii, "POST", resource_path, "application/json",
                    KII_FALSE) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("fail to start api call.\n"));
    }

    if (kii_api_call_append_body(kii,
                    "{\"grant_type\":\"client_credentials\",\"client_id\":\"",
                    CONST_STRLEN(
                        "{\"grant_type\":\"client_credentials\",\"client_id\":\""))
            != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return -1;
    }
    if (kii_api_call_append_body(kii, kii->kii_core.app_id,
                    strlen(kii->kii_core.app_id)) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return -1;
    }
    if (kii_api_call_append_body(kii, "\",\"client_secret\": \"",
                    CONST_STRLEN("\",\"client_secret\": \"")) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return -1;
    }
    if (kii_api_call_append_body(kii, kii->kii_core.app_key,
                    strlen(kii->kii_core.app_key)) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return -1;
    }
    if (kii_api_call_append_body(kii, "\"}", CONST_STRLEN("\"}")) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return -1;
    }
    if (kii_api_call_run(kii) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("fail to run api.\n"));
        return -1;
    }

    memset(fields, 0x00, sizeof(fields));
    fields[0].path = "/access_token";
    fields[0].type = KII_JSON_FIELD_TYPE_STRING;
    fields[0].field_copy.string = kii->kii_core.author.access_token;
    fields[0].field_copy_buff_size = sizeof(kii->kii_core.author.access_token) /
            sizeof(kii->kii_core.author.access_token[0]);
    fields[1].path = NULL;

    if (prv_kii_iot_json_read_object(kii, kii->kii_core.response_body,
                    strlen(kii->kii_core.response_body), fields)
            != KII_JSON_PARSE_SUCCESS) {
        M_KII_LOG(kii->kii_core.logger_cb("fail to parse received message.\n"));
        return -1;
    }

    return 0;
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

    if (prv_kii_iot_get_anonymous_token(kii) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("fail to get anonymous token.\n"));
        return KII_FALSE;
    }

    if (sizeof(resource_path) / sizeof(resource_path[0]) <=
            CONST_STRLEN(IOT_APP_PATH) +
            strlen(kii->kii_core.app_id) + CONST_STRLEN(ONBOARDING_PATH)) {
        M_KII_LOG(kii->kii_core.logger_cb(
                "resource path is longer than expected.\n"));
        return KII_FALSE;
    }
    sprintf(resource_path, "%s%s%s", IOT_APP_PATH, kii->kii_core.app_id,
            ONBOARDING_PATH);

    if (kii_api_call_start(kii, "POST", resource_path,
                    CONTENT_TYPE_VENDOR_THING_ID, KII_TRUE) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("fail to start api call.\n"));
    }

    if (kii_api_call_append_body(kii, "{\"vendorThingID\":\"",
                    CONST_STRLEN("{\"vendorThingID\":\"")) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return KII_FALSE;
    }
    if (kii_api_call_append_body(kii, vendor_thing_id,
                    strlen(vendor_thing_id)) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return KII_FALSE;
    }
    if (kii_api_call_append_body(kii, "\",\"thingPassword\":\"",
                    CONST_STRLEN("\",\"thingPassword\":\"")) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return KII_FALSE;
    }
    if (kii_api_call_append_body(kii, password, strlen(password)) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return KII_FALSE;
    }
    if (kii_api_call_append_body(kii, "\"", CONST_STRLEN("\"")) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return KII_FALSE;
    }

    if (thing_type != NULL) {
        if (kii_api_call_append_body(kii, ",\"thingType\":\"",
                        CONST_STRLEN(",\"thingType\":\"")) != 0) {
            M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
            return KII_FALSE;
        }
        if (kii_api_call_append_body(kii, thing_type, strlen(thing_type))
                != 0) {
            M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
            return KII_FALSE;
        }
        if (kii_api_call_append_body(kii, "\"", CONST_STRLEN("\"")) != 0) {
            M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
            return KII_FALSE;
        }
    }
    if (thing_properties != NULL) {
        if (kii_api_call_append_body(kii, ",\"thingProperties\":\"",
                        CONST_STRLEN(",\"thingProperties\":\"")) != 0) {
            M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
            return KII_FALSE;
        }
        if (kii_api_call_append_body(kii, thing_properties,
                        strlen(thing_properties)) != 0) {
            M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
            return KII_FALSE;
        }
        if (kii_api_call_append_body(kii, "\"", CONST_STRLEN("\"")) != 0) {
            M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
            return KII_FALSE;
        }
    }
    if (kii_api_call_append_body(kii, "}", CONST_STRLEN("}")) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return KII_FALSE;
    }

    if (kii_api_call_run(kii) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("fail to run api.\n"));
        return KII_FALSE;
    }

    if (prv_iot_parse_onboarding_response(kii) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("fail to parse resonse.\n"));
        return KII_FALSE;
    }

    if (kii_push_start_routine(kii, 0, 0, received_callback) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("fail to start routine.\n"));
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
    kii_t* kii = &kii_iot->command_handler;
    char resource_path[64];

    if (prv_kii_iot_get_anonymous_token(kii) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("fail to get anonymous token.\n"));
        return KII_FALSE;
    }

    if (sizeof(resource_path) / sizeof(resource_path[0]) <=
            CONST_STRLEN(IOT_APP_PATH) +
            strlen(kii->kii_core.app_id) + CONST_STRLEN(ONBOARDING_PATH)) {
        M_KII_LOG(kii->kii_core.logger_cb(
                "resource path is longer than expected.\n"));
        return KII_FALSE;
    }
    sprintf(resource_path, "%s%s%s", IOT_APP_PATH, kii->kii_core.app_id,
            ONBOARDING_PATH);

    if (kii_api_call_start(kii, "POST", resource_path, CONTENT_TYPE_THING_ID,
                    KII_TRUE) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb(
            "fail to start api call.\n"));
    }

    if (kii_api_call_append_body(kii, "{\"thingID\":\"",
                    CONST_STRLEN("{\"thingID\":\"")) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return KII_FALSE;
    }
    if (kii_api_call_append_body(kii, thing_id, strlen(thing_id)) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return KII_FALSE;
    }
    if (kii_api_call_append_body(kii, "\",\"thingPassword\":\"",
                    CONST_STRLEN("\",\"thingPassword\":\"")) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return KII_FALSE;
    }
    if (kii_api_call_append_body(kii, password, strlen(password)) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return KII_FALSE;
    }
    if (kii_api_call_append_body(kii, "\"}", CONST_STRLEN("\"}")) != 0) {
        M_KII_LOG(kii->kii_core.logger_cb("request size overflowed.\n"));
        return KII_FALSE;
    }

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
