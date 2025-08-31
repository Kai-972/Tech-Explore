# Terminal Asteroid Dodger 🚀

A simple retro-style asteroid dodging game written in C++ that runs directly in your terminal using the ncurses library.



---

## Features
* Endless, procedurally generated star and asteroid fields.
* Smooth, responsive player controls (WASD or Arrow Keys).
* Live score and timer to track your survival skills.
* Animated text intro sequence.
* Game over and restart functionality.

---

## Getting Started

Follow these instructions to compile and run the game on your own machine.

### Prerequisites

You will need a C++ compiler and the ncurses development library. Here is how to install them on common Linux distributions:

* **Debian / Ubuntu:**
    ```bash
    sudo apt-get update && sudo apt-get install build-essential libncurses5-dev
    ```

* **Fedora / CentOS / RHEL:**
    ```bash
    sudo dnf install gcc-c++ ncurses-devel
    ```

* **Arch Linux:**
    ```bash
    sudo pacman -S base-devel ncurses
    ```

### Building and Running the Game

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/Kai-972/Tech-Explore.git](https://github.com/Kai-972/Tech-Explore.git)
    ```

2.  **Navigate to the game directory:**
    ```bash
    cd Tech-Explore/terminal-game
    ```

3.  **Compile the game using the Makefile:**
    ```bash
    make
    ```

4.  **Run the executable:**
    ```bash
    ./bin/main
    ```

---

## How to Play

* **Move Ship:** `W`, `A`, `S`, `D` or `Up`, `Left`, `Down`, `Right` arrow keys.
* **Quit Game:** `Q`

The goal is to survive as long as possible by dodging the oncoming asteroids (`*`). Your score increases the longer you survive.

---

## Makefile Commands

* `make`: Compiles the game and creates the executable in the `bin/` directory.
* `make clean`: Removes all compiled object files and the final executable.
