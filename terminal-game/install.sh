#!/bin/bash

# Terminal Asteroid Dodger - One-line installer
# Usage: curl -s https://raw.githubusercontent.com/Kai-972/Tech-Explore/main/terminal-game/install.sh | bash

set -e

REPO_URL="https://github.com/Kai-972/Tech-Explore"
GAME_NAME="asteroid-dodger"
INSTALL_DIR="$HOME/.local/bin"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Detect OS and architecture
detect_platform() {
    local os=$(uname -s | tr '[:upper:]' '[:lower:]')
    local arch=$(uname -m)
    
    case "$os" in
        linux*)
            case "$arch" in
                x86_64) echo "linux-x64" ;;
                i386|i686) echo "linux-x86" ;;
                armv7l) echo "linux-arm" ;;
                aarch64) echo "linux-arm64" ;;
                *) echo "unsupported" ;;
            esac
            ;;
        darwin*)
            case "$arch" in
                x86_64) echo "macos-x64" ;;
                arm64) echo "macos-arm64" ;;
                *) echo "unsupported" ;;
            esac
            ;;
        msys*|cygwin*|mingw*)
            echo "windows"
            ;;
        *)
            echo "unsupported"
            ;;
    esac
}

# Check if required tools are available
check_requirements() {
    for cmd in curl chmod; do
        if ! command -v "$cmd" >/dev/null 2>&1; then
            print_error "Required command '$cmd' not found!"
            exit 1
        fi
    done
}

# Download and install the game
install_game() {
    local platform=$(detect_platform)
    
    if [ "$platform" = "unsupported" ]; then
        print_error "Your platform is not supported by pre-built binaries."
        print_status "Please build from source: $REPO_URL"
        exit 1
    fi
    
    print_status "Detected platform: $platform"
    print_status "Installing Terminal Asteroid Dodger..."
    
    # Create install directory
    mkdir -p "$INSTALL_DIR"
    
    # Determine binary name
    local binary_name="$GAME_NAME-$platform"
    if [ "$platform" = "windows" ]; then
        binary_name="$binary_name.exe"
    fi
    
    # Download URL (GitHub releases)
    local download_url="$REPO_URL/releases/latest/download/$binary_name"
    local local_binary="$INSTALL_DIR/$GAME_NAME"
    
    print_status "Downloading from: $download_url"
    
    # Download the binary
    if curl -L -o "$local_binary" "$download_url" 2>/dev/null; then
        print_success "Downloaded successfully!"
    else
        print_error "Failed to download binary. Trying to build from source..."
        build_from_source
        return
    fi
    
    # Make executable
    chmod +x "$local_binary"
    
    print_success "Game installed to: $local_binary"
    
    # Check if install dir is in PATH
    if [[ ":$PATH:" != *":$INSTALL_DIR:"* ]]; then
        print_warning "Install directory is not in PATH."
        print_status "Add this line to your ~/.bashrc or ~/.zshrc:"
        echo "export PATH=\"\$PATH:$INSTALL_DIR\""
        print_status "Or run directly: $local_binary"
    else
        print_status "You can now run: $GAME_NAME"
    fi
    
    # Test the installation
    print_status "Testing installation..."
    if "$local_binary" --help >/dev/null 2>&1 || [ $? -eq 0 ]; then
        print_success "Installation test passed!"
        print_status "Run '$GAME_NAME' or '$local_binary' to play!"
    else
        print_warning "Installation completed but test failed. You may need to install ncurses:"
        echo "  Ubuntu/Debian: sudo apt install libncurses5"
        echo "  Arch: sudo pacman -S ncurses"
        echo "  macOS: brew install ncurses"
    fi
}

# Fallback: build from source
build_from_source() {
    print_status "Building from source..."
    
    # Check for build requirements
    local missing_deps=()
    
    if ! command -v g++ >/dev/null 2>&1 && ! command -v clang++ >/dev/null 2>&1; then
        missing_deps+=("C++ compiler (g++ or clang++)")
    fi
    
    if ! command -v make >/dev/null 2>&1; then
        missing_deps+=("make")
    fi
    
    # Check for ncurses (this is tricky to check universally)
    if ! ldconfig -p | grep -q ncurses 2>/dev/null && ! ls /usr/lib*/libncurses* >/dev/null 2>&1; then
        missing_deps+=("ncurses development library")
    fi
    
    if [ ${#missing_deps[@]} -gt 0 ]; then
        print_error "Missing dependencies:"
        for dep in "${missing_deps[@]}"; do
            echo "  - $dep"
        done
        print_status "Install them with:"
        echo "  Ubuntu/Debian: sudo apt install build-essential libncurses5-dev"
        echo "  Arch: sudo pacman -S base-devel ncurses"
        echo "  macOS: xcode-select --install && brew install ncurses"
        exit 1
    fi
    
    # Create temporary directory
    local temp_dir=$(mktemp -d)
    cd "$temp_dir"
    
    print_status "Cloning repository..."
    if ! git clone "$REPO_URL.git" .; then
        print_error "Failed to clone repository"
        exit 1
    fi
    
    cd terminal-game
    
    print_status "Building game..."
    if make; then
        print_success "Build successful!"
        cp bin/main "$INSTALL_DIR/$GAME_NAME"
        chmod +x "$INSTALL_DIR/$GAME_NAME"
        print_success "Game installed to: $INSTALL_DIR/$GAME_NAME"
    else
        print_error "Build failed!"
        exit 1
    fi
    
    # Cleanup
    cd /
    rm -rf "$temp_dir"
}

# Main installation process
main() {
    echo "🚀 Terminal Asteroid Dodger Installer 🚀"
    echo "=========================================="
    
    check_requirements
    install_game
    
    echo
    print_success "Installation complete! 🎮"
    echo "Run the game with: $GAME_NAME"
    echo "Or: $INSTALL_DIR/$GAME_NAME"
    echo
    echo "Enjoy dodging those asteroids! 🌟"
}

# Run main function
main "$@"
