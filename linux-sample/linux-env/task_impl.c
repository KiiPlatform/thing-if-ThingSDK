#include "kii_task_impl.h"
#include <pthread.h>
#include <unistd.h>

kii_task_code_t task_create_cb
    (const char* name,
     KII_TASK_ENTRY entry,
     void* param)
{
    int ret;
    pthread_t pthid;

    ret = pthread_create(&pthid, NULL, entry, param);

    if(ret == 0)
    {
        return KII_TASKC_OK;
    }
    else
    {
        return KII_TASKC_FAIL;
    }
}

void delay_ms_cb(unsigned int msec)
{
    usleep(msec * 1000);
}
/* vim:set ts=4 sts=4 sw=4 et fenc=UTF-8 ff=unix: */
