ifdef DEBUG
CFLAGS += -g -DDEBUG
endif
CFLAGS += -std=gnu89 -Wall -pedantic -pthread -DKII_JSON_FIXED_TOKEN_NUM=256 -DKII_PUSH_KEEP_ALIVE_INTERVAL_SECONDS=30

LIBS = -lssl -lcrypto -lpthread
LD_FLAGS = -L..
SOURCES = $(wildcard *.c)
SOURCES += ../kii_thing_if.c ../kii_thing_if_environment_linux.c
SOURCES += $(wildcard ../kii/Linux/kii_*.c)
SOURCES += $(wildcard ../kii/kii-core/*.c)
SOURCES += $(wildcard ../kii/kii-core/linux/kii_core*.c)
SOURCES += $(wildcard ../kii/kii/*.c)
SOURCES += $(wildcard ../kii/kii_json/src/*.c)
SOURCES += ../kii/lib/jsmn/jsmn.c
TARGET = exampleapp
INCLUDES = -I.. -I../kii/kii-core -I../kii/kii-core/linux -I../kii/kii -I../kii/kii_json/include -I../kii/lib/jsmn -I../kii/Linux


all: clean $(TARGET) $(DOCTARGET)

$(TARGET):
	gcc $(CFLAGS) $(SOURCES) $(LIBS) $(LD_FLAGS) $(INCLUDES) -o $@

clean:
	touch $(TARGET)
	rm $(TARGET)
	rm -rf $(DOCTARGET)

.PHONY: all clean copy
