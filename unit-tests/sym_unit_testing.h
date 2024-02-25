#ifndef SYM_UNIT_TESTING_H
#define SYM_UNIT_TESTING_H
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <LINF/sym_all.h>

// ANSI color codes
#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_RESET "\033[0m"

typedef void (*test_func_t)(void);

typedef struct test_case {
  test_func_t function;
  const char* name;
  struct test_case* next;
} test_case_t;

static test_case_t* test_list_head = NULL;
static int tests_passed = 0;
static int tests_failed = 0;
static const char* current_test_name = NULL;

static void register_test_case(test_func_t test, const char* name) {
  test_case_t* new_test_case = (test_case_t*)malloc(sizeof(test_case_t));
  if (new_test_case == NULL) {
    printf(COLOR_RED "Failed to register test case due to memory allocation error." COLOR_RESET "\n");
    exit(1);
  }
  
  new_test_case->function = test;
  new_test_case->name = strdup(name); // Duplicate the string to ensure it's not overwritten
  new_test_case->next = NULL; // This new test case will be the last in the list
  
  if (test_list_head == NULL) {
    // If the list is empty, this new test case becomes the head
    test_list_head = new_test_case;
  } else {
    // Otherwise, find the last test case in the list and append the new test case
    test_case_t* current = test_list_head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = new_test_case;
  }
}

#define TEST_CASE(name)                                             \
  static void name(void);                                           \
  static void __attribute__((constructor)) register_##name(void) {  \
    register_test_case(name, #name);                                \
  }                                                                 \
  static void name(void)

#define ASSERT(condition) do {                                          \
    if (!(condition)) {                                                 \
      printf(COLOR_RED "Assertion failed: %s, in function %s, file %s, line %d." COLOR_RESET "\n", \
             #condition, __func__, __FILE__, __LINE__);                 \
      tests_failed++;                                                   \
      return;                                                           \
    }                                                                   \
  } while(0)

static void run_test(test_case_t *test) {
  current_test_name = test->name;
  
  int initial_fail_count = tests_failed;
  test->function();

  if (tests_failed == initial_fail_count) {
    tests_passed++;
    printf(COLOR_GREEN "PASS: %s" COLOR_RESET "\n", test->name);
  } else {
    printf(COLOR_RED "FAIL: %s" COLOR_RESET "\n", test->name);
  }

  current_test_name = NULL;
}

static void run_all_tests(void) {
  test_case_t* current = test_list_head;
  while (current != NULL) {
    printf(COLOR_YELLOW "Running %s..." COLOR_RESET "\n", current->name);
    run_test(current); // Let run_test handle pass/fail logic
    current = current->next;
  }
}

static int print_test_results(void) {
  printf(COLOR_YELLOW "\nTest Summary:" COLOR_RESET "\n");
  printf(COLOR_GREEN "Passed: %d" COLOR_RESET "\n", tests_passed);
  if (tests_failed > 0) {
    printf(COLOR_RED "Failed: %d" COLOR_RESET "\n", tests_failed);
    return 1;
  } else {
    printf(COLOR_GREEN "All tests passed!" COLOR_RESET "\n");
    return 0;
  }
}

static void segfault_signal_handler(int _signal) {
  (void)_signal;
  int remaining_tests = 0;
  test_case_t* current = test_list_head;

  // Find the current test in the list and count remaining tests
  while (current != NULL && strcmp(current->name, current_test_name) != 0) {
    current = current->next;
  }
  // If current is not NULL, we found the test that segfaulted, count remaining tests
  if (current != NULL) {
    for (; current != NULL; current = current->next) {
      remaining_tests++;
    }
  }

  // Decrement to exclude the current test which caused the segfault
  remaining_tests = remaining_tests > 0 ? remaining_tests - 1 : 0;

  // Print message about the segfault and remaining tests
  printf(COLOR_RED "SEGFAULT in %s. Skipping %d remaining test(s)." COLOR_RESET "\n", 
         current_test_name ? current_test_name : "Unknown Test", remaining_tests);

  // Assume all remaining tests failed due to the segfault
  tests_failed += remaining_tests;
    
  // Print test results before exiting
  print_test_results();
  exit(1);
}


#define UNIT_TEST_ENTRY_POINT(name)                                     \
  int main() {                                                          \
    signal(SIGSEGV, segfault_signal_handler);                           \
    printf("*------ %s Unit Test ------*\n", #name);                    \
    run_all_tests();                                                    \
    return print_test_results();                                        \
  }

#endif
 
