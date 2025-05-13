#include "headers/Dict.h"
#include "headers/Exceptions.h"
#include <iostream>
#include <windows.h>

void runTests();
const std::string DATAPATH = "../data/DictionaryT.json";

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    try {
        std::cout << "\t\tЗапуск тестов\n" << std::endl;
        runTests();
        std::cout << "\n\t\tВсе тесты прошли!\n\n" << std::endl;

        Dict dictionary(DATAPATH);
        dictionary.run();
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << std::endl;
        system("pause");
        return EXIT_FAILURE;
    }

    system("pause");
    return EXIT_SUCCESS;
}

