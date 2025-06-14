#include "headers/Dict.h"
#include <iostream>
#include <windows.h>

#include "headers/Exceptions.h"

const std::string DATAPATH = "../data/DictionaryT.json";

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    try {
        std::cout << "\t\tЗапуск тестов\n" << std::endl;
        runTests();
        std::cout << "\n\t\tЗавершение тестирования!\n\n" << std::endl;

        Dict dictionary(DATAPATH);
        dictionary.run();
    } catch (const Exit&) {
        system("pause");
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
        system("pause");
        return EXIT_FAILURE;
    }
}

