#include "../headers/Dict.h"
#include "../headers/Exceptions.h"
#include "../headers/MyJson.h"

void Dict::printRecursive(const Node* node) {
    if (!node)
        return;

    printRecursive(node->left_);
    std::cout << node->data_ << std::endl;
    printRecursive(node->right_);
}

void Dict::writeToFile(const Pair& value) {
    std::ifstream fin(DATAPATH);
    if (!fin) {
        std::ofstream fout(DATAPATH);
        fout << "{\n}\n";
        fout.close();
        fin.open(DATAPATH);
        if (!fin)
            throw FailOfMemoryAllocation("Dictionary.json file");
    }

    std::string content;
    char ch;
    while (fin.get(ch))
        content += ch;
    fin.close();

    MyJson jsonData;
    jsonData.parse(content);

    std::string newContent = "{";
    bool inserted = false;

    for (size_t i = 0; i < jsonData.getSize(); ++i) {
        const auto& entry = jsonData.getEntry(i);

        if (entry.key == value.word_) {
            if (i > 0)
                newContent += ",";
            newContent += "\n  \"" + value.word_ + "\": [";
            bool first = true;
            for (NodeList* current = value.translation_.getHead(); current; current = current->next_) {
                if (!first)
                    newContent += ",";
                newContent += "\"" + current->value_ + "\"";
                first = false;
            }
            newContent += "]";
            inserted = true;
        } else {
            if (i > 0 || inserted)
                newContent += ",";
            newContent += "\n  \"" + entry.key + "\": [";
            for (size_t j = 0; j < entry.translationsCount; ++j) {
                if (j > 0)
                    newContent += ",";
                newContent += "\"" + entry.translations[j] + "\"";
            }
            newContent += "]";
        }
    }

    if (!inserted) {
        if (jsonData.getSize() > 0)
            newContent += ",";

        newContent += "\n  \"" + value.word_ + "\": [";
        bool first = true;
        for (NodeList* current = value.translation_.getHead(); current; current = current->next_) {
            if (!first)
                newContent += ",";

            newContent += "\"" + current->value_ + "\"";
            first = false;
        }
        newContent += "]";
    }

    newContent += "\n}\n";

    std::ofstream fout(DATAPATH);
    if (!fout)
        throw FailOfMemoryAllocation("Dictionary.json file");

    fout << newContent;
    fout.close();
}

void Dict::removeFromFile(const std::string& word) {
    std::ifstream fin(DATAPATH);
    if (!fin)
        throw NodeNotFound();

    std::string content;
    char ch;
    while (fin.get(ch))
        content += ch;
    fin.close();

    MyJson jsonData;
    jsonData.parse(content);

    bool found = false;
    size_t indexToRemove = 0;
    for (size_t i = 0; i < jsonData.getSize(); ++i) {
        if (jsonData.getEntry(i).key == word) {
            found = true;
            indexToRemove = i;
            break;
        }
    }

    if (!found)
        throw NodeNotFound();

    std::string newContent = "{";
    bool firstEntry = true;

    for (size_t i = 0; i < jsonData.getSize(); ++i) {
        if (i == indexToRemove)
            continue;

        if (!firstEntry)
            newContent += ",";
        firstEntry = false;

        const auto& entry = jsonData.getEntry(i);
        newContent += "\n  \"" + entry.key + "\": [";

        for (size_t j = 0; j < entry.translationsCount; ++j) {
            if (j > 0)
                newContent += ",";
            newContent += "\"" + entry.translations[j] + "\"";
        }
        newContent += "]";
    }

    newContent += "\n}\n";

    std::ofstream fout(DATAPATH);
    if (!fout)
        throw FailOfMemoryAllocation("Dictionary.json file");
    fout << newContent;
    fout.close();
}

Dict::Dict() = default;

Dict::~Dict() {
    tree_.clear();
}

RBT Dict::getTree() {
    return tree_;
}

void Dict::insert(std::string& word, std::string& translationsStr) {
    trimSpaces(word);
    if (word.empty() || !isEnglishWord(word))
        throw InvalidWordLanguage();

    toLowerCaseE(word);

    if (translationsStr.empty())
        throw EmptyValue();

    List listTranslations;
    size_t start = 0;

    for (size_t end = 0; end <= translationsStr.size(); ++end) {
        if (end == translationsStr.size() || translationsStr[end] == ';') {
            std::string translation = translationsStr.substr(start, end - start);
            trimSpaces(translation);

            if (!translation.empty()) {
                if (!isRussianWord(translation))
                    throw InvalidTranslationLanguage();

                toLowerCaseR(translation);
                listTranslations.push(translation);
            }
            start = end + 1;
        }
    }

    if (listTranslations.isEmpty())
        throw EmptyValue();

    tree_.insert(Pair(word, listTranslations));
    writeToFile(Pair(word, listTranslations));
}

