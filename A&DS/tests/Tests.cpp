#include "Tests.h"

void printTestResult(const std::string & testName, bool passed) {
    std::cout << (passed ? "[PASSED] " : "[FAILED] (¬_¬ ) ")
            << testName << "\n";
}

void initializeEmptyJsonFile(const std::string & filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    file << "{\n}";
    file.close();
}

void testListConstructors() {
    bool passed = true;

    List list1;
    if (list1.getHead() != nullptr || !list1.isEmpty()) {
        passed = false;
    }

    List list2;
    list2.push("test");
    List list3 = list2;
    if (list3.getHead() == nullptr ||
        list3.isEmpty() ||
        list3.getHead()->value_ != "test") {
        passed = false;
    }

    printTestResult("List Constructors", passed);
}

void testListPush() {
    List list;
    list.push("c");
    list.push("b");
    list.push("a");

    NodeList* head = list.getHead();
    bool passed = head != nullptr &&
             head->value_ == "a" &&
             head->next_ != nullptr &&
             head->next_->value_ == "b" &&
             head->next_->next_ != nullptr &&
             head->next_->next_->value_ == "c";

    printTestResult("List Push", passed);
}

void testListRemove() {
    bool passed = true;
    List list;
    list.push("c");
    list.push("b");
    list.push("a");

    NodeList* node = list.getHead()->next_;
    list.remove(node);
    passed &= list.getSize() == 2 &&
            list.getHead()->value_ == "a" &&
            list.getHead()->next_->value_ == "c";

    list.remove(list.getHead());
    passed &= list.getSize() == 1 &&
            list.getHead()->value_ == "c";

    list.remove(list.getHead());
    passed &= list.isEmpty();

    printTestResult("List Remove", passed);
}

void testListToString() {
    List list;
    list.push("в");
    list.push("б");
    list.push("а");

    bool passed = list.convertTranslationsToString() == "а; б; в";
    printTestResult("List ToString", passed);
}

void testPairConstructors() {
    bool passed = true;

    Pair pair1;
    passed &= pair1.word_.empty() && pair1.translation_.isEmpty();

    List translations;
    translations.push("trans1");
    Pair pair2("word", translations);
    passed &= pair2.word_ == "word" &&
            pair2.translation_.getHead()->value_ == "trans1";

    printTestResult("Pair Constructors", passed);
}

void testPairOperators() {
    bool passed = true;
    List listA, listB, listC;
    listA.push("a");
    listB.push("b");
    listC.push("c");

    const Pair p1("apple", listA);
    const Pair p2("banana", listB);
    const Pair p3("apple", listC);

    passed &= p1 < p2 && !(p2 < p1) &&
            p2 > p1 && !(p1 > p2) &&
            p1 == p3 && !(p1 == p2);

    printTestResult("Pair Operators", passed);
}

void testPairOutputOperator() {
    bool passed = true;

    List list;
    list.push("trans1");
    Pair pair("word", list);

    std::ostringstream oss;
    oss << pair;
    passed &= oss.str() == "word - trans1";

    Pair emptyPair("empty", List());
    try {
        oss.str("");
        oss << emptyPair;
        passed = false;
    } catch (const NoTranslations&) {
        // Ожидаемое исключение
    }

    printTestResult("Pair Output Operator", passed);
}

void testRBTConstructors() {
    RBT tree;
    bool passed = tree.getRoot() == nullptr;
    printTestResult("RBT Constructors", passed);
}

void testRBTInsert() {
    RBT tree;
    List list1, list2, list3;
    list1.push("банан");
    list2.push("яблоко");
    list3.push("вишня");

    tree.insert(Pair("banana", list1));
    tree.insert(Pair("apple", list2));
    tree.insert(Pair("cherry", list3));

    bool passed = tree.search(tree.getRoot(), "apple") != nullptr &&
                  tree.search(tree.getRoot(), "banana") != nullptr &&
                  tree.search(tree.getRoot(), "cherry") != nullptr;

    printTestResult("RBT Insert", passed);
}

