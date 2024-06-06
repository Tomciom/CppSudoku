cmake_minimum_required(VERSION 3.26)
project(CppSudoku)

set(CMAKE_CXX_STANDARD 14)

# Dodaj ścieżki do plików nagłówkowych
include_directories(C:/dev/curl/include)

# Dodaj ścieżki do bibliotek
link_directories(C:/dev/curl/lib)

# Dodaj plik źródłowy
add_executable(CppSudoku main.cpp)

# Linkuj bibliotekę cURL
target_link_libraries(CppSudoku curl)