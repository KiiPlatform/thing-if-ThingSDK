SDK_BUILD_PATH=./build
SDK_PREFIX=usr/local
INSTALL_PATH=$(BUILD_DIR)/$(SDK_PREFIX)

CMAKE_BUILD_TYPE = RELEASE
ifdef DEBUG
CFLAGS += -g -DDEBUG
CMAKE_BUILD_TYPE = DEBUG
endif

CFLAGS += -Wall -pedantic -pthread

LIBS = -lssl -lcrypto -lpthread -lkiithingifsdk
LD_FLAGS = -L$(INSTALL_PATH)/lib
SOURCES = $(wildcard *.c)
SOURCES += $(wildcard linux-env/*.c)
TARGET = exampleapp
INCLUDES = -I$(INSTALL_PATH)/include
INCLUDES += -I/usr/local/opt/openssl/include/

all: clean $(TARGET)

sdk:
	touch $(SDK_BUILD_PATH)
	rm -rf $(SDK_BUILD_PATH)
	mkdir $(SDK_BUILD_PATH)
	cd $(SDK_BUILD_PATH) && cmake -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) ../../ && make && make DESTDIR=$(SDK_BUILD_PATH) install

$(TARGET): sdk
	gcc $(CFLAGS) $(SOURCES) $(LIBS) $(LD_FLAGS) $(INCLUDES) -o $@

clean:
	touch $(TARGET)
	rm $(TARGET)
	touch $(SDK_BUILD_PATH)
	rm -rf $(SDK_BUILD_PATH)

.PHONY: all clean sdk
