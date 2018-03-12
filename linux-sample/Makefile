ifdef DEBUG
CFLAGS += -g -DDEBUG
endif
CFLAGS += -Wall -pedantic -pthread

LIBS = -lssl -lcrypto -lpthread -lkiithingifsdk
# Uncomment and edit if you install sdk in custom directory.
#LD_FLAGS = -L../build/usr/local/lib
SOURCES = $(wildcard *.c)
SOURCES += $(wildcard linux-env/*.c)
TARGET = exampleapp
# Uncomment and edit if you install sdk in custom directory.
#INCLUDES = -I../build/usr/local/include
INCLUDES += -I/usr/local/opt/openssl/include/

all: clean $(TARGET)

$(TARGET):
	gcc $(CFLAGS) $(SOURCES) $(LIBS) $(LD_FLAGS) $(INCLUDES) -o $@

clean:
	touch $(TARGET)
	rm $(TARGET)

.PHONY: all clean
