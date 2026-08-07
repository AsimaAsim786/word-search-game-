# Word Search Game

A terminal-based word search puzzle game built in C++ featuring multiple levels, colored output, and 8-directional word placement.

## 🎮 Game Design

- Played on an 8x8 letter grid
- Each level contains 5 hidden words to find
- Words can be placed horizontally, vertically, or diagonally (forward and backward)
- Player selects the start and end position of a word to check it
- Colored terminal output highlights found words in green
- Progress through multiple levels by finding all words in each grid
- Win the game by completing every level

## 🧩 Code Structure

- **Board class** – Creates the letter grid, places words in 8 directions, checks player selections, and tracks which words have been found
- **Level class** – Stores the word list for a specific level and generates the board for it
- **Game class** – Manages the overall game flow: builds the word pool, loads levels, takes player input, tracks progress, and handles level completion / win conditions
- **main()** – Displays the menu (Info, Start/Continue Game, Exit) and controls the game loop

## ✨ Features

- 8x8 dynamic word search grid
- Multi-level progression system with a large word pool
- Words placed in all 8 directions (including diagonals, forward & backward)
- Colored terminal UI (green for found words, red for errors, yellow for headings)
- "Go back" option while entering coordinates
- Win screen when all levels are completed

## 🚀 What I Want to Add Next

- Score system based on time taken and levels completed
- Difficulty levels (easy, medium, hard) with different grid sizes
- Hint system for stuck players
- Save/load progress between sessions
- A GUI version using a graphics library

## 👩‍💻 Developed By

Asima Asim# word-search-game-
A terminal-based word search puzzle game built in C++ featuring multiple levels, colored output, and 8-directional word placement.
