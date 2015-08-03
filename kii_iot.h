#ifndef _KII_IOT_
#define _KII_IOT_

#include <kii.h>

#define EMESSAGE_SIZE 50

#ifdef __cplusplus
extern "C" {
#endif

/** callback function for handling action.
 * @param [in] action_name name of the action.
 * @param [in] action_params json object represents parameter of this action.
 * @param [out] error error message if operation is failed.(optional)
 * @return true if succeeded, otherwise false.
 */
typedef kii_bool_t
    (*KII_IOT_ACTION_HANDLER)
    (const char* action_name,
     const char* action_params,
     char error[EMESSAGE_SIZE + 1]);

typedef struct kii_iot_t {
    kii_t command_handler;
    kii_t state_updater;
    KII_IOT_ACTION_HANDLER action_handler;
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
        KII_IOT_ACTION_HANDLER action_handler);

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
        const char* thing_properties,
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
        const char* password,
        );

#ifdef __cplusplus
}
#endif

#endif /* _KII_IOT_ */

