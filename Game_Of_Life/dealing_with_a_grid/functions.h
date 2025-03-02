#ifndef functions
#define functions

#include <SFML/Graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <math.h>

#include "tinyfiledialogs.h"

char* to_string(unsigned short number);
// Function to convert unsigned short to char* (only for positive integers)
unsigned int to_int(char* number);
// Function to convert char* to unsigned int (only for positive integers)

unsigned short count_neighbours(unsigned short** old_grid, int row, int column, const unsigned short AMOUNT);
// Function to count neighbours of each square
void kill_or_revive_square(unsigned short** grid, int row, int column, const unsigned short counter);
// Function which will change the state of square depending on the amount of its neighbours

void display_main_window(sfRenderWindow* main_window, sfRectangleShape* square, \
                         unsigned short** grid, \
                         const unsigned short AMOUNT, const unsigned short SIDE_OF_SQUARE, \
                         sfText* population, sfText* instructions, \
                         unsigned short status_of_the_instructions);
// Function to display main window
void next_frame(sfRenderWindow* main_window, sfRectangleShape* square, \
              unsigned short** grid, unsigned short** old_grid, \
              const unsigned short AMOUNT, const unsigned short SIDE_OF_SQUARE, \
              sfText* population, sfText* instructions, \
              unsigned short status_of_the_instructions);
// Function to calculate next state of the board and to show it afterwards

void copy_grid(unsigned short** grid, unsigned short** old_grid, const unsigned short AMOUNT);
// Function to copy state of the board from first parameter to the second one
void clear_grid(unsigned short** grid, const unsigned short AMOUNT);
// Function to clear the board (every square will become default)

unsigned short load_grid(unsigned short** grid, const unsigned short AMOUNT);
// Function to download some state of the board from .txt file (uses tinyfiledilogs for it)
void upload_grid(unsigned short** grid, const unsigned short AMOUNT);
// Function to upload current state of the board to .txt file (uses tinyfiledilogs for it)

#endif // functions
