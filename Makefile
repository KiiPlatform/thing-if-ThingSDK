all: clean linux

linux:
	$(MAKE) -C linux-sample

clean:
	rm -rf $(TARGET) html latex

doc:
	doxygen Doxyfile

.PHONY: clean
