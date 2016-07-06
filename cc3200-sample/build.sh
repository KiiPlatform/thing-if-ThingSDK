#!/bin/sh
rm -rf target
mkdir -p target/freertos_thingsdk_demo
cp -rf freertos_thingsdk_demo/ccs  target/freertos_thingsdk_demo
cp freertos_thingsdk_demo/kii_thing_if_environment_cc3200.c target/freertos_thingsdk_demo
cp freertos_thingsdk_demo/main.c target/freertos_thingsdk_demo
cp freertos_thingsdk_demo/pinmux.c target/freertos_thingsdk_demo
cp freertos_thingsdk_demo/pinmux.h target/freertos_thingsdk_demo
cp ../kii/kii-core/kii_core.c target/freertos_thingsdk_demo
cp ../kii/kii-core/kii_core.h target/freertos_thingsdk_demo
cp ../kii/kii-core/kii_libc_wrapper.c target/freertos_thingsdk_demo
cp ../kii/kii-core/kii_libc_wrapper.h target/freertos_thingsdk_demo
cp ../kii/kii-core/kii_socket_callback.h target/freertos_thingsdk_demo
cp ../kii/kii_json/libs/jsmn/jsmn.c target/freertos_thingsdk_demo
cp ../kii/kii_json/libs/jsmn/jsmn.h target/freertos_thingsdk_demo
cp ../kii/kii_json/include/kii_json.h target/freertos_thingsdk_demo
cp ../kii/kii_json/src/kii_json.c target/freertos_thingsdk_demo
cp ../kii/kii/kii.h target/freertos_thingsdk_demo
cp ../kii/kii/kii_call.c target/freertos_thingsdk_demo
cp ../kii/kii/kii_core_hidden.h target/freertos_thingsdk_demo
cp ../kii/kii/kii_json_utils.c target/freertos_thingsdk_demo
cp ../kii/kii/kii_json_utils.h target/freertos_thingsdk_demo
cp ../kii/kii/kii_mqtt.c target/freertos_thingsdk_demo
cp ../kii/kii/kii_mqtt.h target/freertos_thingsdk_demo
cp ../kii/kii/kii_object.c target/freertos_thingsdk_demo
cp ../kii/kii/kii_push.c target/freertos_thingsdk_demo
cp ../kii/kii/kii_server_code.c target/freertos_thingsdk_demo
cp ../kii/kii/kii_task_callback.h target/freertos_thingsdk_demo
cp ../kii/kii/kii_thing.c target/freertos_thingsdk_demo
cp ../kii_hidden.h target/freertos_thingsdk_demo
cp ../kii_thing_if.c target/freertos_thingsdk_demo
cp ../kii_thing_if.h target/freertos_thingsdk_demo
cp ../kii_thing_if_environment_impl.h target/freertos_thingsdk_demo
patch -p0 < ccs.patch
cd target;zip freertos_thingsdk_demo.zip -r freertos_thingsdk_demo
