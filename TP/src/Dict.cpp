#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <sstream>

#include "../headers/Dict.h"
#include "../headers/Exceptions.h"
#include "../headers/MyJson.h"

Dict::Dict(const std::string& filename) {
    loadFromFile(filename);
    dataPath_ = filename;
}

Dict::~Dict() = default;

void Dict::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        const std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

        MyJson::parse(content, tree_);
    }
}

void Dict::writeToFile() const {
    std::ofstream fileOut(dataPath_);
    if (!fileOut)
        throw FailOfMemoryAllocation("Dictionary.json file");

    fileOut << MyJson::convertDictToJsonString(tree_);
}

void Dict::removeFromFile(const std::string& word, const std::string* translation) {
    if (translation == nullptr) {
        tree_.erase(word);
    } else if (tree_.find(word) != tree_.end()) {
        auto& translations = tree_[word];
        translations.erase(std::remove(translations.begin(), translations.end(), *translation),
                           translations.end());
        if (translations.empty()) {
            tree_.erase(word);
        }
    }

    std::ofstream fileOut(dataPath_);
    if (!fileOut) {
        throw FailOfMemoryAllocation("Dictionary.json file");
    }
    fileOut << MyJson::convertDictToJsonString(tree_);
}

void Dict::insertSorted(std::list<std::string>& list, const std::string& value) {
    const auto it = std::lower_bound(list.begin(), list.end(), value);
    if (it == list.end() || *it != value)
        list.insert(it, value);
}

void Dict::processTranslations(std::istringstream& iss, std::list<std::string>& translationsList) {
    std::string translation;
    if (std::getline(iss, translation, ';')) {
        trimSpaces(translation);
        if (!translation.empty()) {
            if (!isRussianWord(translation))
                throw InvalidTranslationLanguage();
            toLowerCaseR(translation);
            insertSorted(translationsList, translation);
        }

        processTranslations(iss, translationsList);
    }
}

void Dict::insert(const std::string& word, const std::string& translationsStr) {
    if (word.empty())
        throw InvalidWordLanguage();

    if (tree_.find(word) != tree_.end())
        throw DuplicateWord();

    if (translationsStr.empty())
        throw EmptyValue();

    std::list<std::string> translationsList;
    std::istringstream iss(translationsStr);

    processTranslations(iss, translationsList);

    tree_[word] = translationsList;
    writeToFile();
}

void Dict::remove(const std::string & word) {
    if (word.empty())
        throw EmptyValue();

    if (tree_.erase(word))
        removeFromFile(word);
    else
        throw NoFoundWord();
}

void Dict::addTranslation(const std::string& word, const std::string& newTranslation) {
    const auto it = tree_.find(word);
    if (it == tree_.end())
        throw NoFoundWord();

    auto& translations = it->second;
    const auto pos = std::lower_bound(translations.begin(), translations.end(), newTranslation);
    if (pos != translations.end() && *pos == newTranslation)
        throw DuplicateTranslation();

    translations.insert(pos, newTranslation);
    writeToFile();
}

void Dict::removeTranslation(const std::string& word, const std::string& translation) {
    const auto it = tree_.find(word);
    if (it == tree_.end())
        throw NoFoundWord();

    auto& translations = it->second;
    const auto pos = std::lower_bound(translations.begin(), translations.end(), translation);

    if (pos == translations.end() || *pos != translation)
        throw TranslationsNotFound();

    translations.erase(pos);

    if (translations.empty()) {
        tree_.erase(it);
        removeFromFile(word);
    } else
        removeFromFile(word, &translation);
}

const std::list<std::string>& Dict::findTranslationByWord(const std::string& word) const {
    const auto it = tree_.find(word);
    if (it == tree_.end())
        throw NoFoundWord();
    return it->second;
}

std::string Dict::findWordByTranslation(const std::string& translation) const {
    const auto it = std::find_if(tree_.begin(), tree_.end(),
                           [&translation](const auto& pair) {
                               return std::binary_search(pair.second.begin(), pair.second.end(), translation);
                           });
    if (it != tree_.end())
        return it->first;
    throw NoFoundWord();
}

void Dict::print() const {
    std::for_each(tree_.begin(), tree_.end(),
                  [](const auto& pair) {
                      std::cout << pair.first << " - ";
                      std::ostringstream oss;
                      std::copy(pair.second.begin(), pair.second.end(),
                                std::ostream_iterator<std::string>(oss, "; "));
                      std::string result = oss.str();
                      if (!result.empty())
                          result.resize(result.size() - 2);
                      std::cout << result << std::endl;
                  });
}

