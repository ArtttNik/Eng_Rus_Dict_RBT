#ifndef RBT_H
#define RBT_H

#include <fstream>
#include <string>
#include <iostream>

#include "Structs.h"

inline std::string DATAPATH;

enum Color { RED, BLACK };

struct Node {
    Pair data_;
    Color color_;
    Node* left_;
    Node* right_;
    Node* parent_;

    Node(const std::string& word, const List& translation);
    explicit Node(const Pair& pair);
};

class RBT {
private:
    Node* root_ = nullptr;

    void loadFromFile(const std::string& filename);

    static void destroyTree(Node* node);

    void rotateLeft(Node* node);
    void rotateRight(Node* node);

    void fixInsert(Node*& current);
    void fixRemove(Node* current, Node* parentNode);
public:
    RBT();

    ~RBT();
    void clear();

    static Node* search(Node* node, const std::string& key);

    static Color getColor(const Node* node);
    static void setColor(Node* node, const Color& color);
    Node* getRoot() const;

    void insert(const Pair& value);

    void transplant(Node* nodeToReplace, Node* replacementNode);
    static Node* findMaximum(Node* node);
    void remove(const std::string& word);

    std::string makeTreeToString() const;
    static void makeTreeToStringRecursive(const Node* node, const std::string& prefix, bool isTail, std::string& result);
};

bool isEnglishWord(const std::string& word);
bool isRussianWord(const std::string& word);

#endif //RBT_H
