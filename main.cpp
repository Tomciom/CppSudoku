#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "curl/include/curl/curl.h"
#include "json.hpp"
#include <chrono>
#include <thread>

using json = nlohmann::json;
#define BOARD_SIZE 9

// Function to clean the terminal screen
void clearScreen() {
    std::cout << "\033[H\033[J";
}

// Function to display the board
void displayBoard(const std::vector<std::vector<int>>& board) {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        if (row % 3 == 0 && row != 0) {
            std::cout << "---------------------" << std::endl;
        }
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (col % 3 == 0 && col != 0) {
                std::cout << "| ";
            }
            std::cout << board[row][col] << " ";
        }
        std::cout << std::endl;
    }
}

// Write callback function for curl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to get Sudoku board from API
std::tuple<std::vector<std::vector<int>>, std::vector<std::vector<int>>, std::string> getSudokuBoardAndSolution() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_URL, "https://sudoku-api.vercel.app/api/dosuku");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    else {
        nlohmann::json j = nlohmann::json::parse(readBuffer);
        std::ofstream file("output.json");
        file << std::setw(4) << j << std::endl;
    }

    // Parse the JSON response
    auto jsonResponse = json::parse(readBuffer);
    auto boardJson = jsonResponse["newboard"]["grids"][0]["value"];
    auto solutionJson = jsonResponse["newboard"]["grids"][0]["solution"];
    auto difficulty = jsonResponse["newboard"]["grids"][0]["difficulty"];


    std::vector<std::vector<int>> board(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0));
    std::vector<std::vector<int>> solution(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0));
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            board[row][col] = boardJson[row][col];
            solution[row][col] = solutionJson[row][col];
        }
    }

    return {board, solution, difficulty};
}

// Function to check if a move is valid
bool isValidMove(const std::vector<std::vector<int>>& board, int row, int col, int num) {
    for (int x = 0; x < BOARD_SIZE; ++x) {
        if (board[row][x] == num || board[x][col] == num || board[(row/3)*3 + x/3][(col/3)*3 + x%3] == num) {
            return false;
        }
    }
    return true;
}

// Function to check if the board is full
bool isBoardFull(const std::vector<std::vector<int>>& board) {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (board[row][col] == 0) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    auto [board, solution, difficulty] = getSudokuBoardAndSolution();
    int mistakes = 0;

    while (true) {
        clearScreen();
        std::cout << "Welcome to Sudoku Game!" << std::endl << std::endl;
        std::cout << "Difficulty: " << difficulty << "      " << "Mistakes: " << mistakes << std::endl << std::endl;
        displayBoard(board);

        if (isBoardFull(board)) {
            clearScreen();
            std::cout << "Congratulations! You have solved the Sudoku puzzle! ";
                switch (mistakes){
                    case 0:
                        std::cout << "You made no mistakes!!! Awesome!" << std::endl << std::endl;
                        break;
                    case 1:
                        std::cout << "You made 1 mistake." << std::endl << std::endl;
                        break;
                    default:
                        std::cout << "You made " << mistakes << " mistakes." << std::endl << std::endl;
                }
            std::cout << "Press any key to exit...";
            std::cin.get(); // Wait for user input
            break;
        }

        int row, col, num;
        std::cout << std::endl << "Enter row (1-9), column (1-9), and number (1-9) separated by spaces: ";
        std::cin >> row >> col >> num;
        --row; --col; // Adjust for 0-based indexing

        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
            if (isValidMove(board, row, col, num)) {
                if (num == solution[row][col]) {
                    board[row][col] = num;
                } else {
                    std::cout << "Incorrect move. Try again." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    mistakes++;
                }
            } else {
                std::cout << "Invalid move. Try again." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                mistakes++;
            }
        } else {
            std::cout << "Invalid row or column. Try again." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    return 0;
}
