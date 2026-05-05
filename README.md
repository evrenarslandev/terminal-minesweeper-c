# Terminal-Based Minesweeper

A classic Minesweeper game developed in **C** specifically for the Linux/Ubuntu terminal environment. This project demonstrates advanced programming concepts and efficient logic implementation.

## 🚀 Key Technical Features
- **Recursive Flood-Fill:** Implemented a recursive algorithm to automatically reveal all adjacent empty cells when a cell with zero neighboring mines is hit.
- **Dynamic Memory Management:** The game board is managed using 2D dynamic arrays (`malloc` and `free`), ensuring efficient memory usage based on randomized board sizes.
- **Undo System:** Features a stack-based approach to store move history, allowing players to revert their actions seamlessly.
- **File I/O System:** Automatically logs game maps to `map.txt` and tracks all player moves in `moves.txt` for post-game analysis.
- **ANSI Color Feedback:** Utilizes terminal escape codes to provide a color-coded user interface for enhanced gameplay experience.

## 🛠️ How to Compile and Run
Ensure you have the `gcc` compiler installed on your system.

1. **Clone or download** the repository.
2. **Open your terminal** and navigate to the project folder.
3. **Compile** the code:
   gcc minesweeper.c -o minesweeper
4. Run the game:
   ./minesweeper

## 🎮 Controls
- **Reveal a Cell:** Enter the row and column numbers separated by a space (e.g., `2 3`).
- **Undo Last Move:** Type `undo` to revert your last reveal action.
- **Game End:** The game ends if you hit a mine (`BOOM!`) or successfully reveal all non-mine cells.
