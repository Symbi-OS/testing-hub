CC=gcc
CFLAGS=-Wall -Wextra -g
LDFLAGS=

SYMBIOS_DIR=../..
SYMLIB_DIR=$(SYMBIOS_DIR)/Symlib
SYMLIB_DYNAM_BUILD_DIR=$(SYMLIB_DIR)/dynam_build
SYMLIB_INCLUDE_DIR=$(SYMLIB_DIR)/include
SYMLIB_LINK=-L $(SYMLIB_DYNAM_BUILD_DIR) -lSym

CFLAGS += -I$(SYMLIB_INCLUDE_DIR)
LDFLAGS += $(SYMLIB_LINK)

# Source files and executables
SOURCES=$(wildcard *.c)
EXECUTABLES=$(SOURCES:.c=)

# Default target
all: $(EXECUTABLES)

# Rule to compile each source file to its corresponding executable
%: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

# Clean up
clean:
	rm -f $(EXECUTABLES)

run_tests: $(EXECUTABLES)
	@for exe in $^ ; do \
		./$$exe ; \
		if [ $$? -ne 0 ] ; then \
			exit 1 ; \
		fi ; \
		printf "\n" ; \
	done

.PHONY: all clean run_tests

