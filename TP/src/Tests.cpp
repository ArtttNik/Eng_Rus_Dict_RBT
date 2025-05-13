#include "../headers/Dict.h"
#include "../headers/Exceptions.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

void runTests() {

    const std::string testFile = "temp.json";
    std::remove(testFile.c_str());

    Dict dict(testFile);

    try {
        dict.insert("hello", "привет;здравствуй");
        const auto& translations = dict.findTranslationByWord("hello");
        if (translations.size() == 2 &&
            translations.front() == "здравствуй" &&
            translations.back() == "привет") {
            std::cout << "[PASSED] :-D Добавление в словарь: Успешно добавлено слово 'hello'" << std::endl;
        } else {
            throw std::runtime_error("Incorrect translations for 'hello'");
        }
    } catch (const std::exception& e) {
        std::cout << "[FAILED] :-( Добавление в словарь: Ошибка: " << e.what() << std::endl;
    }

    try {
        dict.insert("hello", "привет");
        std::cout << "[FAILED] :-( Проверка дубликата слова: Ошибка: Ожидалось исключение DuplicateWord" << std::endl;
    } catch (const DuplicateWord&) {
        std::cout << "[PASSED] :-D Проверка дубликата слова: Корректно выброшено исключение, тк существует 'hello'" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[FAILED] :-( Проверка дубликата слова: Ошибка с неожиданным исключением: " << e.what() << std::endl;
    }

    try {
        std::string newTranslation = "хеллоу";
        dict.addTranslation("hello", newTranslation);
        const auto& translations = dict.findTranslationByWord("hello");
        if (translations.size() == 3 &&
            std::find(translations.begin(), translations.end(), "хеллоу") != translations.end()) {
            std::cout << "[PASSED] :-D Добавление перевода: Успешно добавлен перевод" << std::endl;
        } else {
            throw std::runtime_error("Translation 'хеллоу' not found or incorrect size");
        }
    } catch (const std::exception& e) {
        std::cout << "[FAILED] :-( Добавление перевода: Ошибка: " << e.what() << std::endl;
    }

    try {
        std::string dupTranslation = "привет";
        dict.addTranslation("hello", dupTranslation);
        std::cout << "[FAILED] :-( Проверка дубликата перевода: Ошибка: Ожидалось исключение DuplicateTranslation" << std::endl;
    } catch (const DuplicateTranslation&) {
        std::cout << "[PASSED] :-D Проверка дубликата перевода: Корректно выброшено исключение для существующего перевода" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[FAILED] :-( Проверка дубликата перевода: Ошибка с неожиданным исключением: " << e.what() << std::endl;
    }

    try {
        dict.removeTranslation("hello", "хеллоу");
        const auto& translations = dict.findTranslationByWord("hello");
        if (translations.size() == 2 &&
            std::find(translations.begin(), translations.end(), "хеллоу") == translations.end()) {
            std::cout << "[PASSED] :-D Удаление перевода: Успешно удален перевод" << std::endl;
        } else {
            throw std::runtime_error("Translation 'хеллоу' still present or incorrect size");
        }
    } catch (const std::exception& e) {
        std::cout << "[FAILED] :-( Удаление перевода: Ошибка: " << e.what() << std::endl;
    }

    try {
        dict.removeTranslation("hello", "привет");
        dict.removeTranslation("hello", "здравствуй");
        try {
            dict.findTranslationByWord("hello");
            std::cout << "[FAILED] :-( Удаление последнего перевода: Ошибка: Ожидалось исключение NoFoundWord" << std::endl;
        } catch (const NoFoundWord&) {
            std::cout << "[PASSED] :-D Удаление последнего перевода: Корректно удалены последние переводы и слово" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "[FAILED] :-( Удаление последнего перевода: Ошибка: " << e.what() << std::endl;
    }

    try {
        dict.insert("world", "мир");
        dict.remove("world");
        try {
            dict.findTranslationByWord("world");
            std::cout << "[FAILED] :-( Удаление слова: Ошибка: Ожидалось исключение NoFoundWord" << std::endl;
        } catch (const NoFoundWord&) {
            std::cout << "[PASSED] :-D Удаление слова: Успешно удалено слово" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "[FAILED] :-( Удаление слова: Ошибка: " << e.what() << std::endl;
    }

    try {
        dict.insert("test", "тест;проверка");
        const auto& translations = dict.findTranslationByWord("test");
        if (translations.size() == 2 &&
            translations.front() == "проверка" &&
            translations.back() == "тест") {
            std::cout << "[PASSED] :-D Поиск переводов по слову: Успешно найдены переводы" << std::endl;
        } else {
            throw std::runtime_error("Incorrect translations for 'test'");
        }
    } catch (const std::exception& e) {
        std::cout << "[FAILED] :-( Поиск переводов по слову: Ошибка: " << e.what() << std::endl;
    }

    try {
        const std::string translation = "тест";
        const std::string word = dict.findWordByTranslation(translation);
        if (word == "test") {
            std::cout << "[PASSED] :-D Поиск слова по переводу: Успешно найдено слово 'test' для перевода 'тест'" << std::endl;
        } else {
            throw std::runtime_error("Incorrect word for 'тест'");
        }
    } catch (const std::exception& e) {
        std::cout << "[FAILED] :-( Поиск слова по переводу: Ошибка: " << e.what() << std::endl;
    }

    try {
        std::string input = "test";
        const std::string result = dict.autoTranslate(input);
        if (result == "проверка; тест") {
            std::cout << "[PASSED] :-D Автоматический перевод английский: Успешно переведено" << std::endl;
        } else {
            throw std::runtime_error("Incorrect translation for 'test'");
        }
    } catch (const std::exception& e) {
        std::cout << "[FAILED] :-( Автоматический перевод английский: Ошибка: " << e.what() << std::endl;
    }

    std::remove(testFile.c_str());
}