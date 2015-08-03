override CFLAGS = -std=gnu89 -Wall -pedantic -pthread
#CFLAGS = -std=gnu89 -w -pedantic
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
INCLUDES = -I.. -I../v1-prototype/kii/kii -I../v1-prototype/kii/kii-core -I../v1-prototype/kii/kii_json/include -I../v1-prototype/kii/lib/jsmn

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
