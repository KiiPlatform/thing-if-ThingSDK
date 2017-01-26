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
In the Debug build, Debug log is enabled.

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

