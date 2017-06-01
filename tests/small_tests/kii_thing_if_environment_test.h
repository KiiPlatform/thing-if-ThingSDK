#ifndef _KII_THING_IF_ENVIRONMENT_TEST_
#define _KII_THING_IF_ENVIRONMENT_TEST_

#include "kii_thing_if_environment_impl.h"


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

#ifdef __cplusplus
}
#endif



#endif /* _KII_THING_IF_ENVIRONMENT_TEST_ */
