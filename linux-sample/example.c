#include "example.h"

#include <kii_thing_if.h>
#include <kii_json.h>

#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

#include <pthread.h>

typedef struct prv_smartlight_t {
    kii_json_boolean_t power;
    int brightness;
    int color[3];
    int color_temperature;
} prv_smartlight_t;

static prv_smartlight_t m_smartlight;
static pthread_mutex_t m_mutex;

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

static kii_bool_t prv_get_smartlight_info(prv_smartlight_t* smartlight)
{
    if (pthread_mutex_lock(&m_mutex) != 0) {
        return KII_FALSE;
    }
    smartlight->power = m_smartlight.power;
    smartlight->brightness = m_smartlight.brightness;
    smartlight->color[0] = m_smartlight.color[0];
    smartlight->color[1] = m_smartlight.color[1];
    smartlight->color[2] = m_smartlight.color[2];
    smartlight->color_temperature = m_smartlight.color_temperature;
    if (pthread_mutex_unlock(&m_mutex) != 0) {
        return KII_FALSE;
    }
    return KII_TRUE;
}

static kii_bool_t prv_set_smartlight_info(const prv_smartlight_t* smartlight)
{
    if (pthread_mutex_lock(&m_mutex) != 0) {
        return KII_FALSE;
    }
    m_smartlight.power = smartlight->power;
    m_smartlight.brightness = smartlight->brightness;
    m_smartlight.color[0] = smartlight->color[0];
    m_smartlight.color[1] = smartlight->color[1];
    m_smartlight.color[2] = smartlight->color[2];
    m_smartlight.color_temperature = smartlight->color_temperature;
    if (pthread_mutex_unlock(&m_mutex) != 0) {
        return KII_FALSE;
    }
    return KII_TRUE;
}

static kii_bool_t action_handler(
        const char* schema,
        int schema_version,
        const char* action_name,
        const char* action_params,
        char error[EMESSAGE_SIZE + 1])
{
    prv_smartlight_t smartlight;

    printf("schema=%s, schema_version=%d, action name=%s, action params=%s\n",
            schema, schema_version, action_name, action_params);

    if (strcmp(schema, "SmartLightDemo") != 0 || schema_version != 1) {
        printf("invalid schema: %s %d\n", schema, schema_version);
        sprintf(error, "invalid schema: %s %d", schema, schema_version);
        return KII_FALSE;
    }

    memset(&smartlight, 0x00, sizeof(smartlight));
    if (prv_get_smartlight_info(&smartlight) == KII_FALSE) {
        printf("fail to lock.\n");
        strcpy(error, "fail to lock.");
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
        smartlight.power = fields[0].field_copy.boolean_value;
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
        smartlight.brightness = fields[0].field_copy.int_value;
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
        smartlight.color[0] = fields[0].field_copy.int_value;
        smartlight.color[1] = fields[1].field_copy.int_value;
        smartlight.color[2] = fields[2].field_copy.int_value;
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
        smartlight.color_temperature = fields[0].field_copy.int_value;
    } else {
        printf("invalid action: %s\n", action_name);
        return KII_FALSE;
    }

    if (prv_set_smartlight_info(&smartlight) == KII_FALSE) {
        printf("fail to unlock.\n");
        return KII_FALSE;
    }
    return KII_TRUE;
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
        prv_smartlight_t smartlight;
        memset(&smartlight, 0x00, sizeof(smartlight));
        if (prv_get_smartlight_info(&smartlight) == KII_FALSE) {
            printf("fail to lock.\n");
            return KII_FALSE;
        }
        if ((*writer)(kii, "{\"power\":") == KII_FALSE) {
            return KII_FALSE;
        }
        if ((*writer)(kii, smartlight.power == KII_JSON_TRUE
                        ? "true," : "false,") == KII_FALSE) {
            return KII_FALSE;
        }
        if ((*writer)(kii, "\"brightness\":") == KII_FALSE) {
            return KII_FALSE;
        }

        sprintf(buf, "%d,", smartlight.brightness);
        if ((*writer)(kii, buf) == KII_FALSE) {
            return KII_FALSE;
        }

        if ((*writer)(kii, "\"color\":") == KII_FALSE) {
            return KII_FALSE;
        }
        sprintf(buf, "[%d,%d,%d],", smartlight.color[0],
                smartlight.color[1], smartlight.color[2]);
        if ((*writer)(kii, buf) == KII_FALSE) {
            return KII_FALSE;
        }

        if ((*writer)(kii, "\"colorTemperature\":") == KII_FALSE) {
            return KII_FALSE;
        }
        sprintf(buf, "%d}", smartlight.color_temperature);
        if ((*writer)(kii, buf) == KII_FALSE) {
            return KII_FALSE;
        }
        return KII_TRUE;
    }
}

