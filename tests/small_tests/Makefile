ifdef DEBUG
	override CFLAGS += -g -DDEBUG
endif
override CFLAGS += -Wall -pedantic -pthread -fPIC -shared

THINGIF_PARAM = KII_THING_IF_TEST_BUILD=1
ifdef DEBUG
THINGIF_PARAM += DEBUG=1
endif

LIBS = -lssl -lcrypto -lpthread -lkiithingifsdk
LD_FLAG = -L. -L/usr/local/opt/openssl/lib
LIBTHINGIF=libkiithingifsdk.so
INCLUDES = -I../../ -I../../kii/kii -I../../kii/kii-core -I../../kii/kii_json/include -I../../kii/lib/jsmn

GTEST_PATH=../../kii/kii-core/gtest-1.7.0
LIBGTEST=libgtest.a

TEST_SOURCES = handle_command_test.c

TARGET=testapp

build: all

all: clean $(LIBTHINGIF) $(LIBGTEST) $(TARGET)

$(LIBTHINGIF):
	make -C ../../ $(THINGIF_PARAM)
	cp ../../libkiithingifsdk.so ./

$(LIBGTEST):
	g++ -isystem $(GTEST_PATH)/include -I$(GTEST_PATH) -pthread -c $(GTEST_PATH)/src/gtest-all.cc
	g++ -isystem $(GTEST_PATH)/include -I$(GTEST_PATH) -pthread -c $(GTEST_PATH)/src/gtest_main.cc
	ar -rv $(LIBGTEST) gtest-all.o gtest_main.o


$(TARGET):
	g++ -pthread -o $(TARGET) $(TEST_SOURCES) $(LIBS) $(LD_FLAG) $(LIBGTEST) $(INCLUDES) -I$(GTEST_PATH)/include

clean:
	touch $(LIBTHINGIF)
	rm -rf $(LIBTHINGIF) $(LIBGTEST) doc a.out *.o

test:
	export LD_LIBRARY_PATH=. && ./$(TARGET)

.PHONY: clean