void testRBTRemove() {
    RBT tree;
    List list1, list2, list3;
    list1.push("банан");
    list2.push("яблоко");
    list3.push("вишня");

    tree.insert(Pair("banana", list1));
    tree.insert(Pair("apple", list2));
    tree.insert(Pair("cherry", list3));

    tree.remove("apple");
    tree.remove("banana");

    const bool passed = tree.search(tree.getRoot(), "apple") == nullptr &&
                  tree.search(tree.getRoot(), "banana") == nullptr &&
                  tree.search(tree.getRoot(), "cherry") != nullptr;

    printTestResult("RBT Remove", passed);
}

void testRBTSearch() {
    RBT tree;
    List list1, list2;
    list1.push("банан");
    list2.push("яблоко");

    tree.insert(Pair("banana", list1));
    tree.insert(Pair("apple", list2));

    bool passed = tree.search(tree.getRoot(), "apple") != nullptr &&
                  tree.search(tree.getRoot(), "banana") != nullptr &&
                  tree.search(tree.getRoot(), "nonexistent") == nullptr;

    printTestResult("RBT Search", passed);
}

void testRBTToString() {
    RBT tree;
    List list1, list2, list3;
    list1.push("банан");
    list2.push("яблоко");
    list3.push("вишня");

    tree.insert(Pair("banana", list1));
    tree.insert(Pair("apple", list2));
    tree.insert(Pair("cherry", list3));

    std::string result = tree.makeTreeToString();
    bool passed = !result.empty() &&
                  result.find("apple") != std::string::npos &&
                  result.find("banana") != std::string::npos &&
                  result.find("cherry") != std::string::npos;

    printTestResult("RBT ToString", passed);
}

void testRBTExceptions() {
    bool passed = true;
    RBT tree;
    List list;
    list.push("trans");

    tree.insert(Pair("word", list));
    try {
        tree.insert(Pair("word", list));
        passed = false;
    } catch (const DuplicateWord&) {
        // Ожидаемое исключение
    }

    Node* node = tree.search(tree.getRoot(), "word");
    try {
        tree.setColor(node, static_cast<Color>(2));
        passed = false;
    } catch (const WrongColor&) {
        // Ожидаемое исключение
    }

    printTestResult("RBT Exceptions", passed);
}

void testDictInsert() {
    bool passed = true;
    std::string testE = "apple";
    std::string testR = "яблоко";

    Dict dict;
    dict.insert(testE, testR);
    passed &= dict.findTranslationByWord("apple").convertTranslationsToString() == "яблоко";

    try {
        dict.insert(testE, testR);
        passed = false;
    } catch (const DuplicateWord&) {
        // Ожидаемое исключение
    }

    printTestResult("Dict Insert", passed);
}

void testDictExceptions() {
    bool passed = true;
    std::string testE = "apple";
    std::string testR = "яблоко";
    std::string testEmpty = "";
    std::string testStrange = ";;;";

    Dict dict;

    try {
        dict.insert(testR, testE);
        passed = false;
    } catch (const InvalidWordLanguage&) {
        // Ожидаемое исключение
    }

    try {
        dict.insert(testE, testE);
        passed = false;
    } catch (const InvalidTranslationLanguage&) {
        // Ожидаемое исключение
    }

    try {
        dict.insert(testEmpty, testR);
        passed = false;
    } catch (const InvalidWordLanguage&) {
        // Ожидаемое исключение
    }

    try {
        dict.insert(testE, testEmpty);
        passed = false;
    } catch (const EmptyValue&) {
        // Ожидаемое исключение
    }

    try {
        dict.insert(testE, testStrange);
        passed = false;
    } catch (const EmptyValue&) {
        // Ожидаемое исключение
    }

    printTestResult("Dict Exceptions", passed);
}

void runAllTests() {
    initializeEmptyJsonFile(DATAPATH);

    testListConstructors();
    testListPush();
    testListRemove();
    testListToString();

    testPairConstructors();
    testPairOperators();
    testPairOutputOperator();

    testRBTConstructors();
    testRBTInsert();
    testRBTRemove();
    testRBTSearch();
    testRBTToString();
    testRBTExceptions();

    testDictInsert();
    testDictExceptions();

    initializeEmptyJsonFile(DATAPATH);

    std::cout << "Тестирование завершено!\n\n";
    exit(EXIT_SUCCESS);
}
