ifdef DEBUG
CFLAGS += -g -DDEBUG
endif
CFLAGS += -Wall -pedantic -pthread

ifndef KEEP_ALIVE_INTERVAL
	override CFLAGS += -DKII_PUSH_KEEP_ALIVE_INTERVAL_SECONDS=300
else
	override CFLAGS += -DKII_PUSH_KEEP_ALIVE_INTERVAL_SECONDS=$(KEEP_ALIVE_INTERVAL)
endif

ifdef KII_MQTT_USE_PORT_TCP
	override CFLAGS += -DKII_MQTT_USE_PORT_TCP=$(KII_MQTT_USE_PORT_TCP)
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

LIBS = -lssl -lcrypto -lpthread -lkiithingifsdk
LD_FLAGS = -L.
SOURCES = $(wildcard *.c)
TARGET = exampleapp
INCLUDES = -I.. -I../kii/kii-core -I../kii/kii-core/linux -I../kii/kii -I../kii/kii_json/include -I../kii/lib/jsmn -I../kii/Linux -I/usr/local/opt/openssl/include/


all: clean sdk $(TARGET) $(DOCTARGET)

sdk:
	$(MAKE) -C .. linux "CFLAGS+=$(CFLAGS)"
	cp ../libkiithingifsdk.so ./

$(TARGET):
	gcc $(CFLAGS) $(SOURCES) $(LIBS) $(LD_FLAGS) $(INCLUDES) -o $@

clean:
	$(MAKE) -C .. clean
	rm -f libkiithingifsdk.so
	touch $(TARGET)
	rm $(TARGET)
	rm -rf $(DOCTARGET)

.PHONY: all clean copy
