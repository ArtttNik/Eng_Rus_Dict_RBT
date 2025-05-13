#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

struct NodeList {
    std::string value_;
    NodeList* next_;

    explicit NodeList(const std::string& value);
};

class List {
private:
    NodeList* head_;
    NodeList* tail_;
    size_t size_;

    void clear();

public:
    List();
    ~List();
    List& operator=(const List& other);
    List(const List& other);

    NodeList* getHead() const;
    void push(const std::string& value);

    void remove(NodeList* nodeToRemove);

    std::string convertTranslationsToString() const;
    bool isEmpty() const;
    size_t getSize() const;
};

struct Pair {
    std::string word_;
    List translation_;

    Pair();
    Pair(const std::string& eng, const List& rus);

    bool operator<(const Pair& other) const;
    bool operator>(const Pair& other) const;
    bool operator==(const Pair& other) const;
    friend std::ostream& operator<<(std::ostream& output, const Pair& pair);
};

#endif // STRUCTS_H
