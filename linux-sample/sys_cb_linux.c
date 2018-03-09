#include "sys_cb_impl.h"
#include "linux-env/task_impl.h"
#include "linux-env/secure_socket_impl.h"

#include <stdio.h>
#include <stdarg.h>

kii_socket_code_t socket_connect_cb_impl(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port)
{
    return s_connect_cb(socket_context, host, port);
}

kii_socket_code_t socket_send_cb_impl(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length)
{
    return s_send_cb(socket_context, buffer, length);
}

kii_socket_code_t socket_recv_cb_impl(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length)
{
    return s_recv_cb(socket_context, buffer, length_to_read, out_actual_length);
}

kii_socket_code_t socket_close_cb_impl(kii_socket_context_t* socket_context)
{
    return s_close_cb(socket_context);
}

kii_socket_code_t mqtt_connect_cb_impl(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port)
{
#ifdef KII_MQTT_USE_PORT_TCP
    return mqtt_connect_cb(socket_context, host, port);
#else
    return s_connect_cb(socket_context, host, port);
#endif
}

kii_socket_code_t mqtt_send_cb_impl(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length)
{
#ifdef KII_MQTT_USE_PORT_TCP
    return mqtt_send_cb(socket_context, buffer, length);
#else
    return s_send_cb(socket_context, buffer, length);
#endif
}

kii_socket_code_t mqtt_recv_cb_impl(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length)
{
#ifdef KII_MQTT_USE_PORT_TCP
    return mqtt_recv_cb(socket_context, buffer, length_to_read, out_actual_length);
#else
    return s_recv_cb(socket_context, buffer, length_to_read, out_actual_length);
#endif
}

kii_socket_code_t mqtt_close_cb_impl(kii_socket_context_t* socket_context)
{
#ifdef KII_MQTT_USE_PORT_TCP
    return mqtt_close_cb(socket_context);
#else
    return s_close_cb(socket_context);
#endif
}

kii_task_code_t task_create_cb_impl(
        const char* name,
        KII_TASK_ENTRY entry,
        void* param)
{
    return task_create_cb(name, entry, param);
}

void delay_ms_cb_impl(unsigned int msec)
{
    delay_ms_cb(msec);
}

void logger_cb_impl(const char* format, ...)
{
    va_list list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
}

