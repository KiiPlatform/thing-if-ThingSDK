#include "example.h"

#include <kii_thing_if.h>
#include <kii_json.h>

#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

#include <pthread.h>
#include <unistd.h>
#include "sys_cb_impl.h"
#include "pi_control.h"

typedef struct prv_air_conditioner_t {
    kii_bool_t power;
    float temperature;
} prv_air_conditioner_t;

static prv_air_conditioner_t m_air_conditioner;
static pthread_mutex_t m_mutex;

static kii_bool_t prv_get_air_conditioner_info(
        prv_air_conditioner_t* air_conditioner)
{
    if (pthread_mutex_lock(&m_mutex) != 0) {
        return KII_FALSE;
    }
    air_conditioner->power = m_air_conditioner.power;
    int temp = readDS18B20Temparature();
    if (temp <= -9996) {
        printf("failed to read temperature, code: %d\n", temp);
        return KII_FALSE;
    }
    air_conditioner->temperature = temp/10.0;
    if (pthread_mutex_unlock(&m_mutex) != 0) {
        return KII_FALSE;
    }
    return KII_TRUE;
}

static kii_bool_t prv_set_air_conditioner_info(
        const prv_air_conditioner_t* air_conditioner)
{
    if (pthread_mutex_lock(&m_mutex) != 0) {
        return KII_FALSE;
    }
    m_air_conditioner.power = air_conditioner->power;
    if (pthread_mutex_unlock(&m_mutex) != 0) {
        return KII_FALSE;
    }
    return KII_TRUE;
}

static kii_bool_t action_handler(
        const char* alias,
        const char* action_name,
        const char* action_params,
        char error[EMESSAGE_SIZE + 1])
{
    prv_air_conditioner_t air_conditioner;

    printf("alias=%s, action name=%s, action params=%s\n",
            alias, action_name, action_params);

    if (strcmp(alias, "AirConditionerAlias") != 0 &&
            strcmp(alias, "HumidityAlias") != 0) {
        snprintf(error, EMESSAGE_SIZE + 1, "invalid alias: %s", alias);
        return KII_FALSE;
    }

    memset(&air_conditioner, 0, sizeof(air_conditioner));
    if (prv_get_air_conditioner_info(&air_conditioner) == KII_FALSE) {
        printf("fail to lock.\n");
        strcpy(error, "fail to lock.");
        return KII_FALSE;
    }
    if (strcmp(action_name, "turnPower") == 0) {
        air_conditioner.power =
            strcmp(action_params, "true") == 0 ? KII_TRUE : KII_FALSE;
        if (air_conditioner.power == KII_TRUE) {
            turnOnLED(0, 50, 0);
        } else {
            turnOffLED();
        }
    }

    if (prv_set_air_conditioner_info(&air_conditioner) == KII_FALSE) {
        printf("fail to unlock.\n");
        return KII_FALSE;
    }
    return KII_TRUE;
}

static kii_bool_t state_handler(
        kii_t* kii,
        KII_THING_IF_WRITER writer)
{
    char buf[256];
    prv_air_conditioner_t air_conditioner;
    memset(&air_conditioner, 0x00, sizeof(air_conditioner));
    if (prv_get_air_conditioner_info(&air_conditioner) == KII_FALSE) {
        printf("fail to lock.\n");
        return KII_FALSE;
    }

    snprintf(
        buf,
        sizeof(buf) / sizeof(buf[0]),
        "{\"AirConditionerAlias\":{\"power\":%s\"currentTemperature\":%.1f}}",
        (int)air_conditioner.power == (int)KII_JSON_TRUE ? "true," : "false,",
        air_conditioner.temperature
    );

    if ((*writer)(kii, buf) == KII_FALSE) {
        return KII_FALSE;
    }
    return KII_TRUE;
}

static void print_help() {
    printf("sub commands: [onboard|onboard-with-token|get|update]\n\n");
    printf("to see detail usage of sub command, execute ./exampleapp {subcommand} --help\n\n");

    printf("onboard with vendor-thing-id\n");
    printf("./exampleapp onboard --vendor-thing-id={vendor thing id} --password={password}\n\n");

    printf("onboard with thing-id\n");
    printf("./exampleapp onboard --thing-id={thing id} --password={password}\n\n");

    printf("onboard-with-token.\n");
    printf("./exampleapp onboard-with-token --thing-id={thing id} --access-token={access token}\n\n");
    printf("to configure app to use, edit example.h\n\n");

    printf("get.\n"
            "./exampleapp get --firmware-version --thing-type --vendor-thing-id={vendor thing id} --password={password} \n\n");

    printf("get.\n"
            "./exampleapp get --firmware-version --thing-type --thing-id={thing id} --password={password} \n\n");

    printf("update.\n"
            "./exampleapp update --firmware-version --thing-type --vendor-thing-id={vendor thing id} --password={password} \n\n");

    printf("update.\n"
            "./exampleapp update --firmware-version --thing-type --thing-id={thing id} --password={password} \n\n");

}

