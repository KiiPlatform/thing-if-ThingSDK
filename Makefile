ifdef DEBUG
	override CFLAGS += -g -DDEBUG
endif
override CFLAGS += -std=gnu89 -Wall -pedantic -pthread -fPIC -shared

ifndef KEEP_ALIVE_INTERVAL
	override CFLAGS += -DKII_PUSH_KEEP_ALIVE_INTERVAL_SECONDS=300
else
	override CFLAGS += -DKII_PUSH_KEEP_ALIVE_INTERVAL_SECONDS=$(KEEP_ALIVE_INTERVAL)
endif

ifndef FIXED_JSON_TOKEN_NUM
	JSON_FLAG = -DKII_JSON_FIXED_TOKEN_NUM=128
else
	JSON_FLAG = -DKII_JSON_FIXED_TOKEN_NUM=$(FIXED_JSON_TOKEN_NUM)
endif

ifdef FLEXIBLE_JSON_TOKEN
	JSON_FLAG =
endif

override CFLAGS += $(JSON_FLAG)

LIBS = -lssl -lcrypto -lpthread
SOURCES = kii_thing_if.c
SOURCES += $(wildcard kii/Linux/kii_*.c)
SOURCES += $(wildcard kii/kii-core/*.c)
SOURCES += $(wildcard kii/kii-core/linux/kii_core*.c)
SOURCES += $(wildcard kii/kii/*.c)
SOURCES += $(wildcard kii/kii_json/src/*.c)
SOURCES += kii/lib/jsmn/jsmn.c
INCLUDES = -Ikii/kii-core -Ikii/kii-core/linux -Ikii/kii -Ikii/kii_json/include -Ikii/lib/jsmn -Ikii/Linux

TARGET = libkiithingifsdk.so

all: linux doc

linux: clean
	gcc $(CFLAGS) $(SOURCES) kii_thing_if_environment_linux.c $(LIBS) $(INCLUDES) -o $(TARGET)

clean:
	touch $(TARGET)
	rm -rf $(TARGET) doc

doc:
	doxygen Doxyfile

.PHONY: clean doc
