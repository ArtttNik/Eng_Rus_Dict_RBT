#ifndef MYJSON_H
#define MYJSON_H

#include <string>

class MyJsonArrayIterator {
public:
    MyJsonArrayIterator(const std::string* data, size_t index);
    MyJsonArrayIterator& operator++();
    bool operator!=(const MyJsonArrayIterator& other) const;
    std::string getValue() const;

private:
    const std::string* data_;
    size_t index_;
};

class MyJsonArray {
public:
    MyJsonArray(const std::string* data, size_t size);
    MyJsonArrayIterator begin() const;
    MyJsonArrayIterator end() const;

private:
    const std::string* data_;
    size_t size_;
};

class MyJsonIterator;

class MyJson {
public:
    struct Entry {
        std::string key;
        std::string* translations;
        size_t translationsCount;

        Entry();
        ~Entry();

        Entry(const Entry& other);

        Entry& operator=(const Entry& other);
    };

    MyJson();
    ~MyJson();

    void parse(const std::string& text);
    size_t getSize() const;
    const Entry& getEntry(size_t index) const;

    MyJsonIterator begin() const;
    MyJsonIterator end() const;

private:
    static void skipWhitespace(const std::string& text, size_t& pos);
    static std::string parseString(const std::string& text, size_t& pos);
    void enlarge();

    Entry* entries_;
    size_t size_;
    size_t capacity_;
};

class MyJsonIterator {
public:
    MyJsonIterator(const MyJson* json, size_t index);
    MyJsonIterator& operator++();
    bool operator!=(const MyJsonIterator& other) const;
    std::string getKey() const;
    MyJsonArray getValue() const;

private:
    const MyJson* json_;
    size_t index_;
};

#endif // MYJSON_H