int main(int argc, char** argv)
{
    // setting up wiringPi
    initLEDPins();

    // setting up kii sdk
    char* subc = argv[1];
    kii_thing_if_command_handler_resource_t command_handler_resource;
    kii_thing_if_state_updater_resource_t state_updater_resource;
    kii_thing_if_system_cb_t sys_cb;
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

    state_updater_resource.buffer = state_updater_buff;
    state_updater_resource.buffer_size =
        sizeof(state_updater_buff) / sizeof(state_updater_buff[0]);
    state_updater_resource.period = EX_STATE_UPDATE_PERIOD;
    state_updater_resource.state_handler = state_handler;

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
                                &command_handler_resource, &state_updater_resource, &sys_cb, NULL);
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
                        &command_handler_resource, &state_updater_resource, &sys_cb, NULL);
                if (result == KII_FALSE) {
                    printf("failed to onboard.\n");
                    exit(1);
                }
                if (vendorThingID != NULL) {
                    result = onboard_with_vendor_thing_id(
                            &kii_thing_if,
                            vendorThingID,
                            password,
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                            NULL);
                } else {
                    result = onboard_with_thing_id(
                            &kii_thing_if,
                            thingID,
                            password,
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                            NULL);
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
                    printf("onboard --vendor-thing-id={ID of the thing} --password={password of the thing}\n");
                    break;
                default:
                    printf("unexpected usage.\n");
            }
            if (strcmp(optName, "help") == 0) {
                break;
            }
        }

    } else if (strcmp(subc, "get") == 0) {
        char* vendorThingID = NULL;
        char* thingID = NULL;
        char* password = NULL;
        int getFirmwareVersion = 0;
        int getThingType = 0;
        while (1) {
            struct option longOptions[] = {
                {"vendor-thing-id", required_argument, 0, 0},
                {"thing-id", required_argument, 0, 1},
                {"password", required_argument, 0, 2},
                {"firmware-version", no_argument, 0, 3},
                {"thing-type", no_argument, 0, 4},
                {"help", no_argument, 0, 5},
                {0, 0, 0, 0}
            };
            int optIndex = 0;
            int c = getopt_long(argc, argv, "", longOptions, &optIndex);
            if (c == -1) {
                break;
            }
            switch (c) {
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
                    getFirmwareVersion = 1;
                    break;
                case 4:
                    getThingType = 1;
                    break;
                case 5:
                    printf("usage: \n"
                            "get --vendor-thing-id={ID of the thing} "
                            "--password={password of the thing} "
                            "--thing-type "
                            "--firmware-version\n");
                    exit(0);
                    break;
            }
        }
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
        if (getFirmwareVersion == 0 && getThingType == 0) {
            printf("--firmware-version or --thing-type must be specified.\n");
            exit(1);
        }
        if (init_kii_thing_if(
                &kii_thing_if,
                EX_APP_ID,
                EX_APP_KEY,
                EX_APP_SITE,
                &command_handler_resource,
                &state_updater_resource,
                &sys_cb,
                NULL) == KII_FALSE) {
            printf("fail to initialize.\n");
            exit(1);
        }
        if (vendorThingID != NULL) {
            if (onboard_with_vendor_thing_id(
                    &kii_thing_if,
                    vendorThingID,
                    password,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL) == KII_FALSE) {
                printf("fail to onboard.\n");
                exit(1);
            }
        } else {
            if (onboard_with_thing_id(
                    &kii_thing_if,
                    thingID,
                    password,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL) == KII_FALSE) {
                printf("fail to onboard.\n");
                exit(1);
            }
        }

        if (getFirmwareVersion != 0) {
            char firmwareVersion[64];
            kii_thing_if_error_t error;
            if (get_firmware_version(
                    &kii_thing_if,
                    firmwareVersion,
                    sizeof(firmwareVersion) / sizeof(firmwareVersion[0]),
                    &error) == KII_FALSE) {
                printf("get_firmware_version is failed: %d\n", error.code);
                if (error.code == KII_THING_IF_ERROR_HTTP) {
                    printf("status code=%d, error code=%s\n",
                            error.http_status_code,
                            error.error_code);
                }
                exit(0);
            }
            printf("firmware version=%s\n", firmwareVersion);
        }
        if (getThingType != 0) {
            char thingType[64];
            kii_thing_if_error_t error;
            if (get_thing_type(
                    &kii_thing_if,
                    thingType,
                    sizeof(thingType) / sizeof(thingType[0]),
                    &error) == KII_FALSE) {
                printf("get_thing_type is failed: %d\n", error.code);
                if (error.code == KII_THING_IF_ERROR_HTTP) {
                    printf("status code=%d, error code=%s\n",
                            error.http_status_code,
                            error.error_code);
                }
                exit(0);
            }
            printf("thing type=%s\n", thingType);
        }
        exit(0);
    } else if (strcmp(subc, "update") == 0) {
        char* vendorThingID = NULL;
        char* thingID = NULL;
        char* password = NULL;
        char* firmwareVersion = NULL;
        char* thingType = NULL;
        while (1) {
            struct option longOptions[] = {
                {"vendor-thing-id", required_argument, 0, 0},
                {"thing-id", required_argument, 0, 1},
                {"password", required_argument, 0, 2},
                {"firmware-version", required_argument, 0, 3},
                {"thing-type", required_argument, 0, 4},
                {"help", no_argument, 0, 5},
                {0, 0, 0, 0}
            };
            int optIndex = 0;
            int c = getopt_long(argc, argv, "", longOptions, &optIndex);
            if (c == -1) {
                break;
            }
            switch (c) {
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
                    firmwareVersion = optarg;
                    break;
                case 4:
                    thingType = optarg;
                    break;
                case 5:
                    printf("usage: \n"
                            "update --vendor-thing-id={ID of the thing} "
                            "--password={password of the thing} "
                            "--thing-type={thing type "
                            "--firmware-version={firmware version}\n");
                    exit(0);
                    break;
            }
        }
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
        if (firmwareVersion == NULL && thingType == NULL) {
            printf("--firmware-version or --thing-type must be specified.\n");
            exit(1);
        }
        if (init_kii_thing_if(
                &kii_thing_if,
                EX_APP_ID,
                EX_APP_KEY,
                EX_APP_SITE,
                &command_handler_resource,
                &state_updater_resource,
                &sys_cb,
                NULL) == KII_FALSE) {
            printf("fail to initialize.\n");
            exit(1);
        }
        if (vendorThingID != NULL) {
            if (onboard_with_vendor_thing_id(
                    &kii_thing_if,
                    vendorThingID,
                    password,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL) == KII_FALSE) {
                printf("fail to onboard.\n");
                exit(1);
            }
        } else {
            if (onboard_with_thing_id(
                    &kii_thing_if,
                    thingID,
                    password,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL) == KII_FALSE) {
                printf("fail to onboard.\n");
                exit(1);
            }
        }

        if (firmwareVersion != NULL) {
            kii_thing_if_error_t error;
            if (update_firmware_version(
                    &kii_thing_if,
                    firmwareVersion,
                    &error) == KII_FALSE) {
                printf("update_firmware_version is failed: %d\n", error.code);
                if (error.code == KII_THING_IF_ERROR_HTTP) {
                    printf("status code=%d, error code=%s\n",
                            error.http_status_code,
                            error.error_code);
                }
                exit(1);
            }
            printf("firmware version successfully updated.\n");
        }
        if (thingType != NULL) {
            kii_thing_if_error_t error;
            if (update_thing_type(
                    &kii_thing_if,
                    thingType,
                    &error) == KII_FALSE) {
                printf("update_thing_type is failed: %d\n", error.code);
                if (error.code == KII_THING_IF_ERROR_HTTP) {
                    printf("status code=%d, error code=%s\n",
                            error.http_status_code,
                            error.error_code);
                }
                exit(1);
            }
            printf("thing type successfully updated.\n");
        }
        exit(0);
    } else {
        print_help();
        exit(0);
    }

    start(&kii_thing_if);
    /* run forever. TODO: Convert to daemon. */
    while(1){ sleep(1); };

    /*
     * This sample application keeps mutex from the start to end
     * of the applicatoin process. So we don't implement destry.
     * pthread_mutex_destroy(&m_mutex);
    */
}

/* vim: set ts=4 sts=4 sw=4 et fenc=utf-8 ff=unix: */

