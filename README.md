# Conway's Game Of Life

### Overview

This is a C-based implementation of **Conway's Game of Life**, a simulation where cells on a board evolve based on a set of rules. You can control the game through various keyboard and mouse inputs, as well as load and save grid states via text files.

To run this project on your divice, you need to download CSFML library.

##### Features

- Visual simulation of Conway's Game of Life.
- Control the game with keyboard shortcuts (start, pause, reset, etc.).
- Load and save game states via `.txt` files.
- Simple graphical interface with customizable grid size.

##### Controls
| Key             | Action                                                 |
| --------------- | ------------------------------------------------------ |
| **TAB**         | Show/hide instructions.                                |
| **Space**       | Start/pause the game.                                  |
| **LShift**      | Load a grid from a `.txt` file.                        |
| **Enter**       | Save the current grid to a `.txt` file.                |
| **Backspace**   | Clear the entire grid.                                 |
| **Escape**      | Exit the game.                                         |
| **Right Arrow** | Step to the next generation while paused.              |
| **Mouse**       | Toggle cell state (alive/dead) on click (when paused). |

### Instructions

##### Starting the Game

1. After launching the program, you will be prompted to input the number of squares per row for the game grid. The maximum value is 100.
2. The game window will open with an empty grid and controls displayed.
3. You can control the game using the keyboard and mouse as described above.

##### Grid Behavior

- **Alive Cells**: Represented in white.
- **Dead Cells**: Represented in red.
- **Default/Inactive Cells**: Represented in black.
  
The game follows standard Conway's Game of Life rules:
1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

##### Loading a Grid

- Press **LShift** to open a file dialog and select a `.txt` file containing a saved grid state. The program will attempt to read and load the grid. If unsuccessful, an error will be displayed in the console. There are files in `Documentation\States_Of_The_Board` folder to check this function (in the beginning of their names you can see the size of the board). You can use the same folder to save your own files.

##### Saving a Grid

- Press **Enter** to save the current state of the grid to a `.txt` file. This file can be loaded later to restore the game to this state.

##### Clearing the Grid

- Press **Backspace** to reset the entire grid to the default state (all cells inactive).

##### Exiting the Game

- Press **Escape** or click the close button to exit the game.
### Error Handling

- If the window, font, or memory allocation fails at any point, an error message will be displayed and the program will terminate.
- File handling is done cautiously, ensuring that invalid file formats or unreadable files are handled gracefully.


