Lab 7 Comprehensive Test Suite
================================

Author: Reid Hood

This test suite contains 55 carefully designed test cases for the Lab 7 compiler project.
The tests are designed with a logical progression of features and comprehensive coverage
of the language specification, with special attention to finding potential bugs.

TEST ORGANIZATION:
==================

Tests are numbered test01 through test55 to ensure proper ordering.

CATEGORIES:

1. BASIC TESTS (test01-test05):
   - Simple print statements
   - Variable declaration and assignment
   - Basic arithmetic operations (+, -, *, /, %)

2. OPERATOR TESTS (test06-test10):
   - Operator precedence
   - Unary negation
   - Comparison operators (<, >, <=, >=)
   - Equality operators (==, !=)
   - Logical AND operator

3. CONTROL FLOW TESTS (test11-test20):
   - Logical OR operator
   - IF statements (with/without condition)
   - IF-ELSE statements
   - Nested IF statements
   - WHILE loops (simple and complex)
   - Nested loops
   - IF within WHILE

4. FUNCTION TESTS (test21-test26):
   - Function declaration and calls (no parameters)
   - Functions with single/multiple parameters
   - Functions with local variables
   - Nested function calls
   - Functions with loops and control flow
   - Factorial calculation

5. ARRAY TESTS (test27-test32):
   - Array declaration and access
   - Array with variable indices
   - Array with expression indices
   - Arrays in loops
   - Array element modification
   - Passing arrays to functions (extra credit - implemented)

6. STRING TESTS (test33):
   - PRINTS statement for string output
   - Mixed print and prints statements

7. COMPLEX INTEGRATION TESTS (test34-test50):
   - Complex expressions with multiple operators
   - Multiple variables and operations
   - Logical operators with comparisons
   - Fibonacci calculation
   - GCD and MAX functions
   - Zero and negative numbers
   - Boundary conditions
   - Nested if-else-if chains
   - Variable shadowing
   - Early returns
   - Counting and accumulation patterns
   - Power calculations
   - Sum and product range calculations

8. CHAR AND STRUCT TESTS (test51-test55):
   - test51: Char variable declaration, assignment, and print
   - test52: Char arrays with 1-byte element operations
   - test53: Struct variable with field access (lval and rval)
   - test54: Multiple struct instances with field operations
   - test55: Stack preservation across nested function calls

TEST DESIGN PHILOSOPHY:
========================

These tests follow an "evil nemesis" approach:
- Each test isolates a specific feature or combination
- Tests are designed to CATCH BUGS, not just confirm features work
- Edge cases and boundary conditions are tested
- Tests progress logically so each builds on previous features
- Later tests combine multiple features to catch integration issues

FEATURES TESTED:
=================

✓ Integer variables and constants
✓ Char variables (1-byte operations)
✓ All arithmetic operators: +, -, *, /, %
✓ All comparison operators: <, >, <=, >=, ==, !=
✓ Logical operators: &&, ||
✓ Operator precedence and associativity
✓ Unary negation
✓ Assignment and multiple assignments
✓ IF statements (with and without ELSE)
✓ Nested IF statements
✓ WHILE loops
✓ Nested loops
✓ IF statements within loops
✓ Function declarations (with/without parameters)
✓ Function calls (with/without arguments)
✓ Function return values
✓ Nested function calls
✓ Functions with local variables
✓ Functions with loops and control flow
✓ Single-dimensional arrays
✓ Char arrays (1-byte element operations)
✓ Array indexing with constants
✓ Array indexing with variables
✓ Array indexing with expressions
✓ Arrays in loops
✓ Structs as both lvals and rvals
✓ Multiple struct instances
✓ Passing arrays to functions (extra credit)
✓ Stack preservation across calls
✓ PRINT statement
✓ PRINTS statement

FEATURES NOT TESTED (Per Requirements):
=========================================

- Floating point operations (not required)
- Long integers (not required)
- Variables in outer scopes from functions (not required)
- Multi-dimensional arrays (not required)
- Functions returning non-int values (not required)
- Large memory data structures (not required)

RUNNING THE TESTS:
===================

To run all tests at once (from Lab 7 directory):
  wsl bash run_tests.sh

Or manually run individual tests with the compiler and stackl interpreter:
  ./lang test_suite/testXX.lang langout.sl
  slasm langout.sl io320.sl
  stackl langout > out
  diff out test_suite/testXX.correct

Expected output for each test is provided in the corresponding .correct file.

TEST FILE FORMAT:
===================

Each test consists of:
- testNN.lang: Source code in the language
- testNN.correct: Expected output (one number per line for print() calls)

Each .lang file contains a comment at the top stating:
// Reid Hood - Description of what the test covers

COVERAGE NOTES:
================

This test suite aims to provide comprehensive coverage of the language features.
A compiler that passes this test suite should:
- Handle all basic operations correctly
- Properly implement operator precedence
- Correctly manage variable scope and storage
- Properly implement control flow
- Correctly handle function calls with parameters
- Properly manage array storage and access

The tests are ordered such that:
1. Basic features are tested first
2. Each subsequent test adds complexity
3. Integration tests combine multiple features
4. Edge cases and boundary conditions are included

The logical progression ensures that if a compiler fails a later test,
earlier tests can help identify the root cause.
