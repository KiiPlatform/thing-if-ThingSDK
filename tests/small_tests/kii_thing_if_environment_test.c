#include "kii_thing_if_environment_impl.h"

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

kii_socket_code_t socket_connect_cb_impl(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port)
{
    return KII_SOCKETC_FAIL;
}

kii_socket_code_t socket_send_cb_impl(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length)
{
    return KII_SOCKETC_FAIL;
}

kii_socket_code_t socket_recv_cb_impl(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length)
{
    return KII_SOCKETC_FAIL;
}

kii_socket_code_t socket_close_cb_impl(kii_socket_context_t* socket_context)
{
    return KII_SOCKETC_FAIL;
}

kii_socket_code_t mqtt_connect_cb_impl(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port)
{
    return KII_SOCKETC_FAIL;
}

kii_socket_code_t mqtt_send_cb_impl(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length)
{
    return KII_SOCKETC_FAIL;
}

kii_socket_code_t mqtt_recv_cb_impl(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length)
{
    return KII_SOCKETC_FAIL;
}

kii_socket_code_t mqtt_close_cb_impl(kii_socket_context_t* socket_context)
{
    return KII_SOCKETC_FAIL;
}

kii_task_code_t task_create_cb_impl(
        const char* name,
        KII_TASK_ENTRY entry,
        void* param)
{
    return KII_TASKC_FAIL;
}

void delay_ms_cb_impl(unsigned int msec)
{
    assert(0);
}

void logger_cb_impl(const char* format, ...)
{
    va_list list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
}

