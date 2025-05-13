#ifndef MYJSON_H
#define MYJSON_H

#include <string>
#include <vector>
#include <list>
#include <map>

class MyJson {
public:
    using Translations = std::vector<std::string>;
    static void parse(const std::string& text, std::map<std::string, std::list<std::string>>& dictionary);
    static std::string convertToString(const std::map<std::string, std::list<std::string>>& dictionary);

private:
    static void skipWhitespace(const std::string& text, size_t& pos);
    static std::string parseString(const std::string& text, size_t& pos);
    static Translations parseArray(const std::string& text, size_t& pos);
};

#endif // MYJSON_H