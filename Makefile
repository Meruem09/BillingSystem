CC=gcc
CFLAGS=-Wall -Wextra -std=c99
TARGET=xyz_billing
SOURCES=main.c item.c customer.c report.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run