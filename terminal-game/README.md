# Terminal Asteroid Dodger 🚀

A retro-style asteroid dodging game that runs directly in your terminal! Navigate through space, dodge asteroids, and see how long you can survive in this nostalgic ASCII adventure.

![Game Preview](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey)
![Language](https://img.shields.io/badge/Language-C%2B%2B-blue)
![License](https://img.shields.io/badge/License-MIT-green)

## 🎮 Features

- **Endless Gameplay**: Procedurally generated asteroid and star fields
- **Smooth Controls**: Responsive WASD or arrow key movement
- **Live Statistics**: Real-time score tracking and survival timer
- **Cinematic Intro**: Animated story sequence to set the mood
- **Restart System**: Game over screen with instant restart option
- **Lightweight**: Runs on minimal system resources

## 🚀 Quick Start

### Option 1: Download Pre-built Binary (Easiest)

1. Go to [Releases](https://github.com/Kai-972/Tech-Explore/releases)
2. Download the appropriate file for your system:
   - `asteroid-dodger-linux-x64` (Linux 64-bit)
   - `asteroid-dodger-macos` (macOS)
   - `asteroid-dodger-windows.exe` (Windows with WSL/terminal)
3. Make it executable (Linux/macOS): `chmod +x asteroid-dodger-*`
4. Run: `./asteroid-dodger-*` or double-click on Windows

### Option 2: One-Line Install Script

**Linux/macOS:**
```bash
curl -s https://raw.githubusercontent.com/Kai-972/Tech-Explore/main/terminal-game/install.sh | bash
```

**Or download and run:**
```bash
wget https://github.com/Kai-972/Tech-Explore/releases/latest/download/asteroid-dodger-linux-x64
chmod +x asteroid-dodger-linux-x64
./asteroid-dodger-linux-x64
```

### Option 3: Build from Source

**Prerequisites:**
```bash
# Ubuntu/Debian
sudo apt update && sudo apt install build-essential libncurses5-dev

# Arch Linux
sudo pacman -S base-devel ncurses

# macOS
brew install ncurses

# Fedora/RHEL
sudo dnf install gcc-c++ ncurses-devel
```

**Build and Run:**
```bash
git clone https://github.com/Kai-972/Tech-Explore.git
cd Tech-Explore/terminal-game
make
./bin/main
```

## 🎮 How to Play

### Controls
- **Movement**: `W` `A` `S` `D` or Arrow Keys
- **Quit**: `Q` at any time
- **Restart**: `R` after game over

### Objective
Pilot your ship (`o`) through the asteroid field, avoiding the dangerous asteroids (`*`) while collecting points for survival time. The longer you survive, the higher your score!

### Tips
- Watch the animated thruster behind your ship
- Stars (`.`) are just background - they won't hurt you
- Asteroids spawn more frequently as time goes on
- Your ship has "wings" (`<o>`) - the entire ship can collide with asteroids

## 🛠️ Technical Details

- **Language**: C++11
- **Graphics Library**: ncurses
- **Platform Support**: Linux, macOS, Windows (WSL/Cygwin)
- **Memory Usage**: < 5MB
- **Terminal Requirements**: 80x24 minimum, color support recommended

## 📁 Project Structure

```
terminal-game/
├── src/                 # Source code files
│   ├── main.cpp        # Entry point
│   ├── game.cpp        # Main game logic and rendering
│   ├── game.h          # Game structures and definitions
│   ├── ObjectField.cpp # Space object management
│   └── ObjectField.h   # Object field header
├── bin/                # Compiled binaries (created by make)
├── Makefile           # Build configuration
└── README.md          # This file
```

## 🔧 Development

### Building
```bash
make                # Build the game
make clean         # Clean build files
make install       # Install to /usr/local/bin (optional)
```

### Makefile Targets
- `make` or `make all`: Compile the game
- `make clean`: Remove all compiled files
- `make install`: Install system-wide (requires sudo)
- `make uninstall`: Remove system installation

## 🐛 Troubleshooting

**Game doesn't start:**
- Ensure your terminal is at least 80x24 characters
- Check that ncurses is installed
- Verify terminal supports color

**Controls not working:**
- Try using arrow keys instead of WASD
- Ensure terminal has focus
- Check if terminal supports keypad input

**Performance issues:**
- Close other terminal applications
- Try a different terminal emulator
- Ensure sufficient system resources

## 🤝 Contributing

This is a learning project, but contributions are welcome! Feel free to:
- Report bugs or suggest features
- Submit pull requests
- Share your high scores
- Fork and create your own variations


## 🙏 Acknowledgments

Built as a learning project exploring:
- C++ game development
- ncurses terminal graphics
- Cross-platform compilation
- Software distribution

---

**Enjoy the game and may the asteroids be ever in your favor!** 🌟

