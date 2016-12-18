CC=gcc
CFLAGS=-Wall -lcheck_pic -pthread -lrt -lm -lsubunit
LDFLAGS=-pthread -lcheck_pic -lrt -lm -lsubunit

SOURCES=
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bin/rpn_convert

TESTS=tests/test.c
TEST_OBJECTS=$(TESTS:.c=.o)
TEST_EXECUTABLE=bin/test_rpn_convert

# all: $(SOURCES) $(EXECUTABLE)

test: $(TEST_EXECUTABLE)
    
# $(EXECUTABLE): $(OBJECTS) 
#	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(TEST_EXECUTABLE):
	$(CC) $(TESTS) -o $@ $(LDFLAGS)
	bin/test_rpn_convert

#.c.o:
#	$(CC) $(CFLAGS) $< -o $@

clean:
	rm bin/*
