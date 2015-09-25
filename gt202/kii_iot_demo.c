#include "kii_iot_demo.h"

#include <kii_iot.h>
#include <kii_json.h>

#include <string.h>
#include <stdio.h>

#include "main.h"

const char EX_APP_ID[] = "00959619";
const char EX_APP_KEY[] = "1ae0bcded44365e4f83c2daa2f4ca237";
const char EX_APP_SITE[] = "api-development-jp.internal.kii.com";

const char EX_AUTH_VENDOR_ID[] = "464939";
const char EX_AUTH_VENDOR_PASS[] = "1234";

const char EX_AUTH_THING_ID[] = "th.53ae324be5a0-f5e8-5e11-fdd5-000ee263";
const char EX_ACCESS_TOKEN[] = "7H2sc6UYvC2mKCFihYhA52xh1b--G3rga_OYz_gbCg4";

#define EX_COMMAND_HANDLER_BUFF_SIZE 4096
#define EX_STATE_UPDATER_BUFF_SIZE 4096
#define EX_MQTT_BUFF_SIZE 2048
#define EX_STATE_UPDATE_PERIOD 60

typedef struct prv_smartlight_t {
    kii_json_boolean_t power;
    int brightness;
    int color[3];
    int color_temperature;
} prv_smartlight_t;

static prv_smartlight_t m_smartlight;

static kii_json_parse_result_t prv_json_read_object(
        const char* json,
        size_t json_len,
        kii_json_field_t* fields,
        char error[EMESSAGE_SIZE + 1])
{
    kii_json_t kii_json;
    kii_json_resource_t* resource_pointer = NULL;
#ifndef KII_JSON_FIXED_TOKEN_NUM
    kii_json_resource_t resource;
    kii_json_token_t tokens[32];
    resource_pointer = &resource;
    resource.tokens = tokens;
    resource.tokens_num = sizeof(tokens) / sizeof(tokens[0]);
#endif

    memset(&kii_json, 0, sizeof(kii_json));
    kii_json.resource = resource_pointer;
    kii_json.error_string_buff = error;
    kii_json.error_string_length = EMESSAGE_SIZE + 1;

    return kii_json_read_object(&kii_json, json, json_len, fields);
}

static kii_bool_t action_handler(
        const char* schema,
        int schema_version,
        const char* action_name,
        const char* action_params,
        char error[EMESSAGE_SIZE + 1])
{
    printf("schema=%s, schema_version=%d, action name=%s, action params=%s\n",
            schema, schema_version, action_name, action_params);

    if (strcmp(schema, "SmartLightDemo") != 0 && schema_version != 1) {
        printf("invalid schema: %s %d\n", schema, schema_version);
        return KII_FALSE;
    }

    if (strcmp(action_name, "turnPower") == 0) {
        kii_json_field_t fields[2];

        memset(fields, 0x00, sizeof(fields));
        fields[0].path = "/power";
        fields[0].type = KII_JSON_FIELD_TYPE_BOOLEAN;
        fields[1].path = NULL;
        if(prv_json_read_object(action_params, strlen(action_params),
                        fields, error) !=  KII_JSON_PARSE_SUCCESS) {
            printf("invalid turnPower json\n");
            return KII_FALSE;
        }
        m_smartlight.power = fields[0].field_copy.boolean_value;
        return KII_TRUE;
    } else if (strcmp(action_name, "setBrightness") == 0) {
        kii_json_field_t fields[2];

        memset(fields, 0x00, sizeof(fields));
        fields[0].path = "/brightness";
        fields[0].type = KII_JSON_FIELD_TYPE_INTEGER;
        fields[1].path = NULL;
        if(prv_json_read_object(action_params, strlen(action_params),
                        fields, error) !=  KII_JSON_PARSE_SUCCESS) {
            printf("invalid brightness json\n");
            return KII_FALSE;
        }
        m_smartlight.brightness = fields[0].field_copy.int_value;
        return KII_TRUE;
    } else if (strcmp(action_name, "setColor") == 0) {
        kii_json_field_t fields[4];

        memset(fields, 0x00, sizeof(fields));
        fields[0].path = "/color/[0]";
        fields[0].type = KII_JSON_FIELD_TYPE_INTEGER;
        fields[1].path = "/color/[1]";
        fields[1].type = KII_JSON_FIELD_TYPE_INTEGER;
        fields[2].path = "/color/[2]";
        fields[2].type = KII_JSON_FIELD_TYPE_INTEGER;
        fields[3].path = NULL;
        if(prv_json_read_object(action_params, strlen(action_params),
                         fields, error) !=  KII_JSON_PARSE_SUCCESS) {
            printf("invalid color json\n");
            return KII_FALSE;
        }
        m_smartlight.color[0] = fields[0].field_copy.int_value;
        m_smartlight.color[1] = fields[1].field_copy.int_value;
        m_smartlight.color[2] = fields[2].field_copy.int_value;
        return KII_TRUE;
    } else if (strcmp(action_name, "setColorTemperature") == 0) {
        kii_json_field_t fields[2];

        memset(fields, 0x00, sizeof(fields));
        fields[0].path = "/colorTemperature";
        fields[0].type = KII_JSON_FIELD_TYPE_INTEGER;
        fields[1].path = NULL;
        if(prv_json_read_object(action_params, strlen(action_params),
                        fields, error) !=  KII_JSON_PARSE_SUCCESS) {
            printf("invalid colorTemperature json\n");
            return KII_FALSE;
        }
        m_smartlight.color_temperature = fields[0].field_copy.int_value;
        return KII_TRUE;
    } else {
        printf("invalid action: %s\n", action_name);
        return KII_FALSE;
    }
}

