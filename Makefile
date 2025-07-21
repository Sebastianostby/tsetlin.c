CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g -Iinclude
TARGET = bin/main
SRCDIR = src
DATADIR = data
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard *.c) $(wildcard $(SRCDIR)/*.c) $(wildcard $(DATADIR)/*.c)
OBJECTS = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SOURCES)))

.PHONY: all clean run debug

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(DATADIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

run: $(TARGET)
	./$(TARGET)

debug: CFLAGS += -DDEBUG
debug: $(TARGET)