static kii_bool_t custom_push_handler(
        kii_t *kii,
        const char* message,
        size_t message_length)
{
    kii_bool_t ret = KII_TRUE;
    printf("custom_push_handler:\n%s\n", message);
    if (strncmp(message, "{\"schema\"", 9) == 0) {
        ret = KII_FALSE;
    }
    // check no error in parsing topic.
    if (strncmp(message, "{\"Item\":\"CheckNoError\"", 22) == 0) {
        ret = KII_FALSE;
    }
    return ret;
}

static void print_help() {
    printf("sub commands: [onboard|onboard-with-token]\n\n");
    printf("to see detail usage of sub command, execute ./exampleapp {subcommand} --help\n\n");

    printf("onboard with vendor-thing-id\n");
    printf("./exampleapp onboard --vendor-thing-id={vendor thing id} --password={password}\n\n");

    printf("onboard with thing-id\n");
    printf("./exampleapp onboard --thing-id={vendor thing id} --password={password}\n\n");

    printf("onboard-with-token.\n");
    printf("./exampleapp onboard-with-token --thing-id={thing id} --access-token={access token}\n\n");
    printf("to configure app to use, edit example.h\n\n");
}

int main(int argc, char** argv)
{
    char* subc = argv[1];
    kii_thing_if_command_handler_resource_t command_handler_resource;
    kii_thing_if_state_updater_resource_t state_updater_resource;
    char command_handler_buff[EX_COMMAND_HANDLER_BUFF_SIZE];
    char state_updater_buff[EX_STATE_UPDATER_BUFF_SIZE];
    char mqtt_buff[EX_MQTT_BUFF_SIZE];
    kii_thing_if_t kii_thing_if;
    kii_bool_t result;

    command_handler_resource.buffer = command_handler_buff;
    command_handler_resource.buffer_size =
        sizeof(command_handler_buff) / sizeof(command_handler_buff[0]);
    command_handler_resource.mqtt_buffer = mqtt_buff;
    command_handler_resource.mqtt_buffer_size =
        sizeof(mqtt_buff) / sizeof(mqtt_buff[0]);
    command_handler_resource.action_handler = action_handler;
    command_handler_resource.state_handler = state_handler;
    command_handler_resource.custom_push_handler = custom_push_handler;

    state_updater_resource.buffer = state_updater_buff;
    state_updater_resource.buffer_size =
        sizeof(state_updater_buff) / sizeof(state_updater_buff[0]);
    state_updater_resource.period = EX_STATE_UPDATE_PERIOD;
    state_updater_resource.state_handler = state_handler;

    if (pthread_mutex_init(&m_mutex, NULL) != 0) {
        printf("fail to get mutex.\n");
        exit(1);
    }

    if (argc < 2) {
        printf("too few arguments.\n");
        print_help();
        exit(1);
    }

    /* Parse command. */
    if (strcmp(subc, "onboard-with-token") == 0) {
        char* thingID = NULL;
        char* accessToken = NULL;
        while(1) {
            struct option longOptions[] = {
                {"thing-id", required_argument, 0, 0},
                {"access-token", required_argument, 0, 1},
                {"help", no_argument, 0, 2},
                {0, 0, 0, 0}
            };
            int optIndex = 0;
            int c = getopt_long(argc, argv, "", longOptions, &optIndex);
            const char* optName = longOptions[optIndex].name;
            if (c == -1) {
                if (thingID == NULL) {
                    printf("thing-id is not specified.\n");
                    exit(1);
                }
                if (accessToken == NULL) {
                    printf("access-token is not specifeid.\n");
                    exit(1);
                }
                /* Initialize with token. */
                result = init_kii_thing_if_with_onboarded_thing(&kii_thing_if, EX_APP_ID,
                                EX_APP_KEY, EX_APP_SITE, thingID, accessToken,
                                &command_handler_resource, &state_updater_resource, NULL);
                if (result == KII_FALSE) {
                    printf("failed to onboard with token.\n");
                    exit(1);
                }
                printf("program successfully started!\n");
                break;
            }
            printf("option %s : %s\n", optName, optarg);
            switch(c) {
                case 0:
                    thingID = optarg;
                    break;
                case 1:
                    accessToken = optarg;
                    break;
                case 3:
                    printf("usage: \n");
                    printf("onboard-with-token --thing-id={ID of the thing} --access-token={access token of the thing} or\n");
                    break;
                default:
                    printf("unexpected usage.\n");
            }
            if (strcmp(optName, "help") == 0) {
                break;
            }
        }
    } else if (strcmp(subc, "onboard") == 0) {
        char* vendorThingID = NULL;
        char* thingID = NULL;
        char* password = NULL;
        while(1) {
            struct option longOptions[] = {
                {"vendor-thing-id", required_argument, 0, 0},
                {"thing-id", required_argument, 0, 1},
                {"password", required_argument, 0, 2},
                {"help", no_argument, 0, 3},
                {0, 0, 0, 0}
            };
            int optIndex = 0;
            int c = getopt_long(argc, argv, "", longOptions, &optIndex);
            const char* optName = longOptions[optIndex].name;
            if (c == -1) {
                if (vendorThingID == NULL && thingID == NULL) {
                    printf("neither vendor-thing-id and thing-id are specified.\n");
                    exit(1);
                }
                if (password == NULL) {
                    printf("password is not specifeid.\n");
                    exit(1);
                }
                if (vendorThingID != NULL && thingID != NULL) {
                    printf("both vendor-thing-id and thing-id is specified.  either of one should be specified.\n");
                    exit(1);
                }
                printf("program successfully started!\n");
                result = init_kii_thing_if(&kii_thing_if, EX_APP_ID, EX_APP_KEY, EX_APP_SITE,
                        &command_handler_resource, &state_updater_resource, NULL);
                if (result == KII_FALSE) {
                    printf("failed to onboard.\n");
                    exit(1);
                }
                if (vendorThingID != NULL) {
                    result = onboard_with_vendor_thing_id(&kii_thing_if, vendorThingID,
                            password, NULL, NULL);
                } else {
                    result = onboard_with_thing_id(&kii_thing_if, thingID,
                            password);
                }
                if (result == KII_FALSE) {
                    printf("failed to onboard.\n");
                    exit(1);
                }
                break;
            }
            printf("option %s : %s\n", optName, optarg);
            switch(c) {
                case 0:
                    vendorThingID = optarg;
                    break;
                case 1:
                    thingID = optarg;
                    break;
                case 2:
                    password = optarg;
                    break;
                case 3:
                    printf("usage: \n");
                    printf("onboard --thing-id={ID of the thing} --password={password of the thing} or\n");
                    printf("onboard --vendor-thing-id={ID of the thing} --password={password of the thing} or\n");
                    break;
                default:
                    printf("unexpected usage.\n");
            }
            if (strcmp(optName, "help") == 0) {
                break;
            }
        }
    } else {
        print_help();
        exit(0);
    }
    while(1){}; /* run forever. */

    /*
     * This sample application keeps mutex from the start to end
     * of the applicatoin process. So we don't implement destry.
     * pthread_mutex_destroy(&m_mutex);
    */
}

/* vim: set ts=4 sts=4 sw=4 et fenc=utf-8 ff=unix: */

