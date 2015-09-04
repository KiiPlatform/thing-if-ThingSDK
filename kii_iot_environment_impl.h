/**
 * @file kii_iot_environment_dependent.h
 * @brief This is a file defining environment dependent functions. Kii
 * IoT ThingSDK requires to implement these functions in each target
 * environment.
 */
#ifndef _KII_IOT_ENVIRONMENT_DEPENDENT_
#define _KII_IOT_ENVIRONMENT_DEPENDENT_

#ifdef __cplusplus
extern "C" {
#endif

#include <kii_socket_callback.h>
#include <kii_task_callback.h>

/** Function to connect socket to HTTP or HTTPS server.
 * Kii IoT ThingSDK requirest to implement this function in each
 * target environment.
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

/** Function to send data to HTTP or HTTPS server.
 * Kii IoT ThingSDK requirest to implement this function in each
 * target environment.
 *
 * @param [in] socket_context context object.
 * @param [in] buffer data to send server.
 * @param [in] length length of buffer.

 * @return If applications succeed to send data, applications need to
 * return KII_SOCKETC_OK. If connection is failed. applications need
 * to return KII_SOCKETC__FAIL. If applications don't want to send
 * data, applications need to return KII_SOCKETC_AGAIN. In this case,
 * Kii IoT ThingSDK pass same data to this function again.
 */
kii_socket_code_t http_send(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length);

/** Function for receive data from HTTP or HTTPS server.
 * Kii IoT ThingSDK requirest to implement this function in each
 * target environment.
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

/** Function to close HTTP or HTTPS socket.
 * Kii IoT ThingSDK requirest to implement this function in each
 * target environment.
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

/** Function for connect MQTT socket to server.
 * Kii IoT ThingSDK requirest to implement this function in each
 * target environment.
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

/** Function to send MQTT data to server.
 * Kii IoT ThingSDK requirest to implement this function in each
 * target environment.
 *
 * @param [in] socket_context context object.
 * @param [in] buffer data to send server.
 * @param [in] length length of buffer.

 * @return If applications succeed to send data, applications need to
 * return KII_SOCKETC_OK. If connection is failed. applications need
 * to return KII_SOCKETC__FAIL. If applications don't want to send
 * data, applications need to return KII_SOCKETC_AGAIN. In this case,
 * Kii IoT ThingSDK pass same data to this function again.
 */
kii_socket_code_t mqtt_send(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length);

/** Function to receive MQTT data from server.
 * Kii IoT ThingSDK requirest to implement this function in each
 * target environment.
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

/** Function to close MQTT socket.
 * Kii IoT ThingSDK requirest to implement this function in each
 * target environment.
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

/** Function to create task.
 * Kii IoT ThingSDK requirest to implement this function in each
 * target environment.
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

/** Function to delay task.
 * Kii IoT ThingSDK requirest to implement this function in each
 * target environment.
 *
 * @param[in] msec millisecond to delay.
 */
void delay_ms(unsigned int msec);

/** Function to log kii iot.
 * Kii IoT ThingSDK requirest to implement this function in each
 * target environment.
 *
 * @param [in] format outputted string format.
 */
void logger(const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif /* _KII_IOT_ENVIRONMENT_DEPENDENT_ */
