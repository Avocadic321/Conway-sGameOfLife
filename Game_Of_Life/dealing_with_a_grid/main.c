#include "functions.h"

int main(void)
{
    unsigned short AMOUNT = 1000; // Amount of squares on the board (will be defined by user)
    const unsigned short SIDE_OF_SQUARE = 15; // Side of each square on the board
    unsigned short key = 1;

    printf("Hello! Welcome to Conway's game of life!\n\
Please, input the amount of squares in each row of the board: "); // Prompt to input for user
    while (key) // Loop will stop after appropriate input
    {
        key = 0;
        char input[10]; // String to get an input from the user
        scanf("%s", input); // Input from the user
        unsigned short length = strlen(input);
        for (unsigned short i = 0; i < length; ++i) // Loop to check if the input is a number
        {
            if (!isdigit(input[i]))
            {
                key = 1; // key = 1 means that the input is not a number
            }
        }
        if (!key)
        {
            AMOUNT = to_int(input); // If it is a number, convert it to unsigned short type
            key = AMOUNT > 100; // If the number is more than 100, the user should repeat the input
        }
        if (key) // If it is not a number or a number which is more than 100
        {
            printf("Repeat the input, please. Amount of squares in each row should be less than 101: ");
            // Prompt to repeat input
        }
    }

    sfVideoMode mode = {AMOUNT * SIDE_OF_SQUARE, AMOUNT * SIDE_OF_SQUARE, 64};
    // Create Video Mode: AMOUNT*SIDE_OF_SQUARE x AMOUNT*SIDE_OF_SQUARE, 64 bits per pixel
    sfRenderWindow* main_window = sfRenderWindow_create(mode, "Conway's Game of Life", sfClose, NULL);
    /*
    Create a pointer for main_window:
    sfRenderWindow_create(sfVideoMode, Title, Opportunity to close the window, additional settings)
    */

    if (main_window == NULL) // main_window = NULL if the program could not open it
    {
        printf("Couldn't create the main window!");
        return 1;
    }

    sfFont* font = sfFont_createFromFile("Roboto-Bold.ttf"); // Font for the text
    if (font == NULL) // Font = NULL if the program could not open it
    {
        printf("Font has not been found!");
        return 1;
    }

    sfText *population = sfText_create(); // Create a pointer for a text
    sfText_setFont(population, font); // Set font for the text
    sfText_setCharacterSize(population, SIDE_OF_SQUARE * 2); // Set size of characters in the text
    sfText_setString(population, "Population: 0");
    // Set text (0 in the start, then changes in function display_main_window)
    sfText_setFillColor(population, sfWhite); // Set colour of the text
    sfText_setOutlineColor(population, sfBlack); // Set outline colour of the text
    sfText_setOutlineThickness(population, 2); // Set outline thickness of the text
    sfText_setPosition(population, (sfVector2f){SIDE_OF_SQUARE, SIDE_OF_SQUARE});
    // Set position of the text: sfVector2f is a structure: {float x coordinate, float y coordinate}

    sfText *instructions = sfText_create(); // Create a pointer for a text
    sfText_setFont(instructions, font); // Set font for the text
    sfText_setCharacterSize(instructions, SIDE_OF_SQUARE * 2); // Set size of characters in the text
    sfText_setString(instructions, \
"Instructions:\n\
TAB - show/hide instructions\n\
Space - start/pause the game\n\
LShift - download .txt board from your device\n\
Enter - upload .txt current board to your device\n\
Backspace - clear the board\n\
Escape - close the game"); // Set text
    sfText_setFillColor(instructions, sfWhite); // Set outline colour of the text
    sfText_setOutlineColor(instructions, sfBlack); // Set outline colour of the text
    sfText_setOutlineThickness(instructions, 2); // Set outline thickness of the text
    sfText_setPosition(instructions, (sfVector2f){SIDE_OF_SQUARE, SIDE_OF_SQUARE * 5});
    /* Set position of the text: sfVector2f is a structure: {float x coordinate, float y coordinate}
    The instructions will be lower than population by 4 * SIDE_OF_SQUARE */

    unsigned short** grid = (unsigned short**) calloc(AMOUNT, sizeof(unsigned short*));
    // Allocation of memory for the grid (calloc to fill it in with 0)
    unsigned short** old_grid = (unsigned short**) calloc(AMOUNT, sizeof(unsigned short*));
    /* Allocation of memory for the old_grid. It will keep previous position of the board.
    Have to be created in order to check neighbours of each square. Grid will store new states of the squares */

    if (grid == NULL) // grid = NULL if the program could not allocate memory for grid
    {
        printf("Allocation of memory failed");
        return 1;
    }

    if (old_grid == NULL) // old_grid = NULL if the program could not allocate memory for old_grid
    {
        printf("Allocation of memory failed");
        return 1;
    }

    for (unsigned short i = 0; i < AMOUNT; ++i) // Loop for allocation of memory for each row
    {
        grid[i] = (unsigned short*) calloc(AMOUNT, sizeof(unsigned short));
        // Allocation of memory for i row for grid
        if (grid[i] == NULL) // grid[i] = NULL if the program could not allocate memory for grid[i]
        {
            printf("Allocation of memory failed");
            return 1;
        }

        old_grid[i] = (unsigned short*) calloc(AMOUNT, sizeof(unsigned short));
        // Allocation of memory for i row for old_grid
        if (old_grid[i] == NULL) // old_grid[i] = NULL if the program could not allocate memory for old_grid[i]
        {
            printf("Allocation of memory failed");
            return 1;
        }
    }

    unsigned short status_of_the_game = 0; // State of game: 0 - pause, 1 - play
    unsigned short status_of_the_instructions = 1; // State of instructions: 0 - not visible, 1 - visible
    key = 1; // Key to close the main window by escape button

    sfRectangleShape* square = sfRectangleShape_create();
    /* Creating a pointer for 1 square. This pointer will be used to create squares of different colours
    to show them in the main_window (memory will be allocated only for 1 square) */
    sfRectangleShape_setFillColor(square, sfBlack);
    // Set colour of the square. Black: default, red: dead square, white: alive square.
    sfRectangleShape_setOutlineThickness(square, 1);
    // Set outline thickness for a square. Grey: default and dead square, black: alive square.
    sfRectangleShape_setOutlineColor(square, sfColor_fromRGB(150, 150, 150));
    // Set outline colour: grey (sfColor_fromRGB(red, green, blue) will transfer RGB to sfColor)
    sfRectangleShape_setSize(square, (sfVector2f){SIDE_OF_SQUARE, SIDE_OF_SQUARE});
    /* Set size of the square. sfVector2f is a structure: {float x coordinate, float y coordinate},
    so the size of square will be SIDE_OF_SQUARE - 0, SIDE_OF_SQUARE - 0 */

    // Creating a timer
    sfClock* clock = sfClock_create();

    display_main_window(main_window, square, grid, AMOUNT, SIDE_OF_SQUARE, \
                        population, instructions, status_of_the_instructions);
    // Function for displaying main window. Read more about it in function.c

    while (sfRenderWindow_isOpen(main_window))
    // Main loop of the game. If main_window will be closed, sfRenderWindow_isOpen(main_window) = false
    {
        sfEvent event; // Creating a union for every possible event
        while (sfRenderWindow_pollEvent(main_window, &event))
        /*
        sfRenderWindow_pollEvent(main_window, &event) checks for events every turn of the loop.
        If there is any, it will return 1 and in the sfEvent event will be put a structure for each type of event
        (to check which type in it will be used event.type)
        */
        {
            if (event.type == sfEvtClosed) // If user presses "X" button
            {
                sfRenderWindow_close(main_window); // Close the main window
            }
            if (event.type == sfEvtKeyPressed) // If user presses any keyboard button
            {
                if (event.key.code == sfKeyEscape) // If user wants to close the window by button Escape
                {
                    key = 0; // Change the value of key to close the window
                    sfRenderWindow_close(main_window); // Close the main window
                }
                else if (event.key.code == sfKeyRight) // If user presses right arrow: show next state of the game
                {
                    next_frame(main_window, square, grid, old_grid, AMOUNT, \
                               SIDE_OF_SQUARE, population, instructions, status_of_the_instructions);
                    // Function for showing next state of the game. Read more about it in function.c
                }
                else if (event.key.code == sfKeySpace) // If user presses space: start/pause the game
                {
                    if (status_of_the_game) // If the game is not on a pause
                    {
                        status_of_the_game = 0; // Pause the game
                        display_main_window(main_window, square, grid, AMOUNT, \
                                            SIDE_OF_SQUARE, population, instructions, status_of_the_instructions);
                        // Show the board on a pause
                    }
                    else // If the game is on a pause
                    {
                        status_of_the_game = 1; // Start the game
                        status_of_the_instructions = 0; // Hide instructions
                    }
                }
                else if (event.key.code == sfKeyEnter)
                /*
                If user presses enter: pause the game and
                open File Explorer to upload .txt file with current state of the game.
                If user closes it, nothing happens.
                */
                {
                    status_of_the_game = 0; // Pause the game
                    upload_grid(grid, AMOUNT);
                    // Function for uploading current state of the game. Read more about it in function.c
                }
                else if (event.key.code == sfKeyLShift)
                /*
                If user presses LShift: pause the game and
                open File Explorer to download .txt file with some state of the game.
                If user closes it, nothing happens. If the program could not open the file, key = 2.
                If the program has found that file is incorrect (is not readable for this algorithm), key = 1.
                */
                {
                    status_of_the_game = 0; // Pause the game
                    unsigned short key = load_grid(grid, AMOUNT);
                    // Function for downloading state of the game from .txt file. Read more about it in function.c
                    if (key == 2) // If the program could not open the file
                    {
                        printf("Could not open the board!"); // Message to a user
                    }
                    else if (key == 1) // If the program has found that file is incorrect (is not readable for this algorithm)
                    {
                        copy_grid(old_grid, grid, AMOUNT);
                        /*
                        In this branch load_grid function has been done only for some part,
                        so part of the grid has been overwritten.
                        By calling copy_grid function (read more about it in function.c) the program will
                        copy board state from old_grid to grid. Because of that the game will work even after
                        unsuccessful reading of the file.
                        */
                        printf("Could not read the board!"); // Message to a user
                    }
                    else // If the program has read the .txt file successfully
                    {
                        copy_grid(grid, old_grid, AMOUNT);
                        /*
                        Function for copying state of the game from first parameter to the second one.
                        Read more about it in function.c
                        */
                        display_main_window(main_window, square, grid, AMOUNT, \
                                            SIDE_OF_SQUARE, population, instructions, status_of_the_instructions);
                        // Show the board on a pause with new state of the game from the .txt file
                    }
                }
                else if (event.key.code == sfKeyBackspace)
                // If user presses Backspace: fully clear the board (every square = default)
                {
                    if (status_of_the_game == 0) // Only if the game is on a pause
                    {
                        clear_grid(grid, AMOUNT);
                        // Function to clear the board. Read more about it in function.c
                        copy_grid(grid, old_grid, AMOUNT);
                        /*
                        Function to copy grid to the old one (otherwise, function next_frame will work incorrectly)
                        Read more about it in function.c
                        */
                        display_main_window(main_window, square, grid, AMOUNT, \
                                            SIDE_OF_SQUARE, population, instructions, status_of_the_instructions);
                        // Show the cleared board on a pause
                    }
                }
                else if (event.key.code == sfKeyTab)
                // If user presses Tab: show or hide instructions
                {
                    status_of_the_game = 0; // Pause the game
                    status_of_the_instructions = 1 - status_of_the_instructions;
                    // Hide or show instructions (status_of_the_instructions can be either 0 or 1)
                    display_main_window(main_window, square, grid, AMOUNT, SIDE_OF_SQUARE, \
                                        population, instructions, status_of_the_instructions);
                    // Display main window with or without instructions
                }
            }
            if (event.type == sfEvtMouseButtonPressed) // If user presses mouse button
            {
                if (status_of_the_game == 0) // Only if the game is on a pause
                {
                    int x = event.mouseButton.x / SIDE_OF_SQUARE, y = event.mouseButton.y / SIDE_OF_SQUARE;
                    /*
                    Structure event.mouseButton contains the current position of mouse in coordinates
                    ({event.mouseButton.x; event.mouseButton.y}). It should be divided by SIDE_OF_SQUARE
                    to get row and column of the square in which cursor is located.
                    (y shows row of a square and x shows column of a square)
                    */
                    if (grid[y][x] == 2) { grid[y][x] = 0; } // If chosen square is dead, than make it default
                    grid[y][x] = 1 - grid[y][x];
                    // In this line grid[y][x] is either 1 or 0, so change the value for the opposite
                    copy_grid(grid, old_grid, AMOUNT);
                    /*
                    Function to copy grid to the old one (otherwise, function next_frame will work incorrectly)
                    Read more about it in function.c
                    */
                    display_main_window(main_window, square, grid, AMOUNT, SIDE_OF_SQUARE, \
                                        population, instructions, status_of_the_instructions);
                    // Display main window with new state of a square
                }
            }
        }

        if (key) // If window was not closed by Escape button
        {
            sfTime time = sfClock_getElapsedTime(clock); // Take time from the timer
            int milliseconds = sfTime_asMilliseconds(time); // Convert time from sfTime to milliseconds
            if (milliseconds % 50 == 0) // 1000 / 50 = 20 fps (every 50th millisecond the screen will be updated)
            {
                if (status_of_the_game) // If the game is active
                {
                    next_frame(main_window, square, grid, old_grid, AMOUNT, \
                               SIDE_OF_SQUARE, population, instructions, status_of_the_instructions);
                    // Function for showing next state of the game. Read more about it in function.c
                }
            }
        }
    }

    sfClock_destroy(clock); // Destroying the timer (free memory)

    // Destroying font and text (free memory)
    sfFont_destroy(font);
    sfText_destroy(population);
    sfText_destroy(instructions);

    for (unsigned short i = 0; i < AMOUNT; ++i)
    {
        free(old_grid[i]); // Free memory in each row of old_grid

        free(grid[i]); // Free memory in each row of grid
    }

    free(old_grid); // Free old_grid

    free(grid); // Free grid

    sfRectangleShape_destroy(square); // Destroying the square (free memory)
    sfRenderWindow_destroy(main_window); // Destroying the main_window (free memory)
}
