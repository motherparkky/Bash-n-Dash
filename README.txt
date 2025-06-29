Bash n Dash — Terminal Running Game
====================================

Bash n Dash is a simple yet addictive running game that runs entirely in the terminal.
Built with C and ncurses, this game demonstrates how terminal-based graphics, real-time input handling, and simple game mechanics can be combined to create an engaging experience.

Features
--------
- Fast-paced running game in the terminal
- Switch among three lanes to avoid obstacles
- Dynamic obstacles:
  - █ Normal speed
  - ≫ Fast obstacle
  - ██ Large obstacle (takes two rows)
- Animated background with moving stars
- Score and High Score tracking
- Increasing difficulty over time
- High score saved locally in `highscore.dat`

How to Play
-----------
| Key           | Action                      |
|----------------|-----------------------------|
| Up Arrow ↑     | Move to upper lane          |
| Down Arrow ↓   | Move to lower lane          |
| Space          | Start game from the menu    |
| Q              | Quit game anytime           |

Game Objective
--------------
Dodge obstacles that come from the right side.
Survive as long as possible and beat your high score.

Installation
------------
Requirements:
- GCC (C Compiler)
- ncurses library

Install ncurses:

On Debian/Ubuntu-based systems:
    sudo apt install libncurses5-dev libncursesw5-dev

On macOS (with Homebrew):
    brew install ncurses

Build:
    make

Run:
    ./run.sh

File Structure
--------------
bashndash/
├── bashndash.c        # Main source code
├── Makefile           # Build script
├── run.sh             # Run script
└── highscore.dat      # High score file (auto-generated)

Example Screenshot
------------------
───────────────────────────────────────────────────────────────
    O                           .       .        █
───────────────────────────────────────────────────────────────
                                  <<        .         .
───────────────────────────────────────────────────────────────
                         .       .       ██
───────────────────────────────────────────────────────────────
Score: 47 | High Score: 96 | ↑↓ to move | q to quit


Future Improvements (Optional Ideas)
------------------------------------
- Add sound effects (if terminal allows)
- Add more lanes (3 or more)
- Implement power-ups or bonus items
- Multiplayer over network

Author
------
Created by motherparkky — Feel free to fork, contribute, or suggest improvements!
