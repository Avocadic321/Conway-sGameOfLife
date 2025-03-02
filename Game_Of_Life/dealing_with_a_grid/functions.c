#include "functions.h"

/*
Function to convert unsigned short to char* (only for positive integers).
This function allocates memory for char*, so it should be cleaned manually afterwards.
*/
char* to_string(unsigned short number) // Maximum value of the number: 65535 (because of its type)
{
    if (number == 0) // If number == 0, just create a string "0\0"
    {
        char* result = (char*)malloc(2 * sizeof(char)); // Allocate memory for "0\0"
        strcpy(result, "0\0"); // result = "0\0", where '\0' is the end of string
        return result;
    }

    char* string = (char*)malloc(6 * sizeof(char)); // Allocating of memory. String template: *****\0
    unsigned short i = 0; // Counter to write every digit one by one in the string

    while (number > 0) // Loop to convert number to string. It will be reversed.
    {
        string[i] = (number % 10) + 48; // char + number = char (code)
        number /= 10;
        ++i;
    }

    string[i] = '\0'; // Adding last character of string
    char* result = (char*)malloc((i + 1) * sizeof(char));
    // Allocating of memory for the result string: i + 1 characters, because of '\0'

    for (unsigned short j = 0; j < i; ++j) // Repetition to reverse the string
    {
        result[j] = string[i - j - 1]; // Write every digit from string to result with reversion
    }
    result[i] = '\0'; // Adding last character of string

    free(string); // Free memory from the additional string

    return result;
}

/*
Function to convert char* to unsigned int (only for positive integers)
This function does not free memory from char*, so it should be cleaned manually afterwards.
*/
unsigned int to_int(char* number)
{
    unsigned short length = 0; // Variable for the length of the number
    while (number[length] != '\0') // Loop to count the length
    {
        ++length; // Every character => length += 1
    }

    unsigned int result = 0; // Declaration of the result variable
    for (unsigned short i = 0; i < length; ++i) // Loop to convert char* to unsigned int
    {
        result += (number[i] - '0') * pow(10, length - i - 1);
        // Every turn of the loop i increases, so power of 10 decreases: 341 = 3 * 10 ** 2 + 4 * 10 ** 1 + 1 * 10 ** 0
    }
    return result;
}

/*
Function to count neighbours of each square.
Return of this function will be a parameter for the kill_or_revive_square function.
*/
unsigned short count_neighbours(unsigned short** old_grid, int row, int column, const unsigned short AMOUNT)
{
    unsigned short counter = 0; // Variable to store the amount of neighbours
    short start_row = -1, end_row = 2, start_column = -1, end_column = 2;
    // Variables which will define the range of 'for' loop depending on the position of the square [row][column]

    if (row == 0) { start_row = 0; } // If square on the upper boarder: cannot check row above
    else if (row == AMOUNT) { end_row = 1; } // If square on the lower boarder: cannot check row below

    if (column == 0) { start_column = 0; } // If square on the left boarder: cannot check left column
    else if (column == AMOUNT) { end_column = 1; } // If square on the right boarder: cannot check right column


    for (short i = start_row; i < end_row; ++i) // Loop for index of row
    {
        for (short j = start_column; j < end_column; ++j) // Loop for index of column
        {
            if ((i == 0) && (j == 0)) // If it is the square neighbours of which function checks
            {
                continue;
            }
            counter += (old_grid[row + i][column + j] % 2); // possible values: 0, 1, 2. Alive is only 1 % 2 = 1
        }
    }

    return counter; // Return the amount of neighbours
}

