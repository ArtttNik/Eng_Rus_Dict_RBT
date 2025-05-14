#include "../headers/RBT.h"
#include "../headers/Exceptions.h"
#include "../headers/MyJson.h"

Node::Node(const std::string& word, const List& translation) {
    if (word.empty()|| translation.getSize() == 0)
        throw EmptyValueInNode();

    try {
        this->data_ = Pair(word, translation);
    } catch (const std::bad_alloc&) {
        throw FailOfMemoryAllocation("NodeRBT");
    }
    this->color_ = RED;
    this->left_ = nullptr;
    this->right_ = nullptr;
    this->parent_ = nullptr;
}

Node::Node(const Pair& pair) {
    if (pair.word_.empty() || pair.translation_.getSize() == 0)
        throw EmptyValueInNode();

    try {
        this->data_ = pair;
    } catch (const std::bad_alloc&) {
        throw FailOfMemoryAllocation("NodeRBT");
    }
    this->color_ = RED;
    this->left_ = nullptr;
    this->right_ = nullptr;
    this->parent_ = nullptr;
}

void RBT::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        return;

    std::string content;
    std::string line;
    while (std::getline(file, line))
        content += line + '\n';

    file.close();

    size_t i = 0;
    while (i < content.size() && (content[i] == ' ' || content[i] == '\t'
        || content[i] == '\n' || content[i] == '\r'))
        ++i;


    if (i >= content.size() || content[i] != '{') {
        std::ofstream fout(filename);
        fout << "{\n}\n";
        fout.close();
        return;
    }

    MyJson dict;
    dict.parse(content);

    for (auto it = dict.begin(); it != dict.end(); ++it) {
        const std::string& word = it.getKey();
        MyJsonArray translationsArray = it.getValue();

        List translations;
        for (auto tr = translationsArray.begin(); tr != translationsArray.end(); ++tr)
            translations.push(tr.getValue());

        insert(Pair(word, translations));
    }
}

void RBT::destroyTree(Node* node) {
    if (node != nullptr) {
        destroyTree(node->left_);
        destroyTree(node->right_);
        delete node;
    }
}

Color RBT::getColor(const Node* node) {
    if (node != nullptr)
        return node->color_;
    return BLACK;
}

void RBT::setColor(Node* node, const Color& color) {
    if (color != RED && color != BLACK)
        throw WrongColor();

    if (node != nullptr)
        node->color_ = color;
}

Node* RBT::getRoot() const {
    return root_;
}

void RBT::rotateLeft(Node* node) {
    if (node == nullptr)
        throw RotationErrorNullptr("Left");

    if (node->right_ == nullptr)
        throw LeftRotationError();

    Node* rightChild = node->right_;

    node->right_ = rightChild->left_;
    if (rightChild->left_ != nullptr)
        rightChild->left_->parent_ = node;

    rightChild->parent_ = node->parent_;
    if (node->parent_ == nullptr)
        root_ = rightChild;
    else if (node == node->parent_->left_)
        node->parent_->left_ = rightChild;
    else
        node->parent_->right_ = rightChild;

    rightChild->left_ = node;
    node->parent_ = rightChild;
}

void RBT::rotateRight(Node* node) {
    if (node == nullptr)
        throw RotationErrorNullptr("Right");

    if (node->left_ == nullptr)
        throw RightRotationError();

    Node* leftChild = node->left_;

    node->left_ = leftChild->right_;
    if (leftChild->right_ != nullptr)
        leftChild->right_->parent_ = node;

    leftChild->parent_ = node->parent_;
    if (node->parent_ == nullptr)
        root_ = leftChild;
    else if (node == node->parent_->left_)
        node->parent_->left_ = leftChild;
    else
        node->parent_->right_ = leftChild;

    leftChild->right_ = node;
    node->parent_ = leftChild;
}

void RBT::fixInsert(Node*& current) {
    if (current == nullptr)
        throw NullInsertionNode();

    while (current != root_ && getColor(current) == RED &&
           current->parent_ && getColor(current->parent_) == RED) {
        Node* parent = current->parent_;
        Node* grandparent = parent->parent_;

        if (grandparent == nullptr)
            throw MissingGrandparent();

        const bool isParentLeftChild = parent == grandparent->left_;
        Node* uncle = isParentLeftChild ? grandparent->right_ : grandparent->left_;

        if (getColor(uncle) == RED) {
            setColor(parent, BLACK);
            setColor(uncle, BLACK);
            setColor(grandparent, RED);
            current = grandparent;
        } else {
            if (isParentLeftChild && current == parent->right_) {
                rotateLeft(parent);
                current = parent;
            } else if (!isParentLeftChild && current == parent->left_) {
                rotateRight(parent);
                current = parent;
            }

            setColor(current->parent_, BLACK);
            setColor(grandparent, RED);
            if (isParentLeftChild)
                rotateRight(grandparent);
            else
                rotateLeft(grandparent);
        }
    }

    if (root_ != nullptr)
        root_->color_ = BLACK;
}

