CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O3 -march=native -flto -Iinclude
LDFLAGS = -flto
TARGET = bin/main
SRCDIR = src
DATADIR = data
OBJDIR = obj
BINDIR = bin

# Enable parallel compilation
MAKEFLAGS += -j$(shell nproc)

SOURCES = $(wildcard *.c) $(wildcard $(SRCDIR)/*.c) $(wildcard $(DATADIR)/*.c)
OBJECTS = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SOURCES)))

.PHONY: all clean run debug release

# Default target is now optimized
all: release

# Release build with maximum optimization
release: CFLAGS += -DNDEBUG -fomit-frame-pointer -funroll-loops -ffast-math -ftree-vectorize -finline-functions -fno-signed-zeros -fno-trapping-math -falign-functions=16 -falign-loops=16 -falign-jumps=16
release: $(TARGET)

# Debug build (your original flags plus debug info)
debug: CFLAGS = -Wall -Wextra -std=c99 -pedantic -g -O0 -DDEBUG -Iinclude
debug: LDFLAGS = 
debug: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

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

# Profile-guided optimization targets
profile-generate: CFLAGS += -fprofile-generate
profile-generate: LDFLAGS += -fprofile-generate
profile-generate: clean $(TARGET)

profile-use: CFLAGS += -fprofile-use -fprofile-correction
profile-use: LDFLAGS += -fprofile-use
profile-use: $(TARGET)