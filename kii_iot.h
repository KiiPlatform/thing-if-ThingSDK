#ifndef _KII_IOT_
#define _KII_IOT_

#include <kii.h>

#define EMESSAGE_SIZE 50

#ifdef __cplusplus
extern "C" {
#endif

/** callback function for checking schema and schema version.
 * @param [in] schema name of schema.
 * @maram [in] version version of schema.
 * @return KII_TRUE if schema and schema version is valild, otherwise
 * KII_FALSE.
 */
typedef kii_bool_t
    (*KII_IOT_SCHEMA_CHECK_HANDLER)(const char* schema, const int version);

/** callback function for handling action.
 * @param [in] action_name name of the action.
 * @param [in] action_params json object represents parameter of this action.
 * @param [out] error error message if operation is failed.(optional)
 * @return KII_TRUE if succeeded, otherwise KII_FALSE.
 */
typedef kii_bool_t
    (*KII_IOT_ACTION_HANDLER)
    (const char* action_name,
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
typedef kii_bool_t
    (*KII_IOT_STATE_WRITER)
        (const state_handler_context_t* context,
         const char* buff);

/** callback function for writing state.
 *
 * This callback function should write current thing state with
 * KII_IOT_STATE_WRITER. for example:
 *
 * @code
 * kii_bool_t state_handler(
 *         const state_handler_context_t* context,
 *         KII_IOT_STATE_WRITER writer)
 * {
 *     return (*writer)(context, "{\"power\":true}")
 * }
 * @code
 *
 * @param [in] context context of state handler.
 * @param [in] writer writer to write thing state. implementation of
 * this writer is provided by this SDK.
 * @return KII_TRUE if succeeded. otherwise KII_FALSE.
 */
typedef kii_bool_t
    (*KII_IOT_STATE_HANDLER)
        (const state_handler_context_t* context,
         KII_IOT_STATE_WRITER writer);

typedef struct kii_iot_t {
    kii_t command_handler;
    kii_t state_updater;
    KII_IOT_ACTION_HANDLER action_handler;
    KII_IOT_SCHEMA_CHECK_HANDLER schema_check_handler;
    KII_IOT_STATE_HANDLER state_handler;
    /** Specify the period of updating state in milliseconds. */
    int state_update_period;
} kii_iot_t;

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
        KII_IOT_SCHEMA_CHECK_HANDLER schema_check_handler,
        KII_IOT_ACTION_HANDLER command_handler,
        KII_IOT_STATE_HANDLER state_handler);

/** On board to IoT Cloud with specified vendor thing ID.
 * kii_iot_t#command_handler instance is used to call api.
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

/** On board to IoT Cloud with specified thing ID.
 * kii_iot_t#command_handler instance is used to call api.
 * @param [inout] kii_iot kii IoT SDK instance.
 * @param [in] thing_id vendor thing id given by thing vendor.
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

#ifdef __cplusplus
}
#endif

#endif /* _KII_IOT_ */

