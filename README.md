# CppSudoku

This is a console-based Sudoku game created using C++ and the cURL library to fetch Sudoku puzzles from an online API.

## How to Build and Run

### Prerequisites

1. **Compiler:** Make sure you have a C++ compiler installed, such as `g++` or `clang++`.
2. **cURL library:** Ensure that the cURL library is installed on your system. You can download it from [cURL Download](https://curl.se/download.html).

### Building the Program

1. Open a command prompt.
2. Navigate to the directory where you have saved the source code file (`sudoku.cpp`).
3. Use the following command to compile the program:

    ```sh
    g++ -o sudoku.exe sudoku.cpp -lcurl
    ```

### Running the Program

1. After successful compilation, you will get an executable file named `sudoku.exe`.
2. Run the program using the following command:

    ```sh
    sudoku.exe
    ```

## How to Play

1. When you start the game, a Sudoku board will be displayed along with the difficulty level and the number of mistakes you have made.
2. To make a move, enter the row number, column number, and the number you want to place in that cell, separated by spaces. For example: `3 4 5`.
3. The program will check if your move is valid and update the board accordingly. If the move is invalid or incorrect, you will be notified, and the number of mistakes will increase.
4. The game continues until you fill the entire board correctly.
5. Once the board is filled correctly, a congratulatory message will be displayed along with the number of mistakes you made.

## Dependencies

- **cURL library:** This library is used to make HTTP requests to fetch Sudoku puzzles from an online API.
- **nlohmann/json:** This library is used to parse JSON responses from the API. Ensure to include the header file `json.hpp` in your project.

## Files

- `sudoku.cpp`: The main source code file for the Sudoku game.
- `curl/include/curl/curl.h`: Header file for the cURL library.
- `json.hpp`: Header file for the nlohmann/json library.

## Note

This program is designed to run on Windows. Ensure you have the necessary dependencies and a suitable development environment set up on your system.

Happy Playing!
