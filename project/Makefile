CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS = -lm
TARGET = forest_detector
SRC = forest_detector.c
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: clean $(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(OBJ) sound_database.dat

# Clean including database
distclean: clean
	rm -f *.wav

# Install (optional - installs to /usr/local/bin)
install: $(TARGET)
	install -d $(DESTDIR)/usr/local/bin
	install -m 755 $(TARGET) $(DESTDIR)/usr/local/bin

# Uninstall
uninstall:
	rm -f $(DESTDIR)/usr/local/bin/$(TARGET)

# Run tests (requires test audio files)
test: $(TARGET)
	@echo "Running tests..."
	./$(TARGET) create
	@echo "Database created successfully!"
	./$(TARGET) list

# Help message
help:
	@echo "Forest Danger Sound Detection System - Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Build the project"
	@echo "  make debug    - Build with debug symbols"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make distclean- Remove all generated files"
	@echo "  make install  - Install to /usr/local/bin"
	@echo "  make uninstall- Remove from /usr/local/bin"
	@echo "  make test     - Run basic tests"
	@echo "  make help     - Show this help message"

.PHONY: all debug clean distclean install uninstall test help
