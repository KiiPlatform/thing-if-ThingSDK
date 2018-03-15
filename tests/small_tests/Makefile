SDK_BUILD_PATH=./build
SDK_PREFIX=usr/local
INSTALL_PATH=$(SDK_BUILD_PATH)/$(SDK_PREFIX)

DEFAULT_LIBS = -lssl -lcrypto -lpthread
LIBS = $(DEFAULT_LIBS) -lkiithingifsdk
THINGIF_LIBS= $(DEFAULT_LIBS)
LD_FLAG = -L$(INSTALL_PATH)/lib
# Mac using homebrew
LD_FLAG += -L/usr/local/opt/openssl/lib
INCLUDES = -I$(INSTALL_PATH)/include
# Mac using homebrew
INCLUDES += -I/usr/local/opt/openssl/include

GTEST_PATH=../../kii/kii-core/gtest-1.7.0
LIBGTEST=libgtest.a

TEST_SOURCES = $(wildcard ./*.c)

TARGET=testapp

all: clean test

$(LIBGTEST):
	g++ -isystem $(GTEST_PATH)/include -I$(GTEST_PATH) -pthread -c $(GTEST_PATH)/src/gtest-all.cc
	g++ -isystem $(GTEST_PATH)/include -I$(GTEST_PATH) -pthread -c $(GTEST_PATH)/src/gtest_main.cc
	ar -rv $(LIBGTEST) gtest-all.o gtest_main.o


$(TARGET): sdk $(LIBGTEST)
	g++ -pthread -o $(TARGET) $(TEST_SOURCES) $(LIBS) $(LD_FLAG) $(LIBGTEST) $(INCLUDES) -I$(GTEST_PATH)/include

clean:
	rm -rf $(LIBGTEST) $(TARGET) *.o

sdk:
	touch $(SDK_BUILD_PATH)
	rm -rf $(SDK_BUILD_PATH)
	mkdir $(SDK_BUILD_PATH)
	cd $(SDK_BUILD_PATH) && cmake ../../../ && make && make DESTDIR=. install

test: $(TARGET)
	LD_LIBRARY_PATH=$(INSTALL_PATH)/lib ./$(TARGET)

.PHONY: all clean test sdk
