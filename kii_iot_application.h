#ifndef _KII_IOT_APPLICATION_
#define _KII_IOT_APPLICATION_

#ifdef __cplusplus
extern "C" {
#endif

#include <kii.h>

/** Set up callbacks for command handler.
 *
 * Applications must implement this function.
 * In this function, Applications must set up callbacks for kii_t.
 *
 * If you use this SDK with default http client mode, you need to set
 * up following callbacks.
 * - kii_t#kii_core_t#kii_http_context_t#connect_cb
 * - kii_t#kii_core_t#kii_http_context_t#send_cb
 * - kii_t#kii_core_t#kii_http_context_t#recv_cb
 * - kii_t#kii_core_t#kii_http_context_t#close_cb
 * - kii_t#mqtt_socket_connect_cb
 * - kii_t#mqtt_socket_send_cb
 * - kii_t#mqtt_socket_recv_cb
 * - kii_t#mqtt_socket_close_cb
 * - kii_t#task_create_cb
 * - kii_t#delay_ms_cb
 * - kii_t#kii_core_t#logger_cb
 *
 * If you use this SDK with custom http client mode, you need to set
 * up following callbacks.
 * - kii_t#kii_core_t#http_set_request_line_cb
 * - kii_t#kii_core_t#http_set_header_cb
 * - kii_t#kii_core_t#http_append_body_start_cb
 * - kii_t#kii_core_t#http_append_body_cb
 * - kii_t#kii_core_t#http_append_body_end_cb
 * - kii_t#kii_core_t#http_execute_cb
 * - kii_t#mqtt_socket_connect_cb
 * - kii_t#mqtt_socket_send_cb
 * - kii_t#mqtt_socket_recv_cb
 * - kii_t#mqtt_socket_close_cb
 * - kii_t#task_create_cb
 * - kii_t#delay_ms_cb
 * - kii_t#kii_core_t#logger_cb
 *
 * If you want to use this SDK with custom http client mode, you need
 * to build this SDK with USE_CUSTOM_HTTP_CLIENT macro.
 *
 * All callbacks are mandatory except kii_t#kii_core_t#logger_cb.
 *
 * @param [inout] kii_t command handler object.
 */
void setup_command_handler_callbacks(kii_t *command_handler);

/** Set up callbacks for state updater.
 *
 * Applications must implement this function.
 * In this function, Applications must set up callbacks for kii_t.
 *
 * If you use this SDK with default http client mode, you need to set
 * up following callbacks.
 * - kii_t#kii_core_t#kii_http_context_t#connect_cb
 * - kii_t#kii_core_t#kii_http_context_t#send_cb
 * - kii_t#kii_core_t#kii_http_context_t#recv_cb
 * - kii_t#kii_core_t#kii_http_context_t#close_cb
 * - kii_t#task_create_cb
 * - kii_t#delay_ms_cb
 * - kii_t#kii_core_t#logger_cb
 *
 * If you use this SDK with custom http client mode, you need to set
 * up following callbacks.
 * - kii_t#kii_core_t#http_set_request_line_cb
 * - kii_t#kii_core_t#http_set_header_cb
 * - kii_t#kii_core_t#http_append_body_start_cb
 * - kii_t#kii_core_t#http_append_body_cb
 * - kii_t#kii_core_t#http_append_body_end_cb
 * - kii_t#kii_core_t#http_execute_cb
 * - kii_t#task_create_cb
 * - kii_t#delay_ms_cb
 * - kii_t#kii_core_t#logger_cb
 *
 * If you want to use this SDK with custom http client mode, you need
 * to build this SDK with USE_CUSTOM_HTTP_CLIENT macro.
 *
 * All callbacks are mandatory except kii_t#kii_core_t#logger_cb.
 *
 * @param [inout] kii_t state updater object.
 */
void setup_state_update_callbacks(kii_t *state_updater);

#ifdef __cplusplus
}
#endif

#endif /* _KII_IOT_APPLICATION_ */