/*
Rules of the game for function kill_or_revive_square:
1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/

void kill_or_revive_square(unsigned short** grid, int row, int column, const unsigned short counter)
{
    if (grid[row][column] != 1) // If the square is not alive
    {
        if (counter == 3) // Can be alive only if the amount of neighbours = 3
        {
            grid[row][column] = 1; // The square become alive
        }
    }
    else // If square is alive
    {
        if (counter < 2) // The amount of neighbours < 2
        {
            grid[row][column] = 2; // The square dies
        }
        else if (counter > 3) // The amount of neighbours > 3
        {
            grid[row][column] = 2; // The square dies
        }
    }
}

void display_main_window(sfRenderWindow* main_window, sfRectangleShape* square, \
                         unsigned short** grid, \
                         const unsigned short AMOUNT, const unsigned short SIDE_OF_SQUARE, \
                         sfText* population, sfText* instructions, \
                         unsigned short status_of_the_instructions)
{
    sfRenderWindow_clear(main_window, sfBlack); //Clear the previous frame, fill the window with black colour

    unsigned short counter_population = 0;
    /*
    This variable will be increased with every drawn alive square.
    Created to show the amount of alive squares to user.
    */

    for (unsigned short i = 0; i < AMOUNT; ++i) // Loop for index of row
    {
        for (unsigned short j = 0; j < AMOUNT; ++j) // Loop for index of column
        {
            sfRectangleShape_setPosition(square, (sfVector2f){i * SIDE_OF_SQUARE, j * SIDE_OF_SQUARE});
            /*
            Move the only square that have been created in the beginning of main.c to different positions
            in order to draw every square of the board. The square will be moved to coordinates of sfVector2f
            (the left corner of the square will be in those coordinates).
            */

            if (grid[j][i] == 1) // If square is alive
            {
                counter_population += 1; // Increase the counter of alive squares
                sfRectangleShape_setOutlineColor(square, sfBlack); // Set outline colour to black
                sfRectangleShape_setFillColor(square, sfWhite); // Set fill colour to white
            }
            else if (grid[j][i] == 2) // If square is dead
            {
                sfRectangleShape_setOutlineColor(square, sfColor_fromRGB(150, 150, 150)); // Set outline colour to grey
                sfRectangleShape_setFillColor(square, sfColor_fromRGB(200, 50, 50)); // Set fill colour to red
            }
            else // Is square is not touched
            {
                sfRectangleShape_setOutlineColor(square, sfColor_fromRGB(150, 150, 150)); // Set outline colour to grey
                sfRectangleShape_setFillColor(square, sfBlack); // Set fill colour to black
            }

            sfRenderWindow_drawRectangleShape(main_window, square, NULL);
            /*
            Function to draw the square on the window. The third parameter is NULL,
            because there is no need in extra states (e.g. texture of square).
            After calling of this function, the square can be moved and changed in any way.
            It will not be shown on the screen.
            */
        }
    }

    char* temp = (char*)malloc(19 * sizeof(char));
    /*
    Allocating memory for char* which will be shown to user. Template: "Population: %number%".
    19 = length of the string + maximum length of the number in counter_population
    */
    strcpy(temp, "Population: "); // temp = "Population: "
    char* temp_population = to_string(counter_population); // Convert the amount of alive square to char*
    strcat(temp, temp_population); // temp += temp_population
    sfText_setString(population, temp); // Set text of temp *char to population sfText
    sfRenderWindow_drawText(main_window, population, NULL);
    /*
    Show text from sfText population on the screen.
    The third parameter is NULL, because there is no need in extra states of the text.
    */

    if (status_of_the_instructions == 1) // If instructions has to be shown
    {
        sfRenderWindow_drawText(main_window, instructions, NULL);
        /*
        Show text from sfText instructions on the screen.
        The third parameter is NULL, because there is no need in extra states of the text.
        */
    }

    sfRenderWindow_display(main_window); // Show the whole main_window

    free(temp); // Clear the memory from char* temp, which was created before in this function
    free(temp_population); // Clear the memory from char* temp_population, which was created in function to_string
}

/*
Function next_frame is used every time when we need to show main_window with the next state of the board.
This function uses every other function in its body.
*/
void next_frame(sfRenderWindow* main_window, sfRectangleShape* square, \
              unsigned short** grid, unsigned short** old_grid, \
              const unsigned short AMOUNT, const unsigned short SIDE_OF_SQUARE, \
              sfText* population, sfText* instructions, \
              unsigned short status_of_the_instructions)
{
    for (unsigned short i = 0; i < AMOUNT; ++i) // Loop for index of row
    {
        for (unsigned short j = 0; j < AMOUNT; ++j) // Loop for index of column
        {
            // In those 2 nested loops we sort through every square of the board
            unsigned short counter = count_neighbours(old_grid, i, j, AMOUNT - 1);
            /*
            Function to count every neighbour of the square. AMOUNT - 1 = index of the last row/column.
            Neighbours have to be counted on the old_grid.
            grid will be changed accordingly, so the amount of neighbours for next square can and will be affected
            if we would use grid for counting neighbours. old_grid updates only after those 2 nested loops.
            */
            kill_or_revive_square(grid, i, j, counter);
            // Function to change the state of the square according to its neighbours
        }
    }
    copy_grid(grid, old_grid, AMOUNT); // Update the state of old_grid (copy it from grid).
    display_main_window(main_window, square, grid, AMOUNT, SIDE_OF_SQUARE, \
                        population, instructions, status_of_the_instructions);
    // Show the main_window with the next state of the board.
}