static kii_bool_t state_handler(
        kii_t* kii,
        KII_IOT_WRITER writer)
{
    char buf[256];
    if ((*writer)(kii, "{\"power\":") == KII_FALSE) {
        return KII_FALSE;
    }
    if ((*writer)(kii, m_smartlight.power == KII_JSON_TRUE
                    ? "true," : "false,") == KII_FALSE) {
        return KII_FALSE;
    }
    if ((*writer)(kii, "\"brightness\":") == KII_FALSE) {
        return KII_FALSE;
    }

    sprintf(buf, "%d,", m_smartlight.brightness);
    if ((*writer)(kii, buf) == KII_FALSE) {
        return KII_FALSE;
    }

    if ((*writer)(kii, "\"color\":") == KII_FALSE) {
        return KII_FALSE;
    }
    sprintf(buf, "[%d,%d,%d],", m_smartlight.color[0],
            m_smartlight.color[1], m_smartlight.color[2]);
    if ((*writer)(kii, buf) == KII_FALSE) {
        return KII_FALSE;
    }

    if ((*writer)(kii, "\"colorTemperature\":") == KII_FALSE) {
        return KII_FALSE;
    }
    sprintf(buf, "%d}", m_smartlight.color_temperature);
    if ((*writer)(kii, buf) == KII_FALSE) {
        return KII_FALSE;
    }
    return KII_TRUE;
}

int json_resource_cb(
        kii_json_resource_t* resource,
        size_t required_size)
{
    resource->tokens = (kii_json_token_t *)malloc(
        sizeof(kii_json_token_t) * required_size);
    resource->tokens_num = required_size;

    return 1;
}

#define CMD_INDEX 1

int kii_iot_main(int argc, char** argv)
{
    kii_iot_command_handler_resource_t command_handler_resource;
    kii_iot_state_updater_resource_t state_updater_resource;
    kii_iot_t* kii_iot;

    kii_iot = malloc(sizeof(kii_iot_t));
    memset(kii_iot, 0x00, sizeof(kii_iot_t));

    command_handler_resource.buffer = malloc(sizeof(char) * EX_COMMAND_HANDLER_BUFF_SIZE);
    command_handler_resource.buffer_size = EX_COMMAND_HANDLER_BUFF_SIZE;
    command_handler_resource.mqtt_buffer = malloc(sizeof(char) * EX_MQTT_BUFF_SIZE);
    command_handler_resource.mqtt_buffer_size = EX_MQTT_BUFF_SIZE;
    command_handler_resource.action_handler = action_handler;

    state_updater_resource.buffer = malloc(sizeof(char) * EX_STATE_UPDATER_BUFF_SIZE);
    state_updater_resource.buffer_size = EX_STATE_UPDATER_BUFF_SIZE;
    state_updater_resource.period = EX_STATE_UPDATE_PERIOD;
    state_updater_resource.state_handler = state_handler;

    if (ATH_STRCMP(argv[CMD_INDEX], "vendor-thing-id") == 0)
    {
        init_kii_iot(kii_iot, EX_APP_ID, EX_APP_KEY, EX_APP_SITE,
                &command_handler_resource, &state_updater_resource,
                json_resource_cb);
        onboard_with_vendor_thing_id(kii_iot, EX_AUTH_VENDOR_ID,
                EX_AUTH_VENDOR_PASS, NULL, NULL);
    }
    else if(ATH_STRCMP(argv[CMD_INDEX], "thing-id") == 0)
    {
        init_kii_iot(kii_iot, EX_APP_ID, EX_APP_KEY, EX_APP_SITE,
                &command_handler_resource, &state_updater_resource,
                json_resource_cb);
        onboard_with_thing_id(kii_iot, EX_AUTH_THING_ID,
                EX_AUTH_VENDOR_PASS);
    }
    else if(ATH_STRCMP(argv[CMD_INDEX], "onboarded") == 0)
    {
        init_kii_iot_with_onboarded_thing(kii_iot, EX_APP_ID, EX_APP_KEY,
                EX_APP_SITE, EX_AUTH_THING_ID, EX_ACCESS_TOKEN,
                &command_handler_resource, &state_updater_resource,
                json_resource_cb);
    }
    else
    {
        printf("to configure parameters, edit example.h\n\n");
        printf("commands: \n");
        printf("  vendor-thing-id\n onboard to iot cloud with vendor thing ID.\n");
        printf("  thing-id\n onboard to iot cloud with thing ID.\n");
        printf("  onboarded\n join to onboarded iot cloud with thing ID and access token.\n");
        printf("  help\n show this help.\n");
    }

    return 0;
}
