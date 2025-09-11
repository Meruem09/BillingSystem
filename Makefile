CC=gcc
CFLAGS=-Wall -Wextra -std=c99 $(NIX_CFLAGS_COMPILE)
LDFLAGS=$(NIX_LDFLAGS)
LIBS=-lSDL2
TARGET=xyz_billing
SOURCES=main.c item.c customer.c report.c gui.c

all: $(TARGET)

$(TARGET): $(SOURCES)
        $(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS) $(LIBS)

clean:
        rm -f $(TARGET)

run: $(TARGET)
        ./$(TARGET)