// Function to copy grid from the first parameter to the second one
void copy_grid(unsigned short** grid, unsigned short** old_grid, const unsigned short AMOUNT)
{
    for (unsigned short i = 0; i < AMOUNT; ++i)
    {
        for (unsigned short j = 0; j < AMOUNT; ++j)
        {
            old_grid[i][j] = grid[i][j];
            // Simply copy the value of each square of grid to each square of old_grid
        }
    }
}

// Function to clear the whole board
void clear_grid(unsigned short** grid, const unsigned short AMOUNT)
{
    for (unsigned short i = 0; i < AMOUNT; ++i)
    {
        for (unsigned short j = 0; j < AMOUNT; ++j)
            {
                grid[i][j] = 0; // Every element is set to default
            }
    }
}

/*
Structure of the file:
number%number%...%number%number$\n - There are AMOUNT numbers
number%number%...%number%number$\n
number%number%...%number%number$\n
.
.
.
number%number%...%number%number$\n - There are AMOUNT rows

If the file is not like this, the function will return 1!
*/
unsigned short load_grid(unsigned short** grid, const unsigned short AMOUNT)
{
    char* filepath = tinyfd_openFileDialog("Choose file (.txt)", "example.txt", 1, (const char * const[]){"*.txt"}, "Text files", 0);
    /*
    Using tinyfiledialogs in order to open File Explorer for choosing a file.
    The first parameter is char* which will be shown to user in the upper left corner of File Explorer window.
    The second parameter is char* which will be shown to user in the field of file name.
    The third parameter defines the amount of possible formats of file. (in our case it is only .txt).
    The forth parameter defines that user can pick only .txt file.
    The fifth parameter is char* which will be shown to user  near the field of file name.
    The sixth parameter defines if the user is allowed to pick more than 1 file. (0 means no)
    Overall, this function will open File Explorer for user with all parameters we need.
    */
    if (filepath == NULL) // filepath = NULL if the file was not opened
    {
        return 2; // Could not open the file
    }
    FILE* grid_file = fopen(filepath, "r"); // Open file in read mode (cannot change it)

    unsigned short key = 3; // This variable is a flag to show whether the file is appropriate
    // 0 - success; 1 - couldn't read the file; 2 - couldn't open the file; 3 - default

    char* number = (char*) malloc(2 * sizeof(char));
    // Allocate memory for char* to read every character from the file and store it
    unsigned short counter = 0; // Counter to read number correctly
    unsigned short row_counter = 0; // Counter to read rows and stop at the AMOUNT
    unsigned short column_counter = 0; // Counter to read columns and stop at the AMOUNT

    if (grid_file == NULL) // grid_file = NULL if the file could not be opened by the program (some mistake)
    {
        key = 2; // // Could not open the file
    }
    else // If the file was successfully opened
    {
        char ch = fgetc(grid_file); // Read the first character from the file
        while ((key == 3) && (ch != EOF))
        /*
        key != 3 if its value was changed. It happens either on a mistake or after successful reading of the file
        ch = EOF at the end of the file
        */
        {
            if (ch == '\n') // The end of a row
            {
                number[counter] = '\0'; // The end of a string
                unsigned short temp = to_int(number); // Convert char* number to unsigned short temp
                counter = 0; // Reset the counter of symbols in number
                row_counter += 1; // Increase row_counter as the end of a row character has been read
                if ((column_counter == AMOUNT) && (row_counter <= AMOUNT) && (temp < 3))
                /*
                1. If column_counter != AMOUNT, then the file cannot be read successfully.
                One of the rows will be incomplete or overflowed.
                2. If row_counter > AMOUNT, then the file cannot be read successfully.
                The amount of rows in the file is more than in the current grid.
                3. If temp > 2, then the file cannot be read successfully.
                The value in one of the squares is not from the set {0, 1, 2}.
                */
                {
                    grid[row_counter - 1][column_counter - 1] = temp;
                    /*
                    Write the value from the file to grid.
                    -1 because the counters are actual, not by index
                    */
                    if (row_counter == AMOUNT) // If it is the last row
                    {
                        key = 0; // Successfully read the file!
                    }
                    column_counter = 0; // Reset the counter of columns
                }
                else // One of 3 conditions above is true
                {
                    key = 1; // Couldn't read the file! The file does not match the sample.
                }
            }
            else if (ch == '$') // The end of a number. '$' was chosen as a separator in function upload_grid
            {
                number[counter] = '\0'; // The end of a string
                unsigned short temp = to_int(number); // Convert char* number to unsigned short temp
                counter = 0; // Reset the counter of symbols in number
                column_counter += 1; // Increase column_counter as the end of a number has been read
                if ((column_counter > AMOUNT) || (temp > 2))
                /*
                1. If column_counter > AMOUNT, then the file cannot be read successfully.
                One of the rows will be overflowed.
                3. If temp > 2, then file cannot be read successfully.
                The value in one of the squares is not from the set {0, 1, 2}.
                */
                {
                    key = 1; // Couldn't read the file! The file does not match the sample.
                }
                else
                {
                    grid[row_counter][column_counter - 1] = temp;
                    /*
                    Write the value from the file to grid.
                    Here the row_counter shows index, column counter is actual.
                    */
                }
            }
            else
            {
                if (counter == 0) // If the program has just read '\n' or '$'
                {
                    number[counter] = ch; // New character is written to number
                    counter += 1; // Increase the counter of symbols in number
                }
                else
                {
                    key = 1; // Could not read the file! Length of a number > length of any possible number in a square of the grid
                }
            }
            ch = fgetc(grid_file); // Read the next character from the file
        }
    }

    free(number); // Clear the memory from char* number
    fclose(grid_file); // Close the file

    // The memory from filepath should not be cleaned. tinyfiledialogs does not use dynamic memory.

    return key; // Return the result of opening the file
}

