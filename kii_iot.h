#ifndef _KII_IOT_
#define _KII_IOT_

#include <kii.h>

#define EMESSAGE_SIZE 50

#ifdef __cplusplus
extern "C" {
#endif

/** callback function for handling action.
 * @param [in] schema name of schema.
 * @maram [in] schema_version version of schema.
 * @param [in] action_name name of the action.
 * @param [in] action_params json object represents parameter of this action.
 * @param [out] error error message if operation is failed.(optional)
 * @return KII_TRUE if succeeded, otherwise KII_FALSE.
 */
typedef kii_bool_t
    (*KII_IOT_ACTION_HANDLER)
        (const char* schema,
         int schema_version,
         const char* action_name,
         const char* action_params,
         char error[EMESSAGE_SIZE + 1]);

/** a function pointer to write thing state.
 *
 * This function pointer is used at KII_IOT_STATE_HANDLER. This
 * function pointer is passed as second argument of
 * KII_IOT_STATE_HANDLER. Implementation of this function pointer is
 * provided by this SDK.
 *
 * @param [in] context context of state handler.
 * @param [in] buff json string of thing state. must be null terminated.
 * @return KII_TRUE if succeeded. otherwise KII_FALSE.
 */
typedef kii_bool_t (*KII_IOT_WRITER)(kii_t* kii, const char* buff);

/** callback function for writing thing state.
 *
 * This callback function should write current thing state with
 * KII_IOT_WRITER. for example:
 *
 * @code
 * kii_bool_t state_handler(
 *         kii_t* kii,
 *         KII_IOT_WRITER writer)
 * {
 *     char buf[256];
 *
 *     if ((*writer)(kii, "{\"power\":") == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     if (get_power() != 0) {
 *         if ((*writer)(kii, "true,") == KII_FALSE) {
 *             return KII_FALSE;
 *         }
 *     } else {
 *         if ((*writer)(kii, "false,") == KII_FALSE) {
 *             return KII_FALSE;
 *         }
 *     }
 *     if ((*writer)(kii, "\"brightness\":") == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     sprintf(buf, "%d,", get_brightness());
 *     if ((*writer)(kii, buf) == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     if ((*writer)(kii, "\"color\":[") == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     sprintf(buf, "%d,", get_color(0));
 *     if ((*writer)(kii, buf) == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     sprintf(buf, "%d,", get_color(1));
 *     if ((*writer)(kii, buf) == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     sprintf(buf, "%d],", get_color(2));
 *     if ((*writer)(kii, buf) == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     if ((*writer)(kii, "\"colorTemperature\":") == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     sprintf(buf, "%d}", get_colorTemperature());
 *     if ((*writer)(kii, buf) == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     return KII_TRUE;
 * }
 * @code
 *
 * @param [in] kii state_updater object.
 * @param [in] writer writer to write thing state. implementation of
 * this writer is provided by this SDK.
 * @return KII_TRUE if succeeded. otherwise KII_FALSE.
 */
typedef kii_bool_t
    (*KII_IOT_STATE_HANDLER)
        (kii_t* kii,
         KII_IOT_WRITER writer);

/** Resource for command handler. */
typedef struct kii_iot_command_handler_resource_t {
    /** HTTP request and response buffer for command handler. */
    char* buffer;

    /** HTTP request and response buffer size for command handler. */
    size_t buffer_size;

    /** buffer for MQTT. */
    char* mqtt_buffer;

    /** buffer size for MQTT. */
    size_t mqtt_buffer_size;

    /** callback function to handle received action. */
    KII_IOT_ACTION_HANDLER action_handler;
} kii_iot_command_handler_resource_t;

/** Resource for state updater. */
typedef struct kii_iot_state_updater_resource_t {
    /** HTTP request and response buffer for state updater. */
    char* buffer;

    /** HTTP request and response buffer size for state updater. */
    size_t buffer_size;

    /** the period of updating state in milliseconds. */
    int period;

    /** callback function to write thing state. */
    KII_IOT_STATE_HANDLER state_handler;
} kii_iot_state_updater_resource_t;

typedef struct kii_iot_t {
    kii_t command_handler;
    kii_t state_updater;
    KII_IOT_ACTION_HANDLER action_handler;
    KII_IOT_STATE_HANDLER state_handler;
    /** Specify the period of updating state in milliseconds. */
    int state_update_period;
} kii_iot_t;

/** Initialize kii_iot_t object.
 *
 * After this method is called, applications must call
 * onboard_with_vendor_thing_id(kii_iot_t*, const char*, const char*,
 * const char*, const char*) or onboard_with_thing_id(kii_iot_t*,
 * const char*, const char*) to onboard from thing.
 *
 * @param [in] kii_iot kii_iot_t object to be initialized.
 * @param [in] app_id the input of Application ID
 * @param [in] app_key the input of Application Key
 * @param [in] app_host host name. should be one of "CN", "JP", "US",
 * "SG"
 * @param [in] command_handler_data data container for command handler.
 * @param [in] state_updater_data data container for state updater.
 * @param [in] resource_cb callback to resize to kii_json_resource
 * contents. This is optional. If you build IoTCloud ThingSDK with
 * KII_JSON_FIXED_TOKEN_NUM macro, you can set NULL to this
 * argument. otherwise, you need to set kii_json_resource_t object to
 * this argument.
 *
 * @return KII_TRUE when succeeded, KII_FALSE when failed.
 */
kii_bool_t init_kii_iot(
        kii_iot_t* kii_iot,
        const char* app_id,
        const char* app_key,
        const char* app_host,
        kii_iot_command_handler_resource_t* command_handler_resouce,
        kii_iot_state_updater_resource_t* state_updater_resouce,
        KII_JSON_RESOURCE_CB resource_cb);

/** Onboard to IoT Cloud with specified vendor thing ID.
 * kii_iot_t#command_handler and kii_iot_t#state_updater instances are
 * used to call api.
 * @param [inout] kii_iot kii IoT SDK instance.
 * @param [in] vendor_thing_id vendor thing id given by thing vendor.
 * NonNull, NonEmpty value must be specified.
 * @param [in] password password of the thing given by thing vendor.
 * NonNull, NonEmpty value must be specified.
 * @param [in] thing_type type of the thing. If the thing is already registered,
 * this argument is ignored. If NULL the argument is ignored.
 * @param [in] thing_properties Other properties of the thing.
 * please refer to http://docs.kii.com/rest/#thing_management-register_a_thing
 * about the format.
 * If the thing is already registered, this argument is ignored.
 * If NULL the argument is ignored.
 * @return KII_TRUE when succeeded, KII_FALSE when failed.
 */
kii_bool_t onboard_with_vendor_thing_id(
        kii_iot_t* kii_iot,
        const char* vendor_thing_id,
        const char* password,
        const char* thing_type,
        const char* thing_properties
        );

/** Onboard to IoT Cloud with specified thing ID.
 * kii_iot_t#command_handler and kii_iot_t#state_updater instances are
 * used to call api.
 * @param [inout] kii_iot kii IoT SDK instance.
 * @param [in] thing_id thing id issued by Kii Cloud.
 * NonNull, NonEmpty value must be specified.
 * @param [in] password password of the thing given by thing vendor.
 * NonNull, NonEmpty value must be specified.
 * @return KII_TRUE when succeeded, KII_FALSE when failed.
 */
kii_bool_t onboard_with_thing_id(
        kii_iot_t* kii_iot,
        const char* thing_id,
        const char* password
        );

/** Initialize kii_iot_t object with onboarded thing information.
 *
 * This api is used when onboard process has been done by controller
 * application (typically a mobile apps.) and thing ID and access
 * token is given by the controller application (via BLE, etc.) Since
 * in this case onboard process is already completed, no need to call
 * onboard_with_vendor_thing_id() or onboard_with_thing_id().
 *
 * kii_iot_t#command_handler and kii_iot_t#state_updater instances are
 * used to call api.
 *
 * @param [in] app_id the input of Application ID
 * @param [in] app_key the input of Application Key
 * @param [in] app_host host name. should be one of "CN", "JP", "US",
 * "SG"
 * @param [in] thing_id thing id given by a controller application
 * NonNull, NonEmpty value must be specified.
 * @param [in] access_token access token of the thing given by a
 * controller application. NonNull, NonEmpty value must be specified.
 * @param [in] command_handler_data data container for command handler.
 * @param [in] state_updater_data data container for state updater.
 * @param [in] resource_cb callback to resize to kii_json_resource
 * contents. This is optional. If you build IoTCloud ThingSDK with
 * KII_JSON_FIXED_TOKEN_NUM macro, you can set NULL to this
 * argument. otherwise, you need to set kii_json_resource_t object to
 * this argument.
 *
 * @return KII_TRUE when succeeded, KII_FALSE when failed.
 */
kii_bool_t init_kii_iot_with_onboarded_thing(
        kii_iot_t* kii_iot,
        const char* app_id,
        const char* app_key,
        const char* app_host,
        const char* thing_id,
        const char* access_token,
        kii_iot_command_handler_resource_t* command_handler_resouce,
        kii_iot_state_updater_resource_t* state_updater_resouce,
        KII_JSON_RESOURCE_CB resource_cb);

#ifdef __cplusplus
}
#endif

#endif /* _KII_IOT_ */

