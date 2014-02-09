BUILD         = Debug
BINDIR        = bin
OBJDIR        = obj
TARGET        = $(BINDIR)/$(BUILD)/string_test
SHELL         = /bin/sh
CC            = gcc
CFLAGS        = -std=gnu99 -pedantic -Wall -Wextra -Wno-unused-parameter -I.
DEBUGCFLAGS   = -ggdb3 -O0 -DDEBUG
RELEASECFLAGS = -O2 -DNDEBUG
LIBS          = 
SOURCES       = $(shell ls *.c 2>/dev/null)
HEADERS       = $(shell ls *.h 2>/dev/null)
# The following makes a list of .o files by taking the list of
# .c files and replacing the ".c" with ".o"
OBJECTS       = $(SOURCES:.c=.o)

.PHONY: all
.PHONY: dirs
all: dirs $(TARGET)

dirs:
	@mkdir -p $(OBJDIR)/$(BUILD)
	@mkdir -p $(BINDIR)/$(BUILD)

%.o: %.c $(HEADERS)
	$(CC) $(DEBUGCFLAGS) $(CFLAGS) -c -o $(OBJDIR)/$(BUILD)/$@ $<

$(TARGET): $(OBJECTS) 
	$(CC) $(DEBUGCFLAGS) $(CFLAGS) -o $(TARGET) $(OBJDIR)/$(BUILD)/* $(LIBS)

# Note "make release" does a complete rebuild (depends on "clean")
# Also note it doesn't bother with ".o" files as it is a complete rebuild.
.PHONY: release
release: clean $(SOURCES) $(HEADERS) dirs
	$(eval BUILD = Release)
	mkdir -p $(BINDIR)/$(BUILD)
	$(CC) $(RELEASECFLAGS) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

.PHONY: clean
clean:
	@rm -f tags
	@rm -rf $(OBJDIR)
	@rm -rf $(BINDIR)
	
	
# valgrind does a clean then regular (debug) build
# before running valgrind on the target.
.PHONY: valgrind
valgrind: clean dirs $(TARGET)
	valgrind --leak-check=full ./$(TARGET)
