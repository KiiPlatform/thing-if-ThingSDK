#ifndef _KII_IOT_
#define _KII_IOT_

#include <kii.h>

#define EMESSAGE_SIZE 50

#ifdef __cplusplus
extern "C" {
#endif

/** callback function for handling action.
 * @param [in] app_context application context which is passed at
 * init_kii_iot(kii_iot_t*, const char*, const char*, const char*,
 * char*, size_t, char*, size_t, char*, size_t, int,
 * KII_IOT_ACTION_HANDLER, KII_IOT_STATE_HANDLER, void*).
 * @param [in] schema name of schema.
 * @maram [in] schema_version version of schema.
 * @param [in] action_name name of the action.
 * @param [in] action_params json object represents parameter of this action.
 * @param [out] error error message if operation is failed.(optional)
 * @return KII_TRUE if succeeded, otherwise KII_FALSE.
 */
typedef kii_bool_t
    (*KII_IOT_ACTION_HANDLER)
        (void* app_context,
         const char* schema,
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
typedef kii_bool_t(*KII_IOT_WRITER)(const kii_t* kii, const char* buff);

/** callback function for writing thing state.
 *
 * This callback function should write current thing state with
 * KII_IOT_WRITER. for example:
 *
 * @code
 * typedef struct smart_light_t {
 *     kii_bool_t power;
 *     int brightness;
 *     int color[3];
 *     int colorTemperature;
 * } smart_light_t;
 *
 * kii_bool_t state_handler(
 *         void* app_context,
 *         kii_t* kii,
 *         KII_IOT_WRITER writer)
 * {
 *     smart_light_t* smart_light = (smart_light_t*)app_context;
 *     char buf[256];
 *
 *     if ((*writer)(kii, "{\"power\":") == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     if (smart_light->power == KII_TRUE) {
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
 *     sprintf(buf, "%d,", smart_light->brightness);
 *     if ((*writer)(kii, buf) == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     if ((*writer)(kii, "\"color\":[") == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     sprintf(buf, "%d,", smart_light->color[0]);
 *     if ((*writer)(kii, buf) == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     sprintf(buf, "%d,", smart_light->color[1]);
 *     if ((*writer)(kii, buf) == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     sprintf(buf, "%d],", smart_light->color[2]);
 *     if ((*writer)(kii, buf) == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     if ((*writer)(kii, "\"colorTemperature\":") == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     sprintf(buf, "%d}", smart_light->colorTemperature);
 *     if ((*writer)(kii, buf) == KII_FALSE) {
 *         return KII_FALSE;
 *     }
 *     return KII_TRUE;
 * }
 * @code
 *
 * @param [in] kii state_updater object.
 * @param [in] app_context application context which is passed at
 * init_kii_iot(kii_iot_t*, const char*, const char*, const char*,
 * char*, size_t, char*, size_t, char*, size_t, int,
 * KII_IOT_ACTION_HANDLER, KII_IOT_STATE_HANDLER, void*).
 * @param [in] writer writer to write thing state. implementation of
 * this writer is provided by this SDK.
 * @return KII_TRUE if succeeded. otherwise KII_FALSE.
 */
typedef kii_bool_t
    (*KII_IOT_STATE_HANDLER)
        (kii_t* kii,
         void* app_context,
         KII_IOT_WRITER writer);

typedef struct kii_iot_t {
    kii_t command_handler;
    kii_t state_updater;
    KII_IOT_ACTION_HANDLER action_handler;
    KII_IOT_STATE_HANDLER state_handler;
    /** Specify the period of updating state in milliseconds. */
    int state_update_period;
    void* app_context;
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
        int state_update_period,
        KII_IOT_ACTION_HANDLER command_handler,
        KII_IOT_STATE_HANDLER state_handler,
        void* app_context);

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

