#include "headers/Dict.h"
#include "headers/Exceptions.h"
#include "tests/Tests.h"
#include <iostream>

void runProgram();
void runForUser();
void runForTests();

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    DATAPATH = R"(..\data\DictionaryT.json)";

    runProgram();

    return EXIT_SUCCESS;
}

void runProgram() {
    std::cout << "\n\t\tАнгло-русский словарь. Красно-чёрное дерево\n";
    std::cout << "\nДоступные команды:\n"
              << "1. TEST - Для запуска автоматического тестирования программы.\n"
              << "2. USER - Добавить новое слово и его переводы в словарь.\n"
              << "3. EXIT - Завершение работы.\n";
    std::cout << "Ваш выбор: ";

    while (true) {
        std::string command;
        std::getline(std::cin, command);
        trimSpaces(command);
        toUpperCaseE(command);

        try {
            if (command == "TEST" || command == "1") {
                DATAPATH = R"(..\data\DictionaryT.json)";
                runForTests();
            } else if (command == "USER" || command == "2") {
                runForUser();
            } else if (command == "EXIT" || command == "3") {
                break;
            } else {
                std::cout << "Неизвестная команда, попробуйте ещё раз\n";
                std::cout << "Ваш выбор:   ";
            }
        } catch (const ProgramExit&) {
            break;
        } catch (const FailOfMemoryAllocation& error) {
            std::cerr << error.what() << "\n";
            break;
        } catch (const std::exception& error) {
            std::cerr << error.what() << "\n";
            break;
        }
    }
}

void runForUser() {
    try {
        Dict dictionary;
        dictionary.run();
    } catch (const ProgramExit&) {
        throw;
    }
}

void runForTests() {
    runAllTests();
}