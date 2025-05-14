#include "../headers/Structs.h"
#include "../headers/Exceptions.h"

#include <string>
#include <iostream>

NodeList::NodeList(const std::string& value) {
    if (value.empty())
        throw EmptyValueInNode();

    try {
        value_ = value;
    } catch (const std::bad_alloc&) {
        throw FailOfMemoryAllocation("NodeList");
    }
    next_ = nullptr;
}

void List::clear() {
    while (head_ != nullptr) {
        NodeList* temp = head_;
        head_ = head_->next_;
        delete temp;
    }
    tail_ = nullptr;
    size_ = 0;
}

List::List() : head_(nullptr), tail_(nullptr), size_(0) {}

List::~List() {
    clear();
}

List& List::operator=(const List& other) {
    if (this != &other) {
        clear();

        const NodeList* current = other.head_;
        while (current) {
            push(current->value_);
            current = current->next_;
        }
    }
    return *this;
}

List::List(const List& other) : head_(nullptr), tail_(nullptr), size_(0) {
    const NodeList* current = other.head_;
    while (current != nullptr) {
        push(current->value_);
        current = current->next_;
    }
}

NodeList* List::getHead() const {
    return head_;
}

void List::push(const std::string& value) {
    if (value.empty()) {
        throw EmptyValue();
    }

    NodeList* newNode = nullptr;
    try {
        newNode = new NodeList(value);
    } catch (const std::bad_alloc&) {
        throw FailOfMemoryAllocation("NodeList");
    }

    if (head_ == nullptr)
        head_ = tail_ = newNode;
    else if (value < head_->value_) {
        newNode->next_ = head_;
        head_ = newNode;
    } else if (value > tail_->value_ && value > tail_->value_) {
        tail_->next_ = newNode;
        tail_ = newNode;
    } else {
        NodeList* current = head_;
        while (current->next_ != nullptr && current->next_->value_ < value) {
            if (current->value_ == value) {
                delete newNode;
                throw DuplicateTranslation();
            }
            current = current->next_;
        }
        if (current->value_ == value || (current->next_ != nullptr && current->next_->value_ == value)) {
            delete newNode;
            throw DuplicateTranslation();
        }
        newNode->next_ = current->next_;
        current->next_ = newNode;
        if (newNode->next_ == nullptr)
            tail_ = newNode;
    }

    ++size_;
}

void List::remove(NodeList* nodeToRemove) {
    if (nodeToRemove == nullptr || head_ == nullptr) {
        throw NullPointerDeletion();
    }

    NodeList* current = head_;
    NodeList* prev = nullptr;

    while (current != nullptr && current != nodeToRemove) {
        prev = current;
        current = current->next_;
    }

    if (current == nullptr) {
        throw NodeNotFoundInList();
    }

    if (prev == nullptr) {
        head_ = nodeToRemove->next_;
        if (head_ == nullptr) {
            tail_ = nullptr;
        }
    }
    else {
        prev->next_ = nodeToRemove->next_;
        if (nodeToRemove == tail_) {
            tail_ = prev;
        }
    }

    delete nodeToRemove;
    --size_;
}

std::string List::convertTranslationsToString() const {
    std::string result = "";
    NodeList* current = head_;

    while (current) {
        if (current->value_.empty()) {
            throw EmptyValueInNode();
        }

        result += current->value_;
        if (current->next_) {
            result += "; ";
        }
        current = current->next_;
    }

    return result;
}

bool List::isEmpty() const {
    return head_ == nullptr;
}

size_t List::getSize() const {
    return size_;
}

Pair::Pair() = default;

Pair::Pair(const std::string& eng, const List& rus) {
    try {
        word_ = eng;
        translation_ = rus;
    } catch (const std::bad_alloc&) {
        throw FailOfMemoryAllocation("Pair");
    }
}

bool Pair::operator<(const Pair& other) const {
    return word_ < other.word_;
}

bool Pair::operator>(const Pair& other) const {
    return word_ > other.word_;
}

bool Pair::operator==(const Pair& other) const {
    return word_ == other.word_;
}

std::ostream& operator<<(std::ostream& output, const Pair& pair) {
    if (pair.translation_.isEmpty()) {
        throw NoTranslations();
    }

    output << pair.word_ << " - " << pair.translation_.convertTranslationsToString();
    return output;
}