/*
Structure of the file:
number%number%...%number%number$\n - There are AMOUNT numbers
number%number%...%number%number$\n
number%number%...%number%number$\n
.
.
.
number%number%...%number%number$\n - There are AMOUNT rows

The function upload_grid will create a .txt file exactly by this sample.
*/
void upload_grid(unsigned short** grid, const unsigned short AMOUNT)
{
    char* filepath = tinyfd_saveFileDialog("Save current grid as (.txt)", "example.txt", 1, (const char * const[]){"*.txt"}, "Text files");
    /*
    Using tinyfiledialogs in order to open File Explorer for creating a file or choosing a file to replace it.
    The first parameter is char* which will be shown to user in the upper left corner of File Explorer window.
    The second parameter is char* which will be shown to user in the field of file name.
    The third parameter defines the amount of possible formats of file. (in our case it is only .txt).
    The forth parameter defines that user can pick only .txt file.
    The fifth parameter is char* which will be shown to user  near the field of file name.
    Overall, this function will open File Explorer for user with all parameters we need.
    */

    FILE* grid_file = fopen(filepath, "w");
    /*
    Open or create the file with chosen filepath.
    In case of opening the file will be completely overwritten.
    */
    if (grid_file == NULL) // grid_file = NULL if the file could not be opened/created
    {
        printf("\nCould not open/create the file!"); // Mistake to the console
    }
    else // If the file is opened/created
    {
        for (unsigned short i = 0; i < AMOUNT; ++i) // Loop for index of row
        {
            for (unsigned short j = 0; j < AMOUNT; ++j) // Loop for index of column
            {
                char* temp = to_string(grid[i][j]); // Convert number of every square from grid to char*
                fprintf(grid_file, temp); // Write temp to the file
                free(temp); // Clear the memory from temp (to_string allocates memory for char*)
                fprintf(grid_file, "$"); // Write separator to the file
            }
            fprintf(grid_file, "\n"); // Write the end of a row to the file
        }
        fclose(grid_file); // Close the file
    }

}
