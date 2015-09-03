#ifndef _KII_IOT_CALLBACKS_
#define _KII_IOT_CALLBACKS_

#ifdef __cplusplus
extern "C" {
#endif

#include <kii_socket_callback.h>
#include <kii_task_callback.h>

/** Callback for connecting socket to HTTP or HTTPS server.
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
kii_socket_code_t http_connect(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port);

/** Callback for sending data to HTTP or HTTPS server.
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
kii_socket_code_t http_send(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length);

/** Callback for receiving data from HTTP or HTTPS server.
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
kii_socket_code_t http_recv(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length);

/** Callback for closing HTTP or HTTPS socket.
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
kii_socket_code_t http_close(
        kii_socket_context_t* socket_context);

/** Callback for connecting MQTT socket to server.
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
kii_socket_code_t mqtt_connect(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port);

/** Callback for sending MQTT data to server.
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
kii_socket_code_t mqtt_send(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length);

/** Callback for receiving MQTT data from server.
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
kii_socket_code_t mqtt_recv(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length);

/** Callback for closing MQTT socket.
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
kii_socket_code_t mqtt_close(kii_socket_context_t* socket_context);

/** Callback for creating task.
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
kii_task_code_t task_create(
        const char* name,
        KII_TASK_ENTRY entry,
        void* param,
        unsigned char* stk_start,
        unsigned int stk_size,
        unsigned int priority);

/** Callback for delaying task.
 * Applications must implement this callback in the target enviroment.
 *
 * @param[in] msec millisecond to delay.
 */
void delay_ms(unsigned int msec);

/** Callback for logging kii iot.
 * Applications must implement this callback in the target enviroment.
 *
 * @param [in] format outputted string format.
 */
void logger(const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif /* _KII_IOT_CALLBACKS_ */
