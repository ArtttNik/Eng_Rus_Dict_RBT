#ifndef MYJSON_H
#define MYJSON_H

#include "Exceptions.h"

#include <string>
#include <vector>
#include <list>
#include <map>

class MyJson {
public:
    static void parse(const std::string& text, std::map<std::string, std::list<std::string>>& dictionary);
    static std::string convertDictToJsonString(const std::map<std::string, std::list<std::string>>& dictionary);

private:
    static void skipWhitespace(const std::string& text, size_t& pos);
    static std::string parseString(const std::string& text, size_t& pos);
    static std::vector<std::string> parseArray(const std::string& text, size_t& pos);
};

#endif // MYJSON_H