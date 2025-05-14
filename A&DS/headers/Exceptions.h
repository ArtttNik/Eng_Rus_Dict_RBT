#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

class NullPointerDeletion final : public std::logic_error {
public:
    NullPointerDeletion() : std::logic_error("WARNING: List Attempted to delete null pointer\n") {
    }
};

class FailOfMemoryAllocation final : public std::runtime_error {
public:
    explicit FailOfMemoryAllocation(const std::string & objectType)
        : std::runtime_error("ERROR: Memory allocation failed for " + objectType + "\n") {
    }
};

class FailOfObjectDeletion final : public std::runtime_error {
public:
    explicit FailOfObjectDeletion(const std::string & objectType)
        : std::runtime_error("ERROR: Deletion failed for object of type: " + objectType + "\n") {
    }
};

class DuplicateTranslation final : public std::logic_error {
public:
    explicit DuplicateTranslation()
        : std::logic_error("WARNING: Translation already exists!\n") {
    }
};

class NoTranslations final : public std::logic_error {
public:
    explicit NoTranslations()
        : std::logic_error("WARNING: There isn`t any translations!\n") {
    }
};

class DuplicateWord final : public std::logic_error {
public:
    explicit DuplicateWord()
        : std::logic_error("WARNING: Word already exists!\n") {
    }
};

class InvalidListState final : public std::runtime_error {
public:
    explicit InvalidListState()
        : std::runtime_error("ERROR: List Invalid list state\n") {
    }
};

class NodeNotFoundInList final : public std::logic_error {
public:
    NodeNotFoundInList() : std::logic_error("WARNING: List Node not found in list\n") {
    }
};

class EmptyValueInNode final : public std::logic_error {
public:
    EmptyValueInNode() : std::logic_error("WARNING: List Node contains an empty value\n") {
    }
};

class WrongColor final : public std::runtime_error {
public:
    WrongColor() : std::runtime_error("ERROR: Invalid color value specified\n") {
    }
};

class RotationErrorNullptr final : public std::runtime_error {
public:
    explicit RotationErrorNullptr(const std::string & side)
        : std::runtime_error("ERROR:" + side + "Cannot rotate around a null node.\n") {
    }
};

class LeftRotationError final : public std::runtime_error {
public:
    explicit LeftRotationError()
        : std::runtime_error("ERROR: Cannot rotate left; the node's right child is null.\n") {
    }
};

class RightRotationError final : public std::runtime_error {
public:
    explicit RightRotationError()
        : std::runtime_error("ERROR: Cannot rotate right; the node's left child is null.\n") {
    }
};

class EmptyValue final : public std::logic_error {
public:
    explicit EmptyValue()
        : std::logic_error("WARNING: Cannot insert an empty string value.\n") {
    }
};

class NullInsertionNode final : public std::logic_error {
public:
    NullInsertionNode()
        : std::logic_error("WARNING: Initial node for fixup cannot be null.\n") {
    }
};

class MissingGrandparent final : public std::runtime_error {
public:
    MissingGrandparent()
        : std::runtime_error("ERROR: RED parent node has no grandparent.\n") {
    }
};

class NodeNotFound final : public std::logic_error {
public:
    explicit NodeNotFound()
        : std::logic_error("WARNING: Word not found in the tree\n") {
    }
};

class CorruptedTreeException final : public std::runtime_error {
public:
    CorruptedTreeException() : std::runtime_error("ERROR: Corrupted tree structure.\n") {
    }
};

class InvalidColorException final : public std::runtime_error {
public:
    InvalidColorException() : std::runtime_error("ERROR: Invalid color state.\n") {
    }
};

class RotationException final : public std::runtime_error {
public:
    RotationException() : std::runtime_error("ERROR: Rotation error in tree.\n") {
    }
};

class InvalidWordLanguage final : public std::logic_error {
public:
    InvalidWordLanguage() : std::logic_error("WARNING: Word should be in English.\n") {
    }
};

class InvalidTranslationLanguage final : public std::logic_error {
public:
    InvalidTranslationLanguage() : std::logic_error("WARNING: Translation should be in Russian.\n") {
    }
};

class NoFoundWord final : public std::logic_error {
public:
    explicit NoFoundWord()
        : std::logic_error("WARNING: Word not found in dictionary.\n") {
    }
};

class ProgramExit final : public std::exception {
public:
    ProgramExit() {}
    const char* what() const noexcept override { return "Program exit requested"; }
};

#endif //EXCEPTIONS_H
