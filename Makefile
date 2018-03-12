doc:
	doxygen Doxyfile

clean:
	rm -rf doc

doc:
	doxygen Doxyfile

.PHONY: clean doc