std::string Dict::autoTranslate(std::string& input) const {
    trimSpaces(input);

    if (isEnglishWord(input)) {
        toLowerCaseE(input);
        const auto& translations = findTranslationByWord(input);
        std::ostringstream oss;
        std::copy(translations.begin(), translations.end(),
                  std::ostream_iterator<std::string>(oss, "; "));
        std::string result = oss.str();
        if (!result.empty())
            result.resize(result.size() - 2);
        return result;
    }
    toLowerCaseR(input);
    return findWordByTranslation(input);
}

std::string Dict::getCommandInput() {
    std::cout << "\nВведите команду: ";
    std::string command;
    if (!std::getline(std::cin, command)) {
        std::cout << "\nЗавершение работы. Все данные сохранены в файле\n";
        throw Exit();
    }
    trimSpaces(command);
    toUpperCaseE(command);
    return command;
}

void Dict::printWelcome() {
    std::cout << "\n\t\tАнгло-русский словарь\n\n";
    std::cout << "\nДоступные команды:\n"
              << "1. INSERT - Добавить новое слово и его переводы в словарь.\n"
              << "2. REMOVE - Удалить слово из словаря.\n"
              << "3. ADDTRANSLATION - Добавить новый перевод для существующего слова.\n"
              << "4. REMOVETRANSLATION - Удалить перевод для существующего слова.\n"
              << "5. FINDTRANSLATION - Найти все переводы для слова.\n"
              << "6. FINDWORD - Найти слово по его переводу.\n"
              << "7. AUTOTRANSLATE - Автоматический перевод текста (определяется язык ввода).\n"
              << "8. PRINT - Вывести все слова и их переводы из словаря.\n\n"
              << "Для выхода используйте Ctrl+Z (Windows) или Ctrl+D (Linux)\n\n";
}

void Dict::run() {
    printWelcome();

    while (true) {
        try {
            const std::string command = getCommandInput();
            if (command.empty())
                continue;

            if (command == "INSERT") {
                std::string word;
                std::cout << "Введите английское слово: ";
                if (!std::getline(std::cin, word))
                    break;

                trimSpaces(word);
                if (!isEnglishWord(word))
                    throw InvalidWordLanguage();
                toLowerCaseE(word);

                std::string translations;
                std::cout << "Введите перевод(ы) через ';': ";
                if (!std::getline(std::cin, translations))
                    break;

                insert(word, translations);
                std::cout << "Добавлено: " << word << "\n";
            } else if (command == "REMOVE") {
                std::string word;
                std::cout << "Введите слово для удаления: ";
                if (!std::getline(std::cin, word))
                    break;

                trimSpaces(word);
                if (!isEnglishWord(word))
                    throw InvalidWordLanguage();
                toLowerCaseE(word);

                remove(word);
                std::cout << "Удалено успешно!\n";
            } else if (command == "ADDTRANSLATION") {
                std::string word;
                std::cout << "Введите слово: ";
                if (!std::getline(std::cin, word))
                    break;

                trimSpaces(word);
                toLowerCaseE(word);
                if (!isEnglishWord(word))
                    throw InvalidWordLanguage();

                std::string translation;
                std::cout << "Введите новый перевод: ";
                if (!std::getline(std::cin, translation))
                    break;

                trimSpaces(translation);
                if (!isRussianWord(translation))
                    throw InvalidTranslationLanguage();
                toLowerCaseR(translation);

                addTranslation(word, translation);
                std::cout << "Добавлено успешно!\n";
            } else if (command == "REMOVETRANSLATION") {
                std::string word;
                std::cout << "Введите слово: ";
                if (!std::getline(std::cin, word))
                    break;

                trimSpaces(word);
                if (!isEnglishWord(word))
                    throw InvalidWordLanguage();
                toLowerCaseE(word);

                std::string translation;
                std::cout << "Введите перевод для удаления: ";
                if (!std::getline(std::cin, translation))
                    break;

                trimSpaces(translation);
                if (!isRussianWord(translation))
                    throw InvalidTranslationLanguage();
                toLowerCaseR(translation);

                removeTranslation(word, translation);
                std::cout << (tree_.find(word) == tree_.end() ? "Перевод и слово удалены!\n" : "Перевод удалён!\n");
            } else if (command == "FINDTRANSLATION") {
                std::string word;
                std::cout << "Введите слово: ";
                if (!std::getline(std::cin, word))
                    break;

                trimSpaces(word);
                if (!isEnglishWord(word))
                    throw InvalidWordLanguage();
                toLowerCaseE(word);

                const auto& translations = findTranslationByWord(word);

                std::cout << "Переводы: ";
                if (!translations.empty()) {
                    std::copy(translations.begin(),
                             std::prev(translations.end()),
                             std::ostream_iterator<std::string>(std::cout, "; "));
                    std::cout << translations.back();
                }
                std::cout << "\n";
            } else if (command == "FINDWORD") {
                std::string translation;
                std::cout << "Введите перевод: ";
                if (!std::getline(std::cin, translation))
                    break;

                trimSpaces(translation);
                if (!isRussianWord(translation))
                    throw InvalidTranslationLanguage();
                toLowerCaseR(translation);

                std::cout << "Найдено слово: " << findWordByTranslation(translation) << "\n";
            } else if (command == "AUTOTRANSLATE") {
                std::string input;
                std::cout << "Введите текст: ";
                if (!std::getline(std::cin, input))
                    break;

                std::cout << "Результат: " << autoTranslate(input) << "\n";
            } else if (command == "PRINT") {
                print();
            } else {
                std::cout << "Неизвестная команда. Попробуйте снова.\n";
            }
        } catch (const std::logic_error & error) {
            std::cout << error.what() << "\n";
        } catch (const std::runtime_error & error) {
            std::cerr << error.what() << "\n";
            break;
        }
    }
}

