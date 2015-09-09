#include "kii_iot_environment_impl.h"

#include <kii_core_secure_socket.h>
#include <kii_socket_impl.h>
#include <kii_task_impl.h>

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
    return connect_cb(socket_context, host, port);
}

kii_socket_code_t mqtt_send_cb_impl(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length)
{
    return send_cb(socket_context, buffer, length);
}

kii_socket_code_t mqtt_recv_cb_impl(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length)
{
    return recv_cb(socket_context, buffer, length_to_read, out_actual_length);
}

kii_socket_code_t mqtt_close_cb_impl(kii_socket_context_t* socket_context)
{
    return close_cb(socket_context);
}

kii_task_code_t task_create_cb_impl(
        const char* name,
        KII_TASK_ENTRY entry,
        void* param,
        unsigned char* stk_start,
        unsigned int stk_size,
        unsigned int priority)
{
    return task_create_cb(name, entry, param, stk_start, stk_size, priority);
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

