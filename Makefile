CC=gcc
AR=ar
CFLAGS=-c -Wall -std=gnu99 -DUNIT_TESTING
SOURCES=src/word_processor.c
OBJECTS=$(SOURCES:.c=.o)
LIBRARY=lib/libword_processor.a

TESTFLAGS:=$(shell pkg-config --cflags --libs check) -Llib -lword_processor -Isrc
TESTS=tests/test.c
TEST_OBJECTS=$(TESTS:.c=.o)
TEST_EXECUTABLE=bin/test_word_processor

COVERAGEFLAGS:=--coverage

all: $(SOURCES) $(LIBRARY)

test: $(TEST_EXECUTABLE)

mem-leak-check: test
	valgrind --leak-check=yes --error-exitcode=2 bin/test_word_processor

coverage: test
	scripts/testCoverageCheck.sh

$(LIBRARY): $(OBJECTS)
	$(AR) -rcs $@ $(OBJECTS)

$(TEST_EXECUTABLE): $(LIBRARY)
	$(CC) $(TESTS) -o $@ $(TESTFLAGS) $(COVERAGEFLAGS)
	bin/test_word_processor

.c.o:
	$(CC) $(CFLAGS) $(COVERAGEFLAGS) $< -o $@

clean:
	rm -f $(TEST_EXECUTABLE) $(OBJECTS) $(TEST_OBJECTS) $(LIBRARY) ./*.gcov ./*.gc* ./src/*.gc* ./bin/*

basic:
	gcc -std=gnu99 src/word_processor.c -o bin/word_processor
	time bin/word_processor
