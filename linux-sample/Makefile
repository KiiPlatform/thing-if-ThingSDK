ifdef DEBUG
CFLAGS += -g -DDEBUG
endif
CFLAGS += -Wall -pedantic -pthread

LIBS = -lssl -lcrypto -lpthread -lkiithingifsdk
LD_FLAGS = -L../build/usr/local/lib
SOURCES = $(wildcard *.c)
SOURCES += $(wildcard linux-env/*.c)
TARGET = exampleapp
INCLUDES = -I../build/usr/local/include
INCLUDES += -I/usr/local/opt/openssl/include/

all: clean $(TARGET)

$(TARGET):
	gcc $(CFLAGS) $(SOURCES) $(LIBS) $(LD_FLAGS) $(INCLUDES) -o $@

clean:
	touch $(TARGET)
	rm $(TARGET)

.PHONY: all clean
