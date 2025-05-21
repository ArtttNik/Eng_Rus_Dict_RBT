#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

class FailOfMemoryAllocation final : public std::runtime_error {
public:
    explicit FailOfMemoryAllocation(const std::string & objectType)
        : std::runtime_error("ERROR: Memory allocation failed for " + objectType + "\n") {
    }
};

class DuplicateTranslation final : public std::logic_error {
public:
    explicit DuplicateTranslation()
        : std::logic_error("WARNING: Translation already exists!\n") {
    }
};

class DuplicateWord final : public std::logic_error {
public:
    explicit DuplicateWord()
        : std::logic_error("WARNING: Word already exists!\n") {
    }
};

class TranslationsNotFound final : public std::logic_error {
public:
    TranslationsNotFound() : std::logic_error("WARNING: Translations not found\n") {
    }
};

class NoFoundWord final : public std::logic_error {
public:
    explicit NoFoundWord()
        : std::logic_error("WARNING: Word not found\n") {
    }
};

class EmptyValue final : public std::logic_error {
public:
    explicit EmptyValue()
        : std::logic_error("WARNING: Cannot insert an empty string value.\n") {
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

class ErrorInFile final : public std::runtime_error {
public:
    ErrorInFile() : std::runtime_error("ERROR: something went wrong with file\n") {
    }
};

class Exit final : public std::exception {
public:
    Exit() : std::exception() {
    }
};
#endif //EXCEPTIONS_H