bool Dict::isEnglishWord(const std::string& word) {
    return std::all_of(word.begin(), word.end(),
                       [](const unsigned char ch) {
                           return (ch >= 'A' && ch <= 'Z') ||
                                  (ch >= 'a' && ch <= 'z') ||
                                  ch == '\'' || ch == '-' ||
                                  ch == ',' || ch == ' ';
                       });
}

void Dict::toUpperCaseE(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](const unsigned char c) { return std::toupper(c); });
}

void Dict::toLowerCaseE(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](const unsigned char c) { return std::tolower(c); });
}

void Dict::trimSpaces(std::string& str) {
    const auto first =
        std::find_if_not(str.begin(), str.end(), [](const int c) { return std::isspace(c); });
    const auto last =
        std::find_if_not(str.rbegin(), str.rend(), [](const int c) { return std::isspace(c); }).base();
    str = first < last ? std::string(first, last) : "";
}

bool Dict::isRussianWord(const std::string& word) {
    if (word.empty())
        return false;

    std::function<bool(size_t)> checkRussian = [&](const size_t i) -> bool {
        if (i >= word.size())
            return true;
        const unsigned char ch = word[i];

        if (ch == ' ' || ch == ',')
            return checkRussian(i + 1);

        if ((ch == 0xD0 || ch == 0xD1) && i + 1 < word.size()) {
            const unsigned char next = word[i + 1];
            const bool isRussian = (ch == 0xD0 && next >= 0x90 && next <= 0xBF) || //А-Я, а-п
                             (ch == 0xD1 && next >= 0x80 && next <= 0x8F) || //р-я
                             (ch == 0xD0 && next == 0x81) || // Ё
                             (ch == 0xD1 && next == 0x91); //ё
            return isRussian ? checkRussian(i + 2) : false;
        }
        return false;
    };

    return checkRussian(0);
}

void Dict::toLowerCaseR(std::string& str) {
    std::function<void(size_t)> processRussian = [&](const size_t i) {
        if (i >= str.size())
            return;
        const unsigned char c = str[i];

        if ((c == 0xD0 || c == 0xD1) && i + 1 < str.size()) {
            const unsigned char next = str[i + 1];
            if (c == 0xD0) {
                if (next >= 0x90 && next <= 0x9F)
                    str[i + 1] = next + 0x20;
                else if (next == 0x81) {
                    str[i] = 0xD1;
                    str[i + 1] = 0x91;
                } else if (next >= 0xA0 && next <= 0xAF) {
                    str[i] = 0xD1;
                    str[i + 1] = next - 0x20;
                }
            }
            processRussian(i + 2);
        } else
            processRussian(i + 1);
    };

    processRussian(0);
}
