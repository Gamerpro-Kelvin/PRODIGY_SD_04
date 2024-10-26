#include <stdio.h>

// Function prototypes
int main();
int input_grid();
int validate_grid();
int solve_cell(int row, int column);
int is_valid(int row, int column, int value);
void print_grid();

// Global grids: one for the original puzzle and another for solving
int original_grid[9][9], solved_grid[9][9];

// Main function
int main()
{
    // Input the grid from the user
    if (!input_grid()) {
        printf("Error: Unable to input grid\n");
        return 0;
    }

    // Validate the initial puzzle configuration
    printf("\nValidating puzzle... ");
    if (!validate_grid()) {
        printf("Invalid!\n");
        return 0;
    }

    printf("Valid.\n\nSolving the following puzzle:\n");
    print_grid();

    // Attempt to solve the puzzle using backtracking
    if (!solve_cell(0, 0)) {
        printf("\nUnfortunately, your puzzle is unsolvable\n");
        return 0;
    }

    // Print the solved puzzle
    printf("\nPuzzle solved:\n");
    print_grid();
    return 0;
}

// Function to input the Sudoku grid from the user
int input_grid()
{
    int cell_value, row = 0, col = 0;

    printf("Enter rows line by line. Use numbers for known cells, ");
    printf("zero or dot for missing cells.\nRow 1: ");

    // Read each character entered by the user
    while (row < 9) {
        cell_value = getchar();

        // Check if character is a digit or a dot
        if (('0' <= cell_value && cell_value <= '9') || cell_value == '.') {
            if (col > 8) {
                printf("Error: Each row has a maximum of 9 cells\n");
                return 0;
            }

            // Convert character to integer and assign to grids
            if (cell_value != '.')
                original_grid[row][col] = solved_grid[row][col] = cell_value - '0';

            col++;

        } else if (cell_value == '\n') {  // End of row
            col = 0;
            row++;
            if (row < 9) printf("Row %i: ", row + 1);
        }
    }

    return 1;
}

// Function to validate the initial grid before solving
int validate_grid()
{
    for (int row = 0; row < 9; row++)
        for (int col = 0; col < 9; col++)
            if (solved_grid[row][col] && !is_valid(row, col, solved_grid[row][col]))  // Check each filled cell
                return 0;
    return 1;
}

// Recursive backtracking function to solve each cell
int solve_cell(int row, int col)
{
    // Move to the next row after the last column
    if (col == 9) {
        col = 0;
        row++;
    }

    // If reached the end, puzzle is solved
    if (row == 9)
        return 1;

    // Skip cells that are pre-filled in the original puzzle
    if (original_grid[row][col] != 0)
        return solve_cell(row, col + 1);

    // Try placing each number from 1 to 9 in the cell
    for (int num = 1; num <= 9; num++) {
        if (is_valid(row, col, num)) {
            solved_grid[row][col] = num;

            // Recursively try to solve the next cell
            if (solve_cell(row, col + 1))
                return 1;
        }

        // Reset cell if number leads to an unsolvable state
        solved_grid[row][col] = 0;
    }
    return 0; // Trigger backtracking if no number is valid
}

// Function to check if placing a number in a cell is valid
int is_valid(int row, int col, int num)
{
    // Ensure that original puzzle constraints are respected
    if (original_grid[row][col] != 0 && original_grid[row][col] != num)
        return 0;

    // Check if the number already exists in the row or column
    for (int i = 0; i < 9; i++) {
        if (i != col && solved_grid[row][i] == num)  // Row check
            return 0;
        if (i != row && solved_grid[i][col] == num)  // Column check
            return 0;
    }

    // Check the 3x3 subgrid containing the cell
    int start_row = (row / 3) * 3, start_col = (col / 3) * 3;
    for (int r = start_row; r < start_row + 3; r++)
        for (int c = start_col; c < start_col + 3; c++)
            if ((r != row || c != col) && solved_grid[r][c] == num)
                return 0;

    return 1;
}

// Function to print the grid in a formatted style
void print_grid()
{
    for (int row = 0; row < 10; row++) {
        if (row % 3 == 0)
            printf("+-------+-------+-------+\n");

        if (row == 9) return;

        for (int col = 0; col < 9; col++) {
            if (col % 3 == 0)
                printf("| ");
            solved_grid[row][col] != 0 ? printf("%d ", solved_grid[row][col]) : printf(". ");
        }

        printf("|\n");
    }
}

