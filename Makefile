override CFLAGS += -std=gnu89 -Wall -pedantic -pthread -fPIC -shared

ifdef DEBUG
	CFLAGS += -g -DDEBUG
endif

LIBS = -lssl -lcrypto -lpthread
SOURCES = $(wildcard *.c)
SOURCES += $(wildcard kii/Linux/kii_*.c)
SOURCES += $(wildcard kii/kii-core/*.c)
SOURCES += $(wildcard kii/kii/*.c)
SOURCES += $(wildcard kii/kii_json/src/*.c)
SOURCES += kii/lib/jsmn/jsmn.c
INCLUDES = -Ikii/kii-core -Ikii/kii -Ikii/kii_json/include -Ikii/lib/jsmn -Ikii/Linux

TARGET = libkiiiotsdk.so

all: clean $(TARGET)

$(TARGET):
	gcc $(CFLAGS) $(SOURCES) $(LIBS) $(INCLUDES) -o $@

clean:
	touch $(TARGET)
	rm -rf $(TARGET) html latex

doc:
	doxygen Doxyfile

.PHONY: clean
