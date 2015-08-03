override CFLAGS += -std=gnu89 -Wall -pedantic -pthread -fPIC -shared

ifdef DEBUG
	CFLAGS += -g -DDEBUG
endif

LIBS = -lssl -lcrypto -lpthread
SOURCES = $(wildcard *.c)
SOURCES += $(wildcard v1-prototype/kii/Linux/kii_*.c)
SOURCES += $(wildcard v1-prototype/kii/kii-core/*.c)
SOURCES += $(wildcard v1-prototype/kii/kii/*.c)
SOURCES += $(wildcard v1-prototype/kii/kii_json/src/*.c)
SOURCES += v1-prototype/kii/lib/jsmn/jsmn.c
INCLUDES = -Iv1-prototype/kii/kii-core -Iv1-prototype/kii/kii -Iv1-prototype/kii/kii_json/include -Iv1-prototype/kii/lib/jsmn -Iv1-prototype/kii/Linux

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
