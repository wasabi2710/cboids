CC = gcc
CLAGS = -Wall
TARGET = bin/program
SRCDIR = src
LDLIBS = -lraylib -lm

#all sources
SRCS = $(wildcard $(SRCDIR)/*.c)

#build bin
$(shell mkdir -p bin)

#exec
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

run: $(TARGET)
	@echo "Running cboid..."
	./$(TARGET)

#clean artifacts
clean:
	rm -r $(TARGET)
