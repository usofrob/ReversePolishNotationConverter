CC=gcc
AR=ar
CFLAGS=-c -Wall -std=gnu99
SOURCES=src/rpn_convert.c
OBJECTS=$(SOURCES:.c=.o)
LIBRARY=lib/librpn_convert.a

TESTFLAGS:=$(shell pkg-config --cflags --libs check) -Llib -lrpn_convert -Isrc
TESTS=tests/test.c
TEST_OBJECTS=$(TESTS:.c=.o)
TEST_EXECUTABLE=bin/test_rpn_convert

all: $(SOURCES) $(LIBRARY)

test: $(TEST_EXECUTABLE)
    
$(LIBRARY): $(OBJECTS)
	$(AR) -rcs $@ $(OBJECTS)

$(TEST_EXECUTABLE): $(LIBRARY)
	$(CC) $(TESTS) -o $@ $(TESTFLAGS)
	bin/test_rpn_convert

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f bin/* src/*.o lib/*
