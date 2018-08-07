/**
 * @file kii_thing_if_environment_dependent.h
 * @brief This is a file defining environment dependent functions.
 * this SDKrequires to implement these functions in each target
 * environment.
 */
#ifndef _KII_THING_IF_ENVIRONMENT_DEPENDENT_
#define _KII_THING_IF_ENVIRONMENT_DEPENDENT_

#ifdef __cplusplus
extern "C" {
#endif

#include <kii_socket_callback.h>
#include <kii_task_callback.h>

/** Implementation of callback to connect socket to HTTP or HTTPS
 * server. this SDK requirest to implement this function in
 * each target environment.
 *
 * This function is assigned to fields
 * kii_t#kii_core_t#kii_http_context_t#kii_http_context_t#connect_cb
 * of command handler and state updater.
 *
 * @param [in] socket_context context object.
 * @param [in] host host name.
 * @param [in] port port number.
 * @return If connection is succeeded, applications need to return
 * KII_SOCKETC_OK. If connection is failed, applications need to
 * return KII_SOCKETC_FAIL. If applications want to pend returning
 * success or fail, applications need to return KII_SOCKETC_AGAIN.
 */
kii_socket_code_t socket_connect_cb_impl(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port);

/** Implementation of callback to send data to HTTP or HTTPS server.
 * this SDK requirest to implement this function in each
 * target environment.
 *
 * This function is assigned to fields
 * kii_t#kii_core_t#kii_http_context_t#kii_http_context_t#send_cb of
 * command handler and state updater.
 *
 * @param [in] socket_context context object.
 * @param [in] buffer data to send server.
 * @param [in] length length of buffer.
 * @return If applications succeed to send data, applications need to
 * return KII_SOCKETC_OK. If connection is failed. applications need
 * to return KII_SOCKETC__FAIL. If applications don't want to send
 * data, applications need to return KII_SOCKETC_AGAIN. In this case,
 * this SDK pass same data to this function again.
 */
kii_socket_code_t socket_send_cb_impl(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length);

/** Implementation of callback for receive data from HTTP or HTTPS server.
 * this SDK requirest to implement this function in each
 * target environment.
 *
 * This function is assigned to fields
 * kii_t#kii_core_t#kii_http_context_t#kii_http_context_t#recv_cb of
 * command handler and state updater.
 *
 * @param [in] socket_context context object.
 * @param [out] buffer buffer to set receiving data.
 * @param [in] length_to_read buffer size.
 * @param [out] out_actual_length actual set data size.
 * @return If applications succeed to receive data and set the data to
 * buffer, applications need to return KII_SOCKETC_OK. Applications
 * also set data size to out_actual_length. If applications fail,
 * applications need to return KII_HTTPC_FAIL. If applications want to
 * wait to receive data, applications need to return
 * KII_HTTPC_AGAIN. In this case, applications must not set receving
 * data to buffer if some data is received.
 */
kii_socket_code_t socket_recv_cb_impl(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length);

/** Implementation of callback to close HTTP or HTTPS socket.
 * this SDK requirest to implement this function in each
 * target environment.
 *
 * This function is assigned to fields
 * kii_t#kii_core_t#kii_http_context_t#kii_http_context_t#close_cb of
 * command handler and state updater.
 *
 * @param [in] socket_context context object.
 *
 * @return If applications succeed to close socket, applications need
 * to return KII_SOCKETC_OK. If applications fail to close socket,
 * applications need to return KII_SOCKETC_FAIL. If applications want
 * to pend returning success or fail, applications need to return
 * KII_SOCKETC_AGAIN.
 */
kii_socket_code_t socket_close_cb_impl(
        kii_socket_context_t* socket_context);

/** Implementation of callback for connect MQTT socket to server.
 * this SDK requirest to implement this function in each
 * target environment.
 *
 * This function is assigned to a field
 * kii_t#kii_core_t#kii_http_context_t#kii_http_context_t#mqtt_socket_connect_cb
 * of command handler.
 *
 * @param [in] socket_context context object.
 * @param [in] host host name.
 * @param [in] port port number.
 * @return If connection is succeeded, applications need to return
 * KII_SOCKETC_OK. If connection is failed, applications need to
 * return KII_SOCKETC_FAIL. If applications want to pend returning
 * success or fail, applications need to return KII_SOCKETC_AGAIN.
 */
kii_socket_code_t mqtt_connect_cb_impl(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port);

/** Implementation of callback to send MQTT data to server.
 * this SDK requirest to implement this function in each
 * target environment.
 *
 * This function is assigned to a field
 * kii_t#kii_core_t#kii_http_context_t#kii_http_context_t#mqtt_socket_send_cb
 * of command handler.
 *
 * @param [in] socket_context context object.
 * @param [in] buffer data to send server.
 * @param [in] length length of buffer.
 *
 * @return If applications succeed to send data, applications need to
 * return KII_SOCKETC_OK. If connection is failed. applications need
 * to return KII_SOCKETC__FAIL. If applications don't want to send
 * data, applications need to return KII_SOCKETC_AGAIN. In this case,
 * this SDK pass same data to this function again.
 */
kii_socket_code_t mqtt_send_cb_impl(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length);

/** Implementation of callback to receive MQTT data from server.
 * this SDK requirest to implement this function in each
 * target environment.
 *
 * This function is assigned to a field
 * kii_t#kii_core_t#kii_http_context_t#kii_http_context_t#mqtt_socket_recv_cb
 * of command handler.
 *
 * @param [in] socket_context context object.
 * @param [out] buffer buffer to set receiving data.
 * @param [in] length_to_read buffer size.
 * @param [out] out_actual_length actual set data size.
 * @return If applications succeed to receive data and set the data to
 * buffer, applications need to return KII_SOCKETC_OK. Applications
 * also set data size to out_actual_length. If applications fail,
 * applications need to return KII_HTTPC_FAIL. If applications want to
 * wait to receive data, applications need to return
 * KII_HTTPC_AGAIN. In this case, applications must not set receving
 * data to buffer if some data is received.
 */
kii_socket_code_t mqtt_recv_cb_impl(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length);

/** Implementation of callback to close MQTT socket.
 * this SDK requirest to implement this function in each
 * target environment.
 *
 * This function is assigned to a field
 * kii_t#kii_core_t#kii_http_context_t#kii_http_context_t#mqtt_socket_close_cb
 * of command handler.
 *
 * @param [in] socket_context context object.
 *
 * @return If applications succeed to close socket, applications need
 * to return KII_SOCKETC_OK. If applications fail to close socket,
 * applications need to return KII_SOCKETC_FAIL. If applications want
 * to pend returning success or fail, applications need to return
 * KII_SOCKETC_AGAIN.
 */
kii_socket_code_t mqtt_close_cb_impl(kii_socket_context_t* socket_context);

/** Implementation of callback to create task.
 * this SDK requirest to implement this function in each
 * target environment.
 *
 * This function is assigned to fields
 * kii_t#kii_core_t#kii_http_context_t#kii_http_context_t#task_create_cb
 * of command handler and state updater.
 *
 * @param [in] name name of task.
 * @param [in] entry entry of task.
 * @param [in] stk_start start position of stack area.
 * @param [in] stk_size stack size of task
 * @param [in] priority priority of thisk
 *
 * @return KII_TASKC_OK if succeed to create task. otherwise KII_TASKC_FAIL.
 */
kii_task_code_t task_create_cb_impl(
        const char* name,
        KII_TASK_ENTRY entry,
        void* param);

/** Implementation of callback to delay task.
 * this SDK requirest to implement this function in each
 * target environment.
 *
 * This function is assigned to fields
 * kii_t#kii_core_t#kii_http_context_t#kii_http_context_t#delay_ms_cb
 * of command handler and state updater.
 *
 * @param[in] msec millisecond to delay.
 */
void delay_ms_cb_impl(unsigned int msec);

/** Implementation of callback to log this SDK.
 * this SDK requirest to implement this function in each
 * target environment.
 *
 * This function is assigned to fields kii_t#kii_core_t#logger_cb of
 * command handler and state updater.
 *
 * @param [in] format outputted string format.
 */
void logger_cb_impl(const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif /* _KII_THING_IF_ENVIRONMENT_DEPENDENT_ */
