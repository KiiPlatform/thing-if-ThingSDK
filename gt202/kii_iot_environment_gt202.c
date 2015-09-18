#include "kii_iot_environment_impl.h"

#include <gt202_kii_adapter.h>
#include <kii_mqtt_socket.h>
#include <kii_task_impl.h>

kii_socket_code_t socket_connect_cb_impl(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port)
{
    if (socket_context->app_context == NULL)
    {
        socket_context->app_context = (context_t*)malloc(sizeof(context_t));
    }
    return socket_connect_cb(socket_context, host, port);
}

kii_socket_code_t socket_send_cb_impl(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length)
{
    return socket_send_cb(socket_context, buffer, length);
}

kii_socket_code_t socket_recv_cb_impl(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length)
{
    return socket_recv_cb(socket_context, buffer, length_to_read,
            out_actual_length);
}

kii_socket_code_t socket_close_cb_impl(kii_socket_context_t* socket_context)
{
    kii_socket_code_t ret;

    ret = socket_close_cb(socket_context);

    if (socket_context->app_context != NULL)
    {
        free(socket_context->app_context);
        socket_context->app_context = NULL;
    }

    return ret;
}

kii_socket_code_t mqtt_connect_cb_impl(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port)
{
    if (socket_context->app_context == NULL)
    {
        socket_context->app_context = (context_t*)malloc(sizeof(context_t));
    }
    return mqtt_socket_connect(socket_context, host, port);
}

kii_socket_code_t mqtt_send_cb_impl(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length)
{
    return mqtt_socket_send(socket_context, buffer, length);
}

kii_socket_code_t mqtt_recv_cb_impl(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length)
{
    return mqtt_socket_recv(socket_context, buffer, length_to_read,
            out_actual_length);
}

kii_socket_code_t mqtt_close_cb_impl(kii_socket_context_t* socket_context)
{
    kii_socket_code_t ret;

    ret = mqtt_socket_close(socket_context);

    if (socket_context->app_context != NULL)
    {
        free(socket_context->app_context);
        socket_context->app_context = NULL;
    }

    return ret;
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
