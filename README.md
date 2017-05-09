# Kii thing-if SDK for Thing
Thing SDK for Kii Thing Interaction Framework (kii thing-if) .<br>

## Retrieve submodules.

When you build this SDK for the first time, you need to
execute following commands:

```sh
# git submodule init
# git submodule update
# cd kii
# git submodule init
# git submodule update
# cd ../
```

These commands retrieve required modules for this SDK.

## Build

```sh
# make
```

This makes libkiithingifsdk.so.

## Debug Build
```sh
# make DEBUG=1
```

Standard build for debugging.

- KII\_PUSH\_KEEP\_ALIVE\_INTERVAL\_SECONDS is set to 300 seconds.
- KII\_JSON\_FIXED\_TOKEN\_NUM is set to 128.
- Debug option (-g) and Debug log enabled.


## Configure KII\_PUSH\_KEEP\_ALIVE\_INTERVAL\_SECONDS

KII\_PUSH\_KEEP\_ALIVE\_INTERVAL\_SECONDS macro is used to manage the Keep Alive
Time Interval in MQTT.
By default it is set to 300 seconds in the Makefile.
To override the value, we provides `KEEP_ALIVE_INTERVAL` variable.

```
# make KEEP_ALIVE_INTERVAL=600
```

If 0 is specified, turn off the keep alive mechanism defined in MQTT.
Setting short period will increase the load of MQTT broker when there is a lot
of connected devices.
For details, please refer to the [MQTT spec](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc385349238).

## Configure KII\_JSON\_FIXED\_TOKEN\_NUM

KII\_JSON\_FIXED\_TOKEN\_NUM  macro is used to manage amount memory used to
parse JSON when received it from server.
Memory is allocated on stack.
By default it is set to 128 in the Makefile.
To override the value, we provides `FIXED_JSON_TOKEN_NUM` variable.

```
# make FIXED_JSON_TOKEN_NUM=256
```

Size of memory allocated by this can be calculated as following.
```c
sizeof(kii_json_token_t) * KII_JSON_FIXED_TOKEN_NUM
```
## Dynamic memory allocation for JSON parsing
If you prefer to dynamica allocation than fixed memory allocation by
KII\_JSON\_FIXED\_TOKEN\_NUM, You will build sdk with FLEXIBLE\_JSON\_TOKEN
variable.

```
# make FLEXIBLE_JSON_TOKEN=1
```

In this case, you need to implement KII\_JSON\_RESOURCE\_CB function and set the
pointer in kii\_t struct.

If both KII\_JSON\_FIXED\_TOKEN\_NUM and FLEXIBLE\_JSON\_TOKEN are specified,
KII\_JSON\_FIXED\_TOKEN\_NUM is ignored.

## Connect MQTT server using non-secure connection.
By default SDK connect to a MQTT broker using secure connection(tls).
If you need to use non-secure connection(tcp) , please set KII\_MQTT\_USE\_PORT\_TCP macro.

```
# make KII_MQTT_USE_PORT_TCP=1
```

## Combination of variables
Only KII\_JSON\_FIXED\_TOKEN\_NUM and FLEXIBLE\_JSON\_TOKEN is exclusive.
DEBUG, KEEP\_ALIVE\_INTERVAL, KII\_MQTT\_USE\_PORT\_TCP and either KII\_JSON\_FIXED\_TOKEN\_NUM or FLEXIBLE\_JSON\_TOKEN
can be combined.

ex.)

```
# make DEBUG=1 KEEP_ALIVE_INTERVAL=3600 KII_MQTT_USE_PORT_TCP=1 FLEXIBLE_JSON_TOKEN=1
```

## How to use
After the build finished, API Documentation is created under ./doc/html/index.html

## Sample
linux-sample shows how to implement the client application works in the Thing.

## Environment implementations

This SDK requests creating three tasks by task_create_cb_impl callback.  

List of task name symbols.

- KII\_THING\_IF\_TASK\_NAME\_UPDATE\_STATUS

  The task of sending onboarded status.

- KII\_TASK\_NAME\_RECV\_MSG

  The task of receiving push messages.

- KII\_TASK\_NAME\_PING\_REQ

  The task of sending ping request.

You'll specify stack size, depth and priority, etc depending on the environment.
Please check samples for reference.
For example, [Ti CC3200 environment implementation](./cc3200-sample/freertos_thingsdk_demo/kii_thing_if_environment_cc3200.c).

## Execute tests

```sh
# cd test/small_tests
# make run
```
