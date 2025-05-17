#ifndef DICT_H
#define DICT_H

#include <string>
#include <list>
#include <map>

class Dict {
public:
    explicit Dict(const std::string& filename);
    ~Dict();

    std::string dataPath_;

    static void insertSorted(std::list<std::string>& list, const std::string & value);
    void insert(const std::string& word, const std::string& translationsStr);
    void addTranslation(const std::string& word, std::string& newTranslation);
    void removeTranslation(const std::string& word, const std::string& translation);
    const std::list<std::string>& findTranslationByWord(const std::string& word) const;
    std::string findWordByTranslation(const std::string& translation) const;
    void remove(const std::string& word);
    void print() const;
    std::string autoTranslate(std::string& input) const;

    static void printWelcome();
    void run();
private:
    std::map<std::string, std::list<std::string>> tree_;

    void loadFromFile(const std::string& filename);
    void writeToFile() const;
    void removeFromFile(const std::string& word, const std::string* translation = nullptr);
    static std::string getCommandInput();

    static bool isEnglishWord(const std::string& word);
    static bool isRussianWord(const std::string& word);
    static void toUpperCaseE(std::string& str);
    static void toLowerCaseE(std::string& str);
    static void toLowerCaseR(std::string& str);
    static void trimSpaces(std::string& str);
};

void runTests();

#endif // DICT_H