void Dict::addTranslation(const std::string& word, const std::string& newTranslation) {
    Node* node = tree_.search(tree_.getRoot(), word);
    if (!node)
        throw NodeNotFound();

    const NodeList* current = node->data_.translation_.getHead();
    while (current != nullptr) {
        if (current->value_ == newTranslation)
            throw DuplicateTranslation();
        current = current->next_;
    }

    node->data_.translation_.push(newTranslation);
    writeToFile(node->data_);
}

void Dict::removeTranslation(const std::string& word, const std::string& translation) {
    Node* node = tree_.search(tree_.getRoot(), word);
    if (!node)
        throw NodeNotFound();

    NodeList* current = node->data_.translation_.getHead();
    while (current && current->value_ != translation)
        current = current->next_;

    if (!current)
        throw NodeNotFoundInList();

    node->data_.translation_.remove(current);

    if (node->data_.translation_.isEmpty()) {
        tree_.remove(word);
        removeFromFile(word);
    } else {
        writeToFile(node->data_);
    }
}

List& Dict::findTranslationByWord(const std::string& word) const {
    Node* node = tree_.search(tree_.getRoot(), word);

    if (node == nullptr)
        throw NoFoundWord();

    return node->data_.translation_;
}

std::string Dict::findWordByTranslation(const std::string& translation) const {
    Node* result = findInTreeByTranslation(tree_.getRoot(), translation);
    if (!result)
        throw NodeNotFoundInList();

    return result->data_.word_;
}

Node* Dict::findInTreeByTranslation(Node* node, const std::string& translation) {
    if (!node)
        return nullptr;

    Node* leftResult = findInTreeByTranslation(node->left_, translation);
    if (leftResult)
        return leftResult;

    const NodeList* current = node->data_.translation_.getHead();
    while (current) {
        if (current->value_ == translation)
            return node;
        current = current->next_;
    }

    return findInTreeByTranslation(node->right_, translation);
}

std::string Dict::autotranslate(std::string& input) const {
    trimSpaces(input);

    if (isEnglishWord(input)) {
        toLowerCaseE(input);
        return findTranslationByWord(input).convertTranslationsToString();
    }
    toLowerCaseR(input);
    return findWordByTranslation(input);
}


void Dict::remove(const std::string& word) {
    tree_.remove(word);
    removeFromFile(word);
}

void Dict::print() const {
    printRecursive(tree_.getRoot());
}

