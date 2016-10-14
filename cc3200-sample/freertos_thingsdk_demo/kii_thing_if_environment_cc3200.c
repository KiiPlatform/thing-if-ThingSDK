#include "kii_thing_if_environment_impl.h"

#include <stdio.h>
#include <stdarg.h>

#include "kii_thing_if.h"

#include "simplelink.h"
#ifndef NOTERM
#include "uart_if.h"
#endif

kii_socket_code_t socket_connect_cb_impl(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port)
{
    unsigned long destinationIP;
    SlSockAddrIn_t  addr;
    int sock;

    if(sl_NetAppDnsGetHostByName((signed char*)host, strlen(host),
                &destinationIP, SL_AF_INET) < 0){
        return KII_SOCKETC_FAIL;
    }
    memset(&addr, 0x00, sizeof(struct SlSockAddrIn_t));
    addr.sin_family = SL_AF_INET;
    addr.sin_port = sl_Htons(port);
    addr.sin_addr.s_addr = sl_Htonl(destinationIP);
    sock = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
    if (sock < 0) {
        return KII_SOCKETC_FAIL;
    }
    if (sl_Connect(sock, ( SlSockAddr_t *)&addr, sizeof(struct SlSockAddrIn_t)) < 0) {
        sl_Close(sock);
        return KII_SOCKETC_FAIL;
    }
    socket_context->sock = sock;
    return KII_SOCKETC_OK;
}

kii_socket_code_t socket_send_cb_impl(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length)
{
    int ret;
    int sock;

    Report("%.*s\r\n", length, buffer);
    sock = socket_context->sock;
    ret = sl_Send(sock, buffer, length, 0);
    if (ret > 0) {
        return KII_SOCKETC_OK;
    } else {
        return KII_SOCKETC_FAIL;
    }
}

kii_socket_code_t socket_recv_cb_impl(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length)
{
    int ret;
    int sock;

    sock = socket_context->sock;
    ret = sl_Recv(sock, buffer, length_to_read, 0);
    if (ret > 0) {
        *out_actual_length = ret;
        Report("%s", buffer);
        return KII_SOCKETC_OK;
    } else {
        return KII_SOCKETC_FAIL;
    }
}

kii_socket_code_t socket_close_cb_impl(kii_socket_context_t* socket_context)
{
    int sock;
    sock = socket_context->sock;

    sl_Close(sock);
    socket_context->sock = -1;
    return KII_SOCKETC_OK;
}

kii_socket_code_t mqtt_connect_cb_impl(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port)
{
    unsigned long destinationIP;
    SlSockAddrIn_t  addr;
    int sock;

    if(sl_NetAppDnsGetHostByName((signed char*)host, strlen(host),
                &destinationIP, SL_AF_INET) < 0){
        return KII_SOCKETC_FAIL;
    }
    memset(&addr, 0x00, sizeof(struct SlSockAddrIn_t));
    addr.sin_family = SL_AF_INET;
    addr.sin_port = sl_Htons(port);
    addr.sin_addr.s_addr = sl_Htonl(destinationIP);
    sock = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
    if (sock < 0) {
        return KII_SOCKETC_FAIL;
    }

#ifdef KII_PUSH_KEEP_ALIVE_INTERVAL_SECONDS
    {
        SlTimeval_t timeout;
        timeout.tv_sec = KII_PUSH_KEEP_ALIVE_INTERVAL_SECONDS * 2;
        timeout.tv_usec = 0;
        if (sl_SetSockOpt(sock, SL_SOL_SOCKET, SL_SO_RCVTIMEO, &timeout,
                        sizeof(timeout)) != 0) {
            return KII_SOCKETC_FAIL;
        }
    }
#endif

    if (sl_Connect(sock, ( SlSockAddr_t *)&addr, sizeof(struct SlSockAddrIn_t)) < 0) {
        sl_Close(sock);
        return KII_SOCKETC_FAIL;
    }
    socket_context->sock = sock;
    return KII_SOCKETC_OK;
}

kii_socket_code_t mqtt_send_cb_impl(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length)
{
    int ret;
    int sock;

    sock = socket_context->sock;
    ret = sl_Send(sock, buffer, length, 0);
    if (ret > 0) {
        return KII_SOCKETC_OK;
    } else {
        return KII_SOCKETC_FAIL;
    }
}

kii_socket_code_t mqtt_recv_cb_impl(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length)
{
    int ret;
    int sock;

    sock = socket_context->sock;
    ret = sl_Recv(sock, buffer, length_to_read, 0);
    if (ret > 0) {
        *out_actual_length = ret;
        return KII_SOCKETC_OK;
    } else {
        return KII_SOCKETC_FAIL;
    }
}

kii_socket_code_t mqtt_close_cb_impl(kii_socket_context_t* socket_context)
{
    int sock;
    sock = socket_context->sock;

    sl_Close(sock);
    socket_context->sock = -1;
    return KII_SOCKETC_OK;
}

kii_task_code_t task_create_cb_impl(
        const char* name,
        KII_TASK_ENTRY entry,
        void* param)
{
    unsigned int stk_size = 0;
    unsigned int priority = 0;

    if (strcmp(name, KII_THING_IF_TASK_NAME_UPDATE_STATUS) == 0) {
        stk_size = 2048;
        priority = 1;
    } else if (strcmp(name, KII_TASK_NAME_RECV_MSG) == 0) {
        stk_size = 4096;
#ifdef KII_PUSH_KEEP_ALIVE_INTERVAL_SECONDS
    } else if (strcmp(name, KII_TASK_NAME_PING_REQ) == 0) {
        stk_size = 1024;
#endif
    }

    if (osi_TaskCreate(entry, (const signed char*) name, stk_size, param, priority, NULL) < 0) {
        return KII_TASKC_FAIL;
    } else {
        return KII_TASKC_OK;
    }
}

void delay_ms_cb_impl(unsigned int msec)
{
    osi_Sleep(msec);
}

void logger_cb_impl(const char* format, ...)
{
    va_list list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
}

