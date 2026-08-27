CC = gcc
CFLAGS = -Wall
LDFLAGS = -lSDL2
TARGET = chip8
SRCS = main.c emulator.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRCS) emualtor.h
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
