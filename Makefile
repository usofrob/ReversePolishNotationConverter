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

# COVERAGEFLAGS:=-fprofile-arcs -ftest-coverage
COVERAGEFLAGS:=--coverage

all: $(SOURCES) $(LIBRARY)

test: $(TEST_EXECUTABLE)

mem-leak-check: test
	valgrind --leak-check=yes --error-exitcode=2 bin/test_rpn_convert

coverage: test
	scripts/testCoverageCheck.sh

$(LIBRARY): $(OBJECTS)
	$(AR) -rcs $@ $(OBJECTS)

$(TEST_EXECUTABLE): $(LIBRARY)
	$(CC) $(TESTS) -o $@ $(TESTFLAGS) $(COVERAGEFLAGS)
	bin/test_rpn_convert

.c.o:
	$(CC) $(CFLAGS) $(COVERAGEFLAGS) $< -o $@

clean:
	rm -f $(TEST_EXECUTABLE) $(OBJECTS) $(TEST_OBJECTS) $(LIBRARY) ./*.gcov ./*.gc* ./src/*.gc*
