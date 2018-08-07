SDK_BUILD_PATH=./build
SDK_PREFIX=usr/local
INSTALL_PATH=$(SDK_BUILD_PATH)/$(SDK_PREFIX)

CMAKE_BUILD_TYPE = RELEASE
ifdef DEBUG
CFLAGS += -g -DDEBUG
CMAKE_BUILD_TYPE = DEBUG
endif

CFLAGS += -Wall -pedantic -pthread

LIBS = -lssl -lcrypto -lpthread -lkiithingifsdk -lwiringPi
LD_FLAGS = -L$(INSTALL_PATH)/lib
# On Mac using homebrew.
LD_FLAGS += -L/usr/local/opt/openssl/lib
SOURCES = $(wildcard *.c)
SOURCES += $(wildcard linux-env/*.c)
TARGET = exampleapp
INCLUDES = -I$(INSTALL_PATH)/include
# On Mac using homebrew.
INCLUDES += -I/usr/local/opt/openssl/include/

all: clean $(TARGET)

sdk:
	touch $(SDK_BUILD_PATH)
	rm -rf $(SDK_BUILD_PATH)
	mkdir $(SDK_BUILD_PATH)
	cd $(SDK_BUILD_PATH) && cmake -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) ../../ && make && make DESTDIR=. install

$(TARGET): sdk
	gcc $(CFLAGS) $(SOURCES) $(LIBS) $(LD_FLAGS) $(INCLUDES) -o $@

clean:
	touch $(TARGET)
	rm $(TARGET)
	touch $(SDK_BUILD_PATH)
	rm -rf $(SDK_BUILD_PATH)

.PHONY: all clean sdk
