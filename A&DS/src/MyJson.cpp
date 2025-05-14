#include "../headers/MyJson.h"

void MyJson::skipWhitespace(const std::string& text, size_t& pos) {
    while (pos < text.size() && (text[pos] == ' ' || text[pos]
        == '\n' || text[pos] == '\t' || text[pos] == '\r')) {
        ++pos;
    }
}

std::string MyJson::parseString(const std::string& text, size_t& pos) {
    if (text[pos] != '"')
        return "";
    ++pos;
    std::string result;
    while (pos < text.size() && text[pos] != '"')
        result += text[pos++];

    if (pos < text.size() && text[pos] == '"')
        ++pos;
    return result;
}

MyJsonArrayIterator::MyJsonArrayIterator(const std::string* data, size_t index)
    : data_(data), index_(index) {}

MyJsonArrayIterator& MyJsonArrayIterator::operator++() {
    ++index_;
    return *this;
}

bool MyJsonArrayIterator::operator!=(const MyJsonArrayIterator& other) const {
    return index_ != other.index_;
}

std::string MyJsonArrayIterator::getValue() const {
    return data_[index_];
}

MyJsonArray::MyJsonArray(const std::string* data, size_t size)
    : data_(data), size_(size) {}

MyJson::Entry::Entry() {
    translations = new std::string[32];
    translationsCount = 0;
}

MyJson::Entry::~Entry() {
    delete[] translations;
}

MyJson::Entry::Entry(const Entry& other) {
    key = other.key;
    translationsCount = other.translationsCount;
    translations = new std::string[32];
    for (size_t i = 0; i < translationsCount; ++i)
        translations[i] = other.translations[i];
}

MyJson::Entry& MyJson::Entry::operator=(const Entry& other) {
    if (this != &other) {
        delete[] translations;
        key = other.key;
        translationsCount = other.translationsCount;
        translations = new std::string[32];
        for (size_t i = 0; i < translationsCount; ++i)
            translations[i] = other.translations[i];
    }
    return *this;
}

MyJson::MyJson() : size_(0), capacity_(4) {
    entries_ = new Entry[capacity_];
}

MyJson::~MyJson() {
    delete[] entries_;
}

void MyJson::enlarge() {
    capacity_ *= 2;
    auto* newEntries = new Entry[capacity_];
    for (size_t i = 0; i < size_; ++i)
        newEntries[i] = entries_[i];

    delete[] entries_;
    entries_ = newEntries;
}

void MyJson::parse(const std::string& text) {
    size_ = 0;
    size_t pos = 0;
    skipWhitespace(text, pos);
    if (text[pos] != '{')
        return;
    ++pos;

    while (pos < text.size()) {
        skipWhitespace(text, pos);
        if (text[pos] == '}')
            break;

        std::string key = parseString(text, pos);
        skipWhitespace(text, pos);
        if (text[pos] != ':')
            return;
        ++pos;
        skipWhitespace(text, pos);
        if (text[pos] != '[')
            return;
        ++pos;

        if (size_ >= capacity_)
            enlarge();

        entries_[size_].key = key;
        entries_[size_].translationsCount = 0;

        while (pos < text.size()) {
            skipWhitespace(text, pos);
            if (text[pos] == ']') {
                ++pos;
                break;
            }

            std::string translation = parseString(text, pos);
            if (entries_[size_].translationsCount < 32)
                entries_[size_].translations[entries_[size_].translationsCount++] = translation;

            skipWhitespace(text, pos);
            if (text[pos] == ',')
                ++pos;
        }

        skipWhitespace(text, pos);
        if (text[pos] == ',')
            ++pos;
        ++size_;
    }
}

const MyJson::Entry& MyJson::getEntry(size_t index) const {
    return entries_[index];
}

size_t MyJson::getSize() const {
    return size_;
}

MyJsonIterator::MyJsonIterator(const MyJson* json, size_t index)
    : json_(json), index_(index) {}

MyJsonIterator& MyJsonIterator::operator++() {
    ++index_;
    return *this;
}

bool MyJsonIterator::operator!=(const MyJsonIterator& other) const {
    return index_ != other.index_;
}

std::string MyJsonIterator::getKey() const {
    return json_->getEntry(index_).key;
}

MyJsonArray MyJsonIterator::getValue() const {
    const auto& entry = json_->getEntry(index_);
    return MyJsonArray(entry.translations, entry.translationsCount);
}

MyJsonArrayIterator MyJsonArray::begin() const {
    return {data_, 0};
}

MyJsonArrayIterator MyJsonArray::end() const {
    return {data_, size_};
}

MyJsonIterator MyJson::begin() const {
    return {this, 0};
}

MyJsonIterator MyJson::end() const {
    return {this, size_};
}