void RBT::fixRemove(Node* current, Node* parentNode) {
    while (current != root_ && getColor(current) == BLACK) {
        if (parentNode == nullptr)
            break;

        const bool isLeftChild = current == parentNode->left_ || current == nullptr && parentNode->left_ == nullptr;
        Node* sibling = isLeftChild ? parentNode->right_ : parentNode->left_;

        if (sibling == nullptr)
            break;

        if (getColor(sibling) == RED) {
            setColor(sibling, BLACK);
            setColor(parentNode, RED);
            if (isLeftChild)
                rotateLeft(parentNode);
            else
                rotateRight(parentNode);
            sibling = isLeftChild ? parentNode->right_ : parentNode->left_;

            if (sibling == nullptr)
                break;
        }

        const bool leftBlack = sibling->left_ == nullptr || getColor(sibling->left_) == BLACK;
        const bool rightBlack = sibling->right_ == nullptr || getColor(sibling->right_) == BLACK;

        if (leftBlack && rightBlack) {
            setColor(sibling, RED);
            current = parentNode;
            parentNode = current->parent_;
        } else {
            if (isLeftChild) {
                if (sibling->right_ == nullptr || getColor(sibling->right_) == BLACK) {
                    if (sibling->left_ != nullptr)
                        setColor(sibling->left_, BLACK);
                    setColor(sibling, RED);
                    rotateRight(sibling);
                    sibling = parentNode->right_;
                }
                setColor(sibling, getColor(parentNode));
                setColor(parentNode, BLACK);
                if (sibling->right_ != nullptr)
                    setColor(sibling->right_, BLACK);
                rotateLeft(parentNode);
            } else {
                if (sibling->left_ == nullptr || getColor(sibling->left_) == BLACK) {
                    if (sibling->right_ != nullptr)
                        setColor(sibling->right_, BLACK);
                    setColor(sibling, RED);
                    rotateLeft(sibling);
                    sibling = parentNode->left_;
                }
                setColor(sibling, getColor(parentNode));
                setColor(parentNode, BLACK);
                if (sibling->left_ != nullptr)
                    setColor(sibling->left_, BLACK);
                rotateRight(parentNode);
            }
            current = root_;
            break;
        }
    }

    if (current != nullptr)
        setColor(current, BLACK);
}

Node* RBT::search(Node* node, const std::string& key) {
    while (node != nullptr) {
        if (key == node->data_.word_)
            return node;

        if (key < node->data_.word_)
            node = node->left_;
        else
            node = node->right_;
    }
    return nullptr;
}

RBT::RBT() {
    root_ = nullptr;
    loadFromFile(DATAPATH);
}

RBT::~RBT() {
    destroyTree(root_);
    root_ = nullptr;
}

void RBT::clear() {
    destroyTree(root_);
    root_ = nullptr;
}

void RBT::insert(const Pair& value) {
    if (search(root_, value.word_) != nullptr)
        throw DuplicateWord();

    Node** insertionPoint = &root_;
    Node* parent = nullptr;

    while (*insertionPoint != nullptr) {
        parent = *insertionPoint;
        if (value < (*insertionPoint)->data_)
            insertionPoint = &(*insertionPoint)->left_;
        else
            insertionPoint = &(*insertionPoint)->right_;
    }

    Node* newNode;
    try {
        newNode = new Node(value);
    } catch (const std::bad_alloc &) {
        throw FailOfMemoryAllocation("Node");
    }

    newNode->parent_ = parent;
    *insertionPoint = newNode;

    fixInsert(newNode);
}


void RBT::transplant(Node* nodeToReplace, Node* replacementNode) {
    if (nodeToReplace->parent_ == nullptr)
        root_ = replacementNode;
    else if (nodeToReplace == nodeToReplace->parent_->left_)
        nodeToReplace->parent_->left_ = replacementNode;
    else
        nodeToReplace->parent_->right_ = replacementNode;

    if (replacementNode != nullptr)
        replacementNode->parent_ = nodeToReplace->parent_;
}

Node *RBT::findMaximum(Node* node) {
    while (node->right_ != nullptr)
        node = node->right_;
    return node;
}

void RBT::remove(const std::string& word) {
    Node* nodeToDelete = search(root_, word);
    if (nodeToDelete == nullptr)
        return;

    Node* replacementNode = nodeToDelete;
    Node* replacementChild = nullptr;
    Node* replacementChildParent = nullptr;
    Color originalColor = replacementNode->color_;

    if (nodeToDelete->left_ == nullptr) {
        replacementChild = nodeToDelete->right_;
        replacementChildParent = nodeToDelete->parent_;
        transplant(nodeToDelete, nodeToDelete->right_);
    } else if (nodeToDelete->right_ == nullptr) {
        replacementChild = nodeToDelete->left_;
        replacementChildParent = nodeToDelete->parent_;
        transplant(nodeToDelete, nodeToDelete->left_);
    } else {
        replacementNode = findMaximum(nodeToDelete->left_);
        originalColor = replacementNode->color_;
        replacementChild = replacementNode->left_;
        replacementChildParent = replacementNode;

        if (replacementNode->parent_ != nodeToDelete) {
            transplant(replacementNode, replacementNode->left_);
            replacementNode->left_ = nodeToDelete->left_;
            replacementNode->left_->parent_ = replacementNode;
        }


        transplant(nodeToDelete, replacementNode);
        replacementNode->right_ = nodeToDelete->right_;
        replacementNode->right_->parent_ = replacementNode;
        replacementNode->color_ = nodeToDelete->color_;
    }

    delete nodeToDelete;

    if (originalColor == BLACK)
        fixRemove(replacementChild, replacementChildParent);
}

std::string RBT::makeTreeToString() const {
    std::string result;
    makeTreeToStringRecursive(root_, "", true, result);
    return result;
}

void RBT::makeTreeToStringRecursive(const Node* node, const std::string& prefix, const bool isTail,
                                    std::string& result) {
    if (node == nullptr)
        return;

    result += prefix + (isTail ? "└── " : "├── ");

    result += (node->color_ == RED ? "[R] " : "[B] ") +
            node->data_.word_ + ": " +
            node->data_.translation_.convertTranslationsToString() + "\n";

    const std::string newPrefix = prefix + (isTail ? "    " : "│   ");

    if (node->left_ || node->right_) {
        if (node->left_)
            makeTreeToStringRecursive(node->left_, newPrefix, false, result);
        if (node->right_)
            makeTreeToStringRecursive(node->right_, newPrefix, true, result);
    }
}
