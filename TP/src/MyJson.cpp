#include "../headers/MyJson.h"
#include <algorithm>
#include <sstream>
#include <iostream>

void MyJson::skipWhitespace(const std::string& text, size_t& pos) {
    if (pos >= text.size() || !isspace(text[pos]))
        return;
    ++pos;
    skipWhitespace(text, pos);
}

std::string MyJson::parseString(const std::string& text, size_t& pos) {
    if (pos >= text.size() || text[pos] != '"') {
        throw ErrorInFile();
    }

    std::string result;
    bool escape = false;
    const auto start = text.begin() + pos + 1;
    const auto end = text.end();

    const auto it = std::find_if(start, end, [&](const char c) {
        if (escape) {
            result += c;
            escape = false;
            return false;
        }
        if (c == '\\') {
            escape = true;
            return false;
        }
        if (c == '"') {
            return true;
        }
        result += c;
        return false;
    });

    if (it == end) {
        pos = text.size();
        throw ErrorInFile();
    }

    pos = std::distance(text.begin(), it) + 1;

    if (result.empty() && pos - (std::distance(text.begin(), it) == 1))
        throw ErrorInFile();

    return result;
}

std::vector<std::string> MyJson::parseArray(const std::string& text, size_t& pos) {
    std::vector<std::string> result;

    skipWhitespace(text, pos);
    if (pos >= text.size() || text[pos] != '[')
        return result;
    ++pos;

    auto parseNext = [&](auto& self) -> void {
        skipWhitespace(text, pos);
        if (pos >= text.size() || text[pos] == ']') {
            if (text[pos] == ']') ++pos;
            return;
        }

        std::string translation = parseString(text, pos);
        if (!translation.empty()) {
            result.push_back(std::move(translation));
        }

        skipWhitespace(text, pos);
        if (text[pos] == ',') {
            ++pos;
        }

        self(self);
    };

    parseNext(parseNext);
    return result;
}

void MyJson::parse(const std::string& text, std::map<std::string, std::list<std::string>>& dictionary) {
    size_t pos = 0;

    skipWhitespace(text, pos);
    if (text[pos] != '{')
        throw ErrorInFile();
    ++pos;

    auto parseNext = [&](auto& self) -> void {
        skipWhitespace(text, pos);
        if (pos >= text.size() || text[pos] == '}') {
            if (text[pos] == '}')
                ++pos;
            return;
        }

        std::string key = parseString(text, pos);
        if (key.empty())
            throw ErrorInFile();

        skipWhitespace(text, pos);
        if (text[pos] != ':')
            throw ErrorInFile();
        ++pos;

        std::vector<std::string> translations = parseArray(text, pos);
        if (!translations.empty()) {
            std::list<std::string> translationsList(translations.begin(), translations.end());
            translationsList.sort();
            translationsList.unique();
            dictionary[std::move(key)] = std::move(translationsList);
        }

        skipWhitespace(text, pos);
        if (text[pos] == ',')
            ++pos;

        self(self);
    };

    parseNext(parseNext);
}

std::string MyJson::convertToString(const std::map<std::string, std::list<std::string>>& dictionary) {
    std::ostringstream oss;
    oss << "{";

    auto comma = [first = true](std::ostringstream& osst) mutable {
        if (!first)
            osst << ",";
        first = false;
    };

    std::for_each(dictionary.begin(), dictionary.end(), [&](const auto& entry) {
        comma(oss);
        oss << "\n  \"" << entry.first << "\": [";

        auto printTr = [first_tr = true](std::ostringstream& oss, const auto& tr) mutable {
            if (!first_tr)
                oss << ",";
            first_tr = false;
            oss << "\"" << tr << "\"";
        };

        std::for_each(entry.second.begin(), entry.second.end(),
            [&](const auto& tr) { printTr(oss, tr); });

        oss << "]";
    });

    oss << "\n}";
    return oss.str();
}