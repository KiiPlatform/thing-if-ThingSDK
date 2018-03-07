set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR armv5t)

set(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabi-gcc)

link_directories($ENV{HOME}/arm-local/lib)
include_directories($ENV{HOME}/arm-local/include)
