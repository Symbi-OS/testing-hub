#include "sym_unit_testing.h"

#define LOOP_COUNT (1 << 12)

TEST_CASE(elevate_lower) {
  sym_elevate();
  sym_lower();
}

TEST_CASE(elevate_lower_loop) {
  for (size_t i = 0; i < LOOP_COUNT; ++i) {
    sym_elevate();
    sym_lower();
  }
}

UNIT_TEST_ENTRY_POINT(elevate_lower);

