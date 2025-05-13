#include "../headers/MyJson.h"
#include <algorithm>
#include <sstream>

void MyJson::skipWhitespace(const std::string& text, size_t& pos) {
    while (pos < text.size() && isspace(text[pos])) {
        ++pos;
    }
}

std::string MyJson::parseString(const std::string& text, size_t& pos) {
    if (text[pos] != '"') return "";

    std::string result;
    bool escape = false;

    for (++pos; pos < text.size(); ++pos) {
        if (escape) {
            escape = false;
            result += text[pos];
        } else if (text[pos] == '\\') {
            escape = true;
        } else if (text[pos] == '"') {
            ++pos;
            break;
        } else {
            result += text[pos];
        }
    }

    return result;
}

MyJson::Translations MyJson::parseArray(const std::string& text, size_t& pos) {
    Translations result;

    skipWhitespace(text, pos);
    if (text[pos] != '[') return result;
    ++pos;

    while (pos < text.size()) {
        skipWhitespace(text, pos);
        if (text[pos] == ']') {
            ++pos;
            break;
        }

        std::string translation = parseString(text, pos);
        if (!translation.empty()) {
            result.push_back(std::move(translation));
        }

        skipWhitespace(text, pos);
        if (text[pos] == ',') {
            ++pos;
        }
    }

    return result;
}

void MyJson::parse(const std::string& text, std::map<std::string, std::list<std::string>>& dictionary) {
    size_t pos = 0;

    skipWhitespace(text, pos);
    if (text[pos] != '{') return;
    ++pos;

    while (pos < text.size()) {
        skipWhitespace(text, pos);
        if (text[pos] == '}') {
            ++pos;
            break;
        }

        std::string key = parseString(text, pos);
        if (key.empty()) break;

        skipWhitespace(text, pos);
        if (text[pos] != ':') break;
        ++pos;

        Translations translations = parseArray(text, pos);
        if (!translations.empty()) {
            std::list<std::string> translationsList(translations.begin(), translations.end());
            translationsList.sort();
            translationsList.unique();
            dictionary[std::move(key)] = std::move(translationsList);
        }

        skipWhitespace(text, pos);
        if (text[pos] == ',') {
            ++pos;
        }
    }
}

std::string MyJson::convertToString(const std::map<std::string, std::list<std::string>>& dictionary) {
    std::ostringstream oss;
    oss << "{";

    bool first = true;
    for (const auto & it : dictionary) {
        if (!first) oss << ",";
        first = false;

        oss << "\n  \"" << it.first << "\": [";
        bool firstTr = true;
        for (const auto& tr : it.second) {
            if (!firstTr) oss << ",";
            firstTr = false;
            oss << "\"" << tr << "\"";
        }
        oss << "]";
    }

    oss << "\n}";
    return oss.str();
}