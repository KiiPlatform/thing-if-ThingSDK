#ifndef _KII_THING_IF_ENVIRONMENT_TEST_
#define _KII_THING_IF_ENVIRONMENT_TEST_

#include <kii_socket_callback.h>
#include <kii_task_callback.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kii_socket_test_context_t {
    kii_socket_code_t (*CONNECT)(
            void* context, const char* host, unsigned int port);
    kii_socket_code_t (*SEND)(
            void* context, const char* buffer, size_t length);
    kii_socket_code_t (*RECV)(
            void* context,
            char* buffer,
            size_t length_to_read,
            size_t* out_actual_length);
    kii_socket_code_t (*CLOSE)(void* context);

    void* context;
} kii_socket_test_context_t;

kii_socket_code_t socket_connect_cb_impl(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port);

kii_socket_code_t socket_send_cb_impl(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length);

kii_socket_code_t socket_recv_cb_impl(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length);

kii_socket_code_t socket_close_cb_impl(
        kii_socket_context_t* socket_context);

kii_socket_code_t mqtt_connect_cb_impl(
        kii_socket_context_t* socket_context,
        const char* host,
        unsigned int port);

kii_socket_code_t mqtt_send_cb_impl(
        kii_socket_context_t* socket_context,
        const char* buffer,
        size_t length);

kii_socket_code_t mqtt_recv_cb_impl(
        kii_socket_context_t* socket_context,
        char* buffer,
        size_t length_to_read,
        size_t* out_actual_length);

kii_socket_code_t mqtt_close_cb_impl(kii_socket_context_t* socket_context);

kii_task_code_t task_create_cb_impl(
        const char* name,
        KII_TASK_ENTRY entry,
        void* param);

void delay_ms_cb_impl(unsigned int msec);

void logger_cb_impl(const char* format, ...);

#ifdef __cplusplus
}
#endif



#endif /* _KII_THING_IF_ENVIRONMENT_TEST_ */
