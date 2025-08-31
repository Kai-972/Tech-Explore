# Terminal Asteroid Dodger - Enhanced Makefile
# Supports building, installing, and creating distribution packages

# Compiler and flags
CXX = g++
CFLAGS = -std=c++11 -Wall -pedantic -O2
DEBUG_FLAGS = -g -DDEBUG
STATIC_FLAGS = -static-libgcc -static-libstdc++

# Directories
SRC_DIR = src
BIN_DIR = bin
DIST_DIR = dist
INSTALL_PREFIX = /usr/local
INSTALL_BIN = $(INSTALL_PREFIX)/bin

# Target binary name
TARGET = $(BIN_DIR)/main
GAME_NAME = asteroid-dodger

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

# Platform detection
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

ifeq ($(UNAME_S),Linux)
    PLATFORM = linux
    LDFLAGS = -lncurses
endif
ifeq ($(UNAME_S),Darwin)
    PLATFORM = macos
    LDFLAGS = -lncurses
endif
ifeq ($(OS),Windows_NT)
    PLATFORM = windows
    LDFLAGS = -lncurses
    TARGET = $(BIN_DIR)/main.exe
endif

# Architecture detection
ifeq ($(UNAME_M),x86_64)
    ARCH = x64
endif
ifeq ($(UNAME_M),i386)
    ARCH = x86
endif
ifeq ($(UNAME_M),i686)
    ARCH = x86
endif
ifeq ($(UNAME_M),armv7l)
    ARCH = arm
endif
ifeq ($(UNAME_M),aarch64)
    ARCH = arm64
endif
ifeq ($(UNAME_M),arm64)
    ARCH = arm64
endif

PLATFORM_ARCH = $(PLATFORM)-$(ARCH)

# Default target
.PHONY: all
all: $(TARGET)

# Build main target
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	@echo "✅ Build complete: $@"

# Compile object files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) -c -o $@ $< $(CFLAGS)

# Debug build
.PHONY: debug
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)
	@echo "🐛 Debug build complete"

# Static build (for distribution)
.PHONY: static
static: CFLAGS += $(STATIC_FLAGS)
static: $(TARGET)
	@echo "📦 Static build complete"

# Clean build files
.PHONY: clean
clean:
	rm -rf $(BIN_DIR)
	rm -rf $(DIST_DIR)
	@echo "🧹 Cleaned build files"

# Install system-wide
.PHONY: install
install: $(TARGET)
	@echo "📥 Installing $(GAME_NAME) to $(INSTALL_BIN)..."
	@mkdir -p $(INSTALL_BIN)
	cp $(TARGET) $(INSTALL_BIN)/$(GAME_NAME)
	chmod +x $(INSTALL_BIN)/$(GAME_NAME)
	@echo "✅ Installed! Run with: $(GAME_NAME)"

# Uninstall
.PHONY: uninstall
uninstall:
	rm -f $(INSTALL_BIN)/$(GAME_NAME)
	@echo "🗑️  Uninstalled $(GAME_NAME)"

# Create distribution package
.PHONY: dist
dist: static
	@echo "📦 Creating distribution package..."
	@mkdir -p $(DIST_DIR)
	cp $(TARGET) $(DIST_DIR)/$(GAME_NAME)-$(PLATFORM_ARCH)
	cp README.md $(DIST_DIR)/
	cp -r src $(DIST_DIR)/
	cp Makefile $(DIST_DIR)/
	@if [ -f LICENSE ]; then cp LICENSE $(DIST_DIR)/; fi
	@echo "✅ Distribution package created in $(DIST_DIR)/"

# Create release archive
.PHONY: archive
archive: dist
	@echo "📦 Creating release archive..."
	cd $(DIST_DIR) && tar -czf $(GAME_NAME)-$(PLATFORM_ARCH).tar.gz *
	@echo "✅ Archive created: $(DIST_DIR)/$(GAME_NAME)-$(PLATFORM_ARCH).tar.gz"

# Cross-compilation targets (requires appropriate toolchains)
.PHONY: linux-x64
linux-x64:
	$(MAKE) static PLATFORM=linux ARCH=x64 CXX=x86_64-linux-gnu-g++

.PHONY: windows
windows:
	$(MAKE) static PLATFORM=windows CXX=x86_64-w64-mingw32-g++ LDFLAGS=-lncurses

# Run the game
.PHONY: run
run: $(TARGET)
	./$(TARGET)

# Run with debug
.PHONY: debug-run
debug-run: debug
	./$(TARGET)

# Test build (just compile)
.PHONY: test
test: $(TARGET)
	@echo "🧪 Build test passed"

# Show build information
.PHONY: info
info:
	@echo "🔧 Build Information"
	@echo "===================="
	@echo "Compiler: $(CXX)"
	@echo "Flags: $(CFLAGS)"
	@echo "Platform: $(PLATFORM_ARCH)"
	@echo "Target: $(TARGET)"
	@echo "Sources: $(SOURCES)"
	@echo "Objects: $(OBJECTS)"

# Help target
.PHONY: help
help:
	@echo "🚀 Terminal Asteroid Dodger - Build System"
	@echo "=========================================="
	@echo ""
	@echo "Targets:"
	@echo "  all        - Build the game (default)"
	@echo "  debug      - Build with debug symbols"
	@echo "  static     - Build static binary for distribution"
	@echo "  clean      - Remove all build files"
	@echo "  install    - Install system-wide (requires sudo)"
	@echo "  uninstall  - Remove system installation"
	@echo "  dist       - Create distribution package"
	@echo "  archive    - Create release archive"
	@echo "  run        - Build and run the game"
	@echo "  debug-run  - Build debug version and run"
	@echo "  test       - Test build without running"
	@echo "  info       - Show build information"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Cross-compilation:"
	@echo "  linux-x64  - Build for Linux x64"
	@echo "  windows    - Build for Windows (requires MinGW)"
	@echo ""
	@echo "Examples:"
	@echo "  make           # Build the game"
	@echo "  make run       # Build and run"
	@echo "  make dist      # Create distribution package"
	@echo "  make install   # Install system-wide"

# Dependencies
$(BIN_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/game.h
$(BIN_DIR)/game.o: $(SRC_DIR)/game.cpp $(SRC_DIR)/game.h $(SRC_DIR)/ObjectField.h
$(BIN_DIR)/ObjectField.o: $(SRC_DIR)/ObjectField.cpp $(SRC_DIR)/ObjectField.h $(SRC_DIR)/game.h
