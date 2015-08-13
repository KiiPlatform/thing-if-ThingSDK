override CFLAGS += -std=gnu89 -Wall -pedantic -pthread -DKII_JSON_FIXED_TOKEN_NUM=256
ifdef DEBUG
CFLAGS += -g -DDEBUG
endif

IOTSDKPARAM =
ifdef DEBUG
IOTSDKPARAM += DEBUG=1
endif

LIBS = -lssl -lcrypto -lpthread -lkiiiotsdk
LD_FLAGS = -L..
SOURCES = $(wildcard *.c)
TARGET = exampleapp
INCLUDES = -I.. -I../kii/kii -I../kii/kii-core -I../kii/kii_json/include -I../kii/lib/jsmn

IOTSDK = libkiiiotsdk.so

all: clean $(TARGET) $(DOCTARGET)

$(IOTSDK):
	$(MAKE) -C .. $(IOTSDKPARAM) "CFLAGS+=-DKII_JSON_FIXED_TOKEN_NUM=256"
	ln -s ../libkiiiotsdk.so libkiiiotsdk.so


$(TARGET): $(IOTSDK)
	gcc $(CFLAGS) $(SOURCES) $(LIBS) $(LD_FLAGS) $(INCLUDES) -o $@

clean:
	touch $(TARGET)
	rm $(TARGET)
	rm -rf $(DOCTARGET)
	touch $(IOTSDK)
	rm -rf $(IOTSDK)

.PHONY: all clean copy
