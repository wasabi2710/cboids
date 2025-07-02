# os suffix
ifeq ($(OS),Windows_NT)
    EXE := .exe
else
    EXE :=
endif

CC = gcc
CFLAGS = -Wall
TARGET = bin/program$(EXE)
TEST_TARGET = bin/test$(EXE)
SRCDIR = src
LDLIBS = -lraylib -lm

SRCS = $(wildcard $(SRCDIR)/*.c)
SRCS := $(filter-out src/test.c, $(SRCS))
TEST = $(SRCDIR)/test.c

# ensure bin exists
$(shell mkdir -p bin)

# build main program
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# build test
$(TEST_TARGET): $(TEST)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# run targets
run: $(TARGET)
	@echo "Running cboid..."
	./$(TARGET)

test: $(TEST_TARGET)
	@echo "Running test..."
	./$(TEST_TARGET)

# clean
clean:
	$(RM) $(TARGET) $(TEST_TARGET)
