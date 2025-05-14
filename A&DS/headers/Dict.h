#ifndef DICT_H
#define DICT_H

#include "RBT.h"
#include "Structs.h"

#include <string>

class Dict {
private:
    RBT tree_;

    static void printRecursive(const Node* node);

    static void writeToFile(const Pair& value);
    static void removeFromFile(const std::string& word);

public:
    Dict();
    ~Dict();

    RBT getTree();

    void insert(std::string& word, std::string& translationsStr);
    void remove(const std::string& word);
    void addTranslation(const std::string& word, const std::string& newTranslation);
    void removeTranslation(const std::string& word, const std::string& translation);
    List& findTranslationByWord(const std::string& word) const;
    static void printWordWithTranslations(const std::string& word, const List& translations);
    std::string findWordByTranslation(const std::string& translation) const;
    static Node* findInTreeByTranslation(Node* node, const std::string& translation);
    std::string autotranslate(std::string& input) const;
    void print() const;

    void run();
};

void toUpperCaseE(std::string& str);
void trimSpaces(std::string& str);
void toLowerCaseE(std::string& str);
void toLowerCaseR(std::string& str);

#endif //DICT_H
