#include "example.h"

#include <kii_thing_if.h>
#include <kii_json.h>

#include <string.h>
#include <stdio.h>
#include <getopt.h>

typedef struct prv_smartlight_t {
    kii_json_boolean_t power;
    int brightness;
    int color[3];
    int color_temperature;
} prv_smartlight_t;

static prv_smartlight_t m_smartlight;

static prv_json_read_object(
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
        KII_THING_IF_WRITER writer)
{
    FILE* fp = fopen("smartlight-state.json", "r");
    if (fp != NULL) {
        char buf[256];
        kii_bool_t retval = KII_TRUE;
        while (fgets(buf, sizeof(buf) / sizeof(buf[0]), fp) != NULL) {
            if ((*writer)(kii, buf) == KII_FALSE) {
                retval = KII_FALSE;
                break;
            }
        }
        fclose(fp);
        return retval;
    } else {
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
}

int main(int argc, char** argv)
{
    kii_thing_if_command_handler_resource_t command_handler_resource;
    kii_thing_if_state_updater_resource_t state_updater_resource;
    char command_handler_buff[EX_COMMAND_HANDLER_BUFF_SIZE];
    char state_updater_buff[EX_STATE_UPDATER_BUFF_SIZE];
    char mqtt_buff[EX_MQTT_BUFF_SIZE];
    kii_thing_if_t kii_thing_if;
    int option_index = 0;
    struct option long_options[] = {
        {"vendor-thing-id", no_argument, NULL, 0},
        {"thing-id", no_argument, NULL, 1},
        {"onboarded", no_argument, NULL, 2},
        {"help", no_argument, NULL, 3},
        {0, 0, 0, 0}
    };

    command_handler_resource.buffer = command_handler_buff;
    command_handler_resource.buffer_size =
        sizeof(command_handler_buff) / sizeof(command_handler_buff[0]);
    command_handler_resource.mqtt_buffer = mqtt_buff;
    command_handler_resource.mqtt_buffer_size =
        sizeof(mqtt_buff) / sizeof(mqtt_buff[0]);
    command_handler_resource.action_handler = action_handler;

    state_updater_resource.buffer = state_updater_buff;
    state_updater_resource.buffer_size =
        sizeof(state_updater_buff) / sizeof(state_updater_buff[0]);
    state_updater_resource.period = EX_STATE_UPDATE_PERIOD;
    state_updater_resource.state_handler = state_handler;

    switch (getopt_long(argc, argv, "", long_options, &option_index)) {
        case 0:
            init_kii_thing_if(&kii_thing_if, EX_APP_ID, EX_APP_KEY, EX_APP_SITE,
                    &command_handler_resource, &state_updater_resource, NULL);
            onboard_with_vendor_thing_id(&kii_thing_if, EX_AUTH_VENDOR_ID,
                    EX_AUTH_VENDOR_PASS, NULL, NULL);
            while (1) {}
            break;
        case 1:
            init_kii_thing_if(&kii_thing_if, EX_APP_ID, EX_APP_KEY, EX_APP_SITE,
                    &command_handler_resource, &state_updater_resource, NULL);
            onboard_with_thing_id(&kii_thing_if, EX_AUTH_THING_ID,
                    EX_AUTH_VENDOR_PASS);
            while (1) {}
            break;
        case 2:
            init_kii_thing_if_with_onboarded_thing(&kii_thing_if, EX_APP_ID, EX_APP_KEY,
                    EX_APP_SITE, EX_AUTH_THING_ID, EX_ACCESS_TOKEN,
                    &command_handler_resource, &state_updater_resource, NULL);
            while (1) {}
            break;
        case 3:
            printf("to configure parameters, edit example.h\n\n");
            printf("commands: \n");
            printf("--vendor-thing-id\n onboard to thing_if cloud with vendor thing ID.\n");
            printf("--thing-id\n onboard to thing if cloud with thing ID.\n");
            printf("--onboarded\n join to onboarded thing if cloud with thing ID and access token.\n");
            printf("--help\n show this help.\n");
            break;
        default:
            printf("unknown option: %s\n", argv[1]);
            break;
    }

    return 0;
}
