override CFLAGS += -std=gnu89 -Wall -pedantic -pthread -DKII_JSON_FIXED_TOKEN_NUM=256
ifdef DEBUG
CFLAGS += -g -DDEBUG
endif

LIBS = -lssl -lcrypto -lpthread
LD_FLAGS = -L..
SOURCES = $(wildcard *.c)
SOURCES += $(wildcard ../*.c)
SOURCES += $(wildcard ../kii/Linux/kii_*.c)
SOURCES += $(wildcard ../kii/kii-core/*.c)
SOURCES += $(wildcard ../kii/kii-core/linux/kii_core*.c)
SOURCES += $(wildcard ../kii/kii/*.c)
SOURCES += $(wildcard ../kii/kii_json/src/*.c)
SOURCES += ../kii/lib/jsmn/jsmn.c
TARGET = exampleapp
INCLUDES = -I.. -I../kii/kii -I../kii/kii-core -I../kii/kii_json/include -I../kii/kii-core/linux -I../kii/Linux -I../kii/lib/jsmn

all: clean $(TARGET) $(DOCTARGET)

$(TARGET):
	gcc $(CFLAGS) $(SOURCES) $(LIBS) $(LD_FLAGS) $(INCLUDES) -o $@

clean:
	touch $(TARGET)
	rm $(TARGET)
	rm -rf $(DOCTARGET)

.PHONY: all clean copy
