#ifndef _KII_IOT_CALLBACKS_
#define _KII_IOT_CALLBACKS_

#ifdef __cplusplus
extern "C" {
#endif

#include <kii_socket_callback.h>
#include <kii_task_callback.h>

/** Callback for connecting command handler socket to server.
 * Applications must implement this callback in the target enviroment.
 *
 * @param [in] socket_context context object.
 * @param [in] host host name.
 * @param [in] port port number.
 * @return If connection is succeeded, applications need to return
 * KII_SOCKETC_OK. If connection is failed, applications need to
 * return KII_SOCKETC_FAIL. If applications want to pend returning
 * success or fail, applications need to return KII_SOCKETC_AGAIN.
 */
kii_socket_code_t command_handler_connect_cb(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port);

/** Callback for sending command handler data to server.
 * Applications must implement this callback in the target enviroment.
 *
 * @param [in] socket_context context object.
 * @param [in] buffer data to send server.
 * @param [in] length length of buffer.

 * @return If applications succeed to send data, applications need to
 * return KII_SOCKETC_OK. If connection is failed. applications need
 * to return KII_SOCKETC__FAIL. If applications don't want to send
 * data, applications need to return KII_SOCKETC_AGAIN. In this case,
 * KiiThingSDK Embedded Core pass same data to this callback again.
 */
kii_socket_code_t command_handler_send_cb(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length);

/** Callback for receiving command handler data from server.
 * Applications must implement this callback in the target enviroment.
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
kii_socket_code_t command_handler_recv_cb(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length);

/** Callback for closing command handler socket.
 * Applications must implement this callback in the target enviroment.
 *
 * @param [in] socket_context context object.
 *
 * @return If applications succeed to close socket, applications need
 * to return KII_SOCKETC_OK. If applications fail to close socket,
 * applications need to return KII_SOCKETC_FAIL. If applications want
 * to pend returning success or fail, applications need to return
 * KII_SOCKETC_AGAIN.
 */
kii_socket_code_t command_handler_close_cb(
        kii_socket_context_t* socket_context);

/** Callback for connecting mqtt socket to server.
 * Applications must implement this callback in the target enviroment.
 *
 * @param [in] socket_context context object.
 * @param [in] host host name.
 * @param [in] port port number.
 * @return If connection is succeeded, applications need to return
 * KII_SOCKETC_OK. If connection is failed, applications need to
 * return KII_SOCKETC_FAIL. If applications want to pend returning
 * success or fail, applications need to return KII_SOCKETC_AGAIN.
 */
kii_socket_code_t mqtt_connect_cb(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port);

/** Callback for sending mqtt data to server.
 * Applications must implement this callback in the target enviroment.
 *
 * @param [in] socket_context context object.
 * @param [in] buffer data to send server.
 * @param [in] length length of buffer.

 * @return If applications succeed to send data, applications need to
 * return KII_SOCKETC_OK. If connection is failed. applications need
 * to return KII_SOCKETC__FAIL. If applications don't want to send
 * data, applications need to return KII_SOCKETC_AGAIN. In this case,
 * KiiThingSDK Embedded Core pass same data to this callback again.
 */
kii_socket_code_t mqtt_send_cb(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length);

/** Callback for receiving mqtt data from server.
 * Applications must implement this callback in the target enviroment.
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
kii_socket_code_t mqtt_recv_cb(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length);

/** Callback for closing mqtt socket.
 * Applications must implement this callback in the target enviroment.
 *
 * @param [in] socket_context context object.
 *
 * @return If applications succeed to close socket, applications need
 * to return KII_SOCKETC_OK. If applications fail to close socket,
 * applications need to return KII_SOCKETC_FAIL. If applications want
 * to pend returning success or fail, applications need to return
 * KII_SOCKETC_AGAIN.
 */
kii_socket_code_t mqtt_close_cb(kii_socket_context_t* socket_context);

/** Callback for connecting state updater socket to server.
 * Applications must implement this callback in the target enviroment.
 *
 * @param [in] socket_context context object.
 * @param [in] host host name.
 * @param [in] port port number.
 * @return If connection is succeeded, applications need to return
 * KII_SOCKETC_OK. If connection is failed, applications need to
 * return KII_SOCKETC_FAIL. If applications want to pend returning
 * success or fail, applications need to return KII_SOCKETC_AGAIN.
 */
kii_socket_code_t state_updater_connect_cb(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port);

/** Callback for sending state updater data to server.
 * Applications must implement this callback in the target enviroment.
 *
 * @param [in] socket_context context object.
 * @param [in] buffer data to send server.
 * @param [in] length length of buffer.

 * @return If applications succeed to send data, applications need to
 * return KII_SOCKETC_OK. If connection is failed. applications need
 * to return KII_SOCKETC__FAIL. If applications don't want to send
 * data, applications need to return KII_SOCKETC_AGAIN. In this case,
 * KiiThingSDK Embedded Core pass same data to this callback again.
 */
kii_socket_code_t state_updater_send_cb(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length);

/** Callback for receiving state updater data from server.
 * Applications must implement this callback in the target enviroment.
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
kii_socket_code_t state_updater_recv_cb(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length);

/** Callback for closing state updater socket.
 * Applications must implement this callback in the target enviroment.
 *
 * @param [in] socket_context context object.
 *
 * @return If applications succeed to close socket, applications need
 * to return KII_SOCKETC_OK. If applications fail to close socket,
 * applications need to return KII_SOCKETC_FAIL. If applications want
 * to pend returning success or fail, applications need to return
 * KII_SOCKETC_AGAIN.
 */
kii_socket_code_t state_updater_close_cb(kii_socket_context_t* socket_context);

/** Callback for creating command handler task.
 * Applications must implement this callback in the target enviroment.
 *
 * @param [in] name name of task.
 * @param [in] entry entry of task.
 * @param [in] stk_start start position of stack area.
 * @param [in] stk_size stack size of task
 * @param [in] priority priority of thisk
 *
 * @return KII_TASKC_OK if succeed to create task. otherwise KII_TASKC_FAIL.
 */
kii_task_code_t command_handler_task_create_cb(
        const char* name,
        KII_TASK_ENTRY entry,
        void* param,
        unsigned char* stk_start,
        unsigned int stk_size,
        unsigned int priority);

/** Callback for delaying command handler process.
 * Applications must implement this callback in the target enviroment.
 *
 * @param[in] msec millisecond to delay.
 */
void command_handler_delay_ms_cb(unsigned int msec);

/** Callback for creating state updater task.
 * Applications must implement this callback in the target enviroment.
 *
 * @param [in] name name of task.
 * @param [in] entry entry of task.
 * @param [in] stk_start start position of stack area.
 * @param [in] stk_size stack size of task
 * @param [in] priority priority of thisk
 *
 * @return KII_TASKC_OK if succeed to create task. otherwise KII_TASKC_FAIL.
 */
kii_task_code_t state_updater_task_create_cb(
        const char* name,
        KII_TASK_ENTRY entry,
        void* param,
        unsigned char* stk_start,
        unsigned int stk_size,
        unsigned int priority);

/** Callback for delaying state updater process.
 * Applications must implement this callback in the target enviroment.
 *
 * @param[in] msec millisecond to delay.
 */
void state_updater_delay_ms_cb(unsigned int msec);

/** Callback for logging kii iot.
 * Applications must implement this callback in the target enviroment.
 *
 * @param [in] format outputted string format.
 */
void logger_cb(const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif /* _KII_IOT_CALLBACKS_ */
