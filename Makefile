override CFLAGS += -std=gnu89 -Wall -pedantic -pthread -fPIC -shared

all: clean build

build:
	$(MAKE) -C linux-sample

clean:
	rm -rf html latex

doc:
	doxygen Doxyfile

.PHONY: clean
