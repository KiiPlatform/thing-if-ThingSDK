ifdef DEBUG
	override CFLAGS += -g -DDEBUG
	override LIBTHINGIF_CFLAGS = -g -DDEBUG

endif
override CFLAGS += -Wall -pedantic -pthread -fPIC -shared
override LIBTHINGIF_CFLAGS += -DKII_THING_IF_TEST_BUILD -DKII_PUSH_KEEP_ALIVE_INTERVAL_SECONDS=300 -DKII_JSON_FIXED_TOKEN_NUM=128

DEFAULT_LIBS = -lssl -lcrypto -lpthread
LIBS = $(DEFAULT_LIBS) -lkiithingifsdk
THINGIF_LIBS= $(DEFAULT_LIBS)
LD_FLAG = -L.
LIBTHINGIF=libkiithingifsdk.so
INCLUDES = -I../../ -I../../include -I../../kii/kii -I../../kii/kii-core -I../../kii/kii_json/include -I../../kii/lib/jsmn

GTEST_PATH=../../kii/kii-core/gtest-1.7.0
LIBGTEST=libgtest.a

THINGIF_SOURCES = ../../kii_thing_if.c
THINGIF_SOURCES += $(wildcard ../../kii/kii-core/*.c)
THINGIF_SOURCES += $(wildcard ../../kii/kii-core/linux/kii_core*.c)
THINGIF_SOURCES += $(wildcard ../../kii/kii/*.c)
THINGIF_SOURCES += $(wildcard ../../kii/kii_json/src/*.c)
THINGIF_SOURCES += ../../kii/lib/jsmn/jsmn.c

EXTRA_THINGIF_SOURCES = kii_thing_if_environment_test.c

TEST_SOURCES = handle_command_test.c

TARGET=testapp

build: all

all: clean $(LIBTHINGIF) $(LIBGTEST) $(TARGET)

$(LIBTHINGIF):
	gcc $(CFLAGS) $(LIBTHINGIF_CFLAGS) $(THINGIF_SOURCES) $(EXTRA_THINGIF_SOURCES) $(THINGIF_LIBS) $(LD_FLAG) $(INCLUDES) -o $(LIBTHINGIF)

$(LIBGTEST):
	g++ -isystem $(GTEST_PATH)/include -I$(GTEST_PATH) -pthread -c $(GTEST_PATH)/src/gtest-all.cc
	g++ -isystem $(GTEST_PATH)/include -I$(GTEST_PATH) -pthread -c $(GTEST_PATH)/src/gtest_main.cc
	ar -rv $(LIBGTEST) gtest-all.o gtest_main.o


$(TARGET):
	g++ -pthread -o $(TARGET) $(TEST_SOURCES) $(LIBS) $(LD_FLAG) $(LIBGTEST) $(INCLUDES) -I$(GTEST_PATH)/include

clean:
	touch $(LIBTHINGIF)
	rm -rf $(LIBTHINGIF) $(LIBGTEST) doc $(TARGET) *.o

test:
	export LD_LIBRARY_PATH=. && ./$(TARGET)

run: build test

.PHONY: clean
