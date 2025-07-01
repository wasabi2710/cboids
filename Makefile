CC = gcc
CLAGS = -Wall
TARGET = bin/program
TEST_TARGET = bin/test
SRCDIR = src
LDLIBS = -lraylib -lm

#all sources
SRCS = $(wildcard $(SRCDIR)/*.c)
SRCS := $(filter-out src/test.c, $(SRCS))
TEST = $(SRCDIR)/test.c

#build bin
$(shell mkdir -p bin)

#exec prog
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

run: $(TARGET)
	@echo "Running cboid..."
	./$(TARGET)

#run test
$(TEST_TARGET): $(TEST)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

test: $(TEST_TARGET)
	@echo "Running test..."
	./$(TEST_TARGET)

#clean artifacts
clean:
	rm -r $(TARGET)
