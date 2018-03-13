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
$ mkdir build
$ cd build
$ make
```

### Install

After the build, you can install SDK libraries by following commands.
(Assume current directory is build you've created on build.)

```sh
$ make install
```

Or if you want to change install location:

```sh
$ make DESTDIR={path to install} install
```

By specifying `DESTDIR`, libraries and headers are installed in {path to install}/usr/local/lib and {path to install}/usr/local/include respectively.


### Build Configuration

#### Debug Build
If you need to enable -g option and debug log output, following command will generate the debug build.

```sh
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=DEBUG ../
$ make
```

#### Configure MQTT keep-alive interval

Edit CMakeLists.txt and change following line.

```
SET(CMAKE_C_FLAGS "-DKII_PUSH_KEEP_ALIVE_INTERVAL_SECONDS=300")
```

Unit is seconds and by default it is set to 300 seconds.
If 0 is specified, turn off the keep alive mechanism defined in MQTT.
Setting short period will increase the load of MQTT broker when there is a lot
of connected devices.
For details, please refer to the [MQTT spec](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc385349238).

#### Configure memory used by JSON parser.

Edit CMakeLists.txt and change following line.

```
SET(CMAKE_C_FLAGS "-DKII_JSON_FIXED_TOKEN_NUM=128")
```

The number indicates amounts of memory used to
parse JSON when received it from server.
Memory is allocated on stack.
By default it is set to 128.
Size of memory allocated by this can be calculated as following.

```c
sizeof(kii_json_token_t) * KII_JSON_FIXED_TOKEN_NUM
```

#### Dynamic memory allocation for JSON parsing
If you prefer to dynamic allocation than fixed memory allocation,
Edit CMakeLists.txt and remove the line specifies `KII_JSON_FIXED_TOKEN_NUM`.

In this case, you need to implement `KII_JSON_RESOURCE_CB` function and set the
pointer in `kii_t` struct.

#### Connect MQTT server using non-secure connection.
By default SDK connect to a MQTT broker using secure connection(tls).
If you need to use non-secure connection(tcp),
Edit CMakeLists.txt and enable following line.
(By removing # in the head.)

```
SET(CMAKE_C_FLAGS "-DKII_MQTT_USE_PORT_TCP=1")
```

## How to use

### Sample
[linux-sample](./linux-sample) shows how to implement the client application works in the Thing.

### Environment implementations

SDK requires application to implement callback functions.

#### Socket callback functions:

To communucate with server, socket API implmementation is required.
Please refer to [linux-sample/sys\_cb\_impl.h](./linux-sample/sys_cb_impl.h)
and [linux-sample/sys\_cb\_linux.c](./linux-sample/sys_cb_linux.c)

Actual implementations are in [linux-env](./linux-sample/linux-env)
directory.

#### Task callback functions:

To handle tasks asynchronusly, task implementation is required.
`pthread` live up to expectation in linux environment and `task` does in RTOS environments.

List of task name symbols.

- `KII_THING_IF_TASK_NAME_UPDATE_STATUS`

  The task of sending onboarded status.

- `KII_TASK_NAME_RECV_MSG`

  The task of receiving push messages.

- `KII_TASK_NAME_PING_REQ`

  The task of sending ping request.

In RTOS, You'll specify stack size, depth and priority, etc depending on the environment.

## Execute tests

```sh
# cd tests/small_tests
# make test
```
