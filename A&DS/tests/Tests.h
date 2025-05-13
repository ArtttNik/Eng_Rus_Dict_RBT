#ifndef TESTS_H
#define TESTS_H

#include "../headers/Dict.h"
#include "../headers/RBT.h"
#include "../headers/Structs.h"
#include "../headers/Exceptions.h"

#include <string>
#include <iostream>
#include <sstream>

void printTestResult(const std::string& testName, bool passed);
void initializeEmptyJsonFile(const std::string& filename);

void testListConstructors();
void testListPush();
void testListRemove();
void testListToString();

void testPairConstructors();
void testPairOperators();
void testPairOutputOperator();

void testRBTConstructors();
void testRBTInsert();
void testRBTRemove();
void testRBTSearch();
void testRBTToString();
void testRBTExceptions();

void testDictInsert();
void testDictExceptions();

void runAllTests();

#endif // TESTS_H
