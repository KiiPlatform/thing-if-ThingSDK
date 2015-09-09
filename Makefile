override CFLAGS += -std=gnu89 -Wall -pedantic -pthread -fPIC -shared

ifdef DEBUG
	CFLAGS += -g -DDEBUG
endif

LIBS = -lssl -lcrypto -lpthread
SOURCES = kii_iot.c
SOURCES += $(wildcard kii/Linux/kii_*.c)
SOURCES += $(wildcard kii/kii-core/*.c)
SOURCES += $(wildcard kii/kii-core/linux/kii_core*.c)
SOURCES += $(wildcard kii/kii/*.c)
SOURCES += $(wildcard kii/kii_json/src/*.c)
SOURCES += kii/lib/jsmn/jsmn.c
INCLUDES = -Ikii/kii-core -Ikii/kii-core/linux -Ikii/kii -Ikii/kii_json/include -Ikii/lib/jsmn -Ikii/Linux

TARGET = libkiiiotsdk.so

all: linux

linux: clean
	gcc $(CFLAGS) $(SOURCES) kii_iot_environment_linux.c $(LIBS) $(INCLUDES) -o $(TARGET)

clean:
	touch $(TARGET)
	rm -rf $(TARGET) html latex

doc:
	doxygen Doxyfile

.PHONY: clean
