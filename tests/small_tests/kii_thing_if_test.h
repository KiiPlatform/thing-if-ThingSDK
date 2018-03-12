#ifndef _KII_THING_IF_TEST_
#define _KII_THING_IF_TEST_

#ifdef __cplusplus
extern "C" {
#endif

struct kii_t;

void test_handle_command(struct kii_t* kii, char* buffer, size_t buffer_size);

#ifdef __cplusplus
}
#endif


#endif /* _KII_THING_IF_TEST_ */
