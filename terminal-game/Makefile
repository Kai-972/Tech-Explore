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

# Check for static libraries
HAS_STATIC_NCURSES := $(shell pkg-config --exists ncurses --static && echo yes || echo no)
HAS_LIBNCURSES_A := $(shell find /usr/lib* /lib* -name "libncurses.a" 2>/dev/null | head -1)

ifeq ($(UNAME_S),Linux)
    PLATFORM = linux
    LDFLAGS = -lncurses
    
    # Try to determine the best static linking approach
    ifneq ($(HAS_LIBNCURSES_A),)
        # Found static ncurses library
        STATIC_LDFLAGS = -static -lncurses -ltinfo -ldl -lpthread
    else
        # Fallback: try without static, or use alternative approach
        STATIC_LDFLAGS = -lncurses
        STATIC_FLAGS = -static-libgcc -static-libstdc++
        $(warning Warning: Static ncurses library not found. Building with dynamic ncurses.)
    endif
endif

ifeq ($(UNAME_S),Darwin)
    PLATFORM = macos
    LDFLAGS = -lncurses
    # macOS doesn't support full static linking
    STATIC_LDFLAGS = -lncurses
endif

ifeq ($(OS),Windows_NT)
    PLATFORM = windows
    LDFLAGS = -lncurses
    STATIC_LDFLAGS = -static -lncurses
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

# Static build (for distribution) - with better error handling
.PHONY: static
static: 
	@echo "🔍 Checking for static libraries..."
	@echo "Static ncurses search result: $(HAS_LIBNCURSES_A)"
ifneq ($(HAS_LIBNCURSES_A),)
	@echo "✅ Found static ncurses library"
	@$(MAKE) static-build-full
else
	@echo "⚠️  Static ncurses not found, trying partial static build..."
	@$(MAKE) static-build-partial
endif

# Full static build (when static libs are available)
.PHONY: static-build-full
static-build-full: $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $(TARGET) $^ $(CFLAGS) -static -lncurses -ltinfo -ldl -lpthread
	@echo "📦 Full static build complete: $(TARGET)"
	@echo "Verifying binary:"
	@file $(TARGET)
	@ls -la $(TARGET)

# Partial static build (static C++ libs, dynamic ncurses)
.PHONY: static-build-partial
static-build-partial: $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $(TARGET) $^ $(CFLAGS) $(STATIC_FLAGS) $(LDFLAGS)
	@echo "📦 Partial static build complete: $(TARGET)"
	@echo "Verifying binary:"
	@file $(TARGET)
	@ls -la $(TARGET)
	@echo "⚠️  Note: This binary links ncurses dynamically"

# Alternative static build using musl (if available)
.PHONY: static-musl
static-musl: $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	@if command -v musl-gcc >/dev/null 2>&1; then \
		echo "🔧 Building with musl for better static linking..."; \
		musl-gcc -o $(TARGET) $^ $(CFLAGS) -static -lncurses; \
	else \
		echo "❌ musl-gcc not available, falling back to regular build"; \
		$(MAKE) static-build-partial; \
	fi

# Check system libraries and provide helpful information
.PHONY: check-libs
check-libs:
	@echo "🔍 System Library Check"
	@echo "======================"
	@echo "Operating System: $(UNAME_S)"
	@echo "Architecture: $(UNAME_M)"
	@echo ""
	@echo "Checking for ncurses libraries:"
	@find /usr/lib* /lib* -name "*ncurses*" 2>/dev/null | head -10 || echo "No ncurses libraries found in standard locations"
	@echo ""
	@echo "Checking for static libraries:"
	@find /usr/lib* /lib* -name "libncurses.a" 2>/dev/null || echo "No static ncurses library found"
	@echo ""
	@echo "Package manager suggestions:"
	@if command -v pacman >/dev/null 2>&1; then \
		echo "Arch Linux: sudo pacman -S ncurses"; \
		echo "For static: Install ncurses-static from AUR"; \
	elif command -v apt >/dev/null 2>&1; then \
		echo "Ubuntu/Debian: sudo apt install libncurses5-dev"; \
	elif command -v brew >/dev/null 2>&1; then \
		echo "macOS: brew install ncurses"; \
	fi

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
	@echo "🗑️ Uninstalled $(GAME_NAME)"

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
	@echo "LD Flags: $(LDFLAGS)"
	@echo "Static LD Flags: $(STATIC_LDFLAGS)"
	@echo "Has static ncurses: $(HAS_LIBNCURSES_A)"

# Help target
.PHONY: help
help:
	@echo "🚀 Terminal Asteroid Dodger - Build System"
	@echo "=========================================="
	@echo ""
	@echo "Targets:"
	@echo "  all          - Build the game (default)"
	@echo "  debug        - Build with debug symbols"
	@echo "  static       - Build static binary (auto-detects capabilities)"
	@echo "  static-musl  - Build with musl for better static linking"
	@echo "  check-libs   - Check system libraries and dependencies"
	@echo "  clean        - Remove all build files"
	@echo "  install      - Install system-wide (requires sudo)"
	@echo "  uninstall    - Remove system installation"
	@echo "  dist         - Create distribution package"
	@echo "  archive      - Create release archive"
	@echo "  run          - Build and run the game"
	@echo "  debug-run    - Build debug version and run"
	@echo "  test         - Test build without running"
	@echo "  info         - Show build information"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make check-libs  # Check what libraries are available"
	@echo "  make            # Build the game"
	@echo "  make static     # Try to build static binary"
	@echo "  make run        # Build and run"

# Dependencies
$(BIN_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/game.h
$(BIN_DIR)/game.o: $(SRC_DIR)/game.cpp $(SRC_DIR)/game.h $(SRC_DIR)/ObjectField.h
$(BIN_DIR)/ObjectField.o: $(SRC_DIR)/ObjectField.cpp $(SRC_DIR)/ObjectField.h $(SRC_DIR)/game.h
