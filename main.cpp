#include "src/file_search_lib.h"
#include <iostream>

int main() {
    std::string fileName;
    std::cout << "Enter the file name to search: ";
    std::cin >> fileName;
    FileSearch::searchFile("/", fileName);
    return 0;
}