void Dict::run() {
    std::cout << "Выбрана пользовательская работа со словарём\n";
    std::cout << "\nДоступные команды:\n"
                    << "1. INSERT - Добавить новое слово и его переводы в словарь.\n"
                    << "2. REMOVE - Удалить слово из словаря.\n"
                    << "3. ADDTRANSLATION - Добавить новый перевод для существующего слова.\n"
                    << "4. REMOVETRANSLATION - Удалить перевод для существующего слова.\n"
                    << "5. FINDTRANSLATION - Найти все переводы для слова.\n"
                    << "6. FINDWORD - Найти слово по его переводу.\n"
                    << "7. AUTOTRANSLATE - Автоматический перевод текста (определяется язык ввода).\n"
                    << "8. PRINT - Вывести все слова и их переводы из словаря.\n"
                    << "9. TREE - Печать дерева словаря в текстовом виде.\n"
                    << "10. EXIT - Завершить работу программы.\n";
    while (true) {
        std::cout << "Введите команду: ";
        try {
            std::string command;
            std::getline(std::cin, command);
            trimSpaces(command);
            toUpperCaseE(command);

            if (command == "INSERT") {
                std::string word;
                std::cout << "Введите английское слово: ";
                std::getline(std::cin, word);

                std::string translations;
                std::cout << "Введите перевод(ы), разделяя их точкой с запятой ';': ";
                std::getline(std::cin, translations);

                insert(word, translations);
                std::cout << "Добавлено: " << word << "\n";
            } else if (command == "REMOVE") {
                std::string word;
                std::cout << "Введите слово для удаления: ";
                std::getline(std::cin, word);
                trimSpaces(word);
                toLowerCaseE(word);

                if (!isEnglishWord(word))
                    throw InvalidWordLanguage();

                remove(word);
                std::cout << "Удалено успешно!\n";
            } else if (command == "ADDTRANSLATION") {
                std::string word;
                std::cout << "Введите слово для добавления перевода: ";
                std::getline(std::cin, word);
                trimSpaces(word);
                toLowerCaseE(word);

                if (!isEnglishWord(word))
                    throw InvalidWordLanguage();

                if (!tree_.search(tree_.getRoot(), word))
                    throw NodeNotFound();

                std::string translation;
                std::cout << "Введите новый перевод: ";
                std::getline(std::cin, translation);
                trimSpaces(translation);
                toLowerCaseR(translation);

                if (!isRussianWord(translation))
                    throw InvalidTranslationLanguage();

                addTranslation(word, translation);
                std::cout << "Добавлено успешно!\n";
            } else if (command == "REMOVETRANSLATION") {
                std::string word;
                std::cout << "Введите слово для удаления перевода: ";
                std::getline(std::cin, word);
                trimSpaces(word);
                toLowerCaseE(word);

                if (!isEnglishWord(word))
                    throw InvalidWordLanguage();

                if (!tree_.search(tree_.getRoot(), word))
                    throw NodeNotFound();

                std::string translation;
                std::cout << "Введите перевод для удаления: ";
                std::getline(std::cin, translation);
                trimSpaces(translation);
                toLowerCaseR(translation);

                if (!isRussianWord(translation)) throw InvalidTranslationLanguage();

                removeTranslation(word, translation);

                if (!tree_.search(tree_.getRoot(), word)) {
                    std::cout << "Перевод удален, и слово полностью удалено из словаря!\n";
                } else {
                    std::cout << "Перевод удален!\n";
                }
            } else if (command == "FINDTRANSLATION") {
                std::string word;
                std::cout << "Введите слово для поиска переводов: ";
                std::getline(std::cin, word);
                trimSpaces(word);
                toLowerCaseE(word);

                if (!isEnglishWord(word)) throw InvalidWordLanguage();

                Node* node = tree_.search(tree_.getRoot(), word);
                if (!node)
                    throw NoFoundWord();

                std::cout << "Найдено: " << node->data_ << "\n";
            } else if (command == "FINDWORD") {
                std::string translation;
                std::cout << "Введите перевод для поиска слова: ";
                std::getline(std::cin, translation);
                trimSpaces(translation);
                toLowerCaseR(translation);

                if (!isRussianWord(translation)) throw InvalidTranslationLanguage();

                std::string word = findWordByTranslation(translation);
                if (!word.empty()) {
                    std::cout << "Найдено слово: " << word << '\n';
                } else {
                    throw NoFoundWord();
                }
            } else if (command == "PRINT") {
                print();
            } else if (command == "TREE") {
                std::cout << tree_.makeTreeToString() << "\n";
            } else if (command == "EXIT") {
                std::cout << "Выход из программы.\n";
                throw ProgramExit();
            } else if (command == "AUTOTRANSLATE") {
                std::string input;
                std::cout << "Введите текст для автоперевода (язык определяется автоматически): ";
                std::getline(std::cin, input);

                std::cout << "Результат перевода: " << autotranslate(input) << "\n";
            } else {
                std::cout << "Неизвестная команда. Попробуйте снова.\n";
            }
        } catch (const std::logic_error& error) {
            std::cout << error.what() << "\nПопробуйте снова.\n\n";
        } catch (const std::runtime_error& error) {
            std::cerr << error.what();
            std::cout << "Ошибка критическая, завершение работы!!!\n";
            exit(EXIT_FAILURE);
        }
    }
}

void toUpperCaseE(std::string& str) {
    for (char& c : str)
        if (c >= 'a' && c <= 'z')
            c = c - ('a' - 'A');
}

void trimSpaces(std::string& str) {
    if (str.empty()) return;

    size_t start = 0;
    while (start < str.size() && str[start] == ' ')
        ++start;

    if (start == str.size()) {
        str.clear();
        return;
    }

    size_t end = str.size() - 1;
    while (end > start && str[end] == ' ')
        --end;

    str = str.substr(start, end - start + 1);
}


void toLowerCaseE(std::string& str) {
    for (char& c: str) {
        if (c >= 'A' && c <= 'Z')
            c = c + ('a' - 'A');
    }
}

void toLowerCaseR(std::string& str) {
    for (size_t i = 0; i < str.size();) {
        unsigned char c = str[i];

        if ((c == 0xD0 || c == 0xD1) && i + 1 < str.size()) {
            const unsigned char next = str[i + 1];

            if (c == 0xD0) {
                if (next >= 0x90 && next <= 0x9F) {
                    str[i + 1] = next + 0x20;
                } else if (next == 0x81) {
                    str[i] = 0xD1;
                    str[i + 1] = 0x91;
                } else if (next >= 0xA0 && next <= 0xAF) {
                    str[i] = 0xD1;
                    str[i + 1] = next - 0x20;
                }
            }
            i += 2;
        } else {
            i++;
        }
    }
}
