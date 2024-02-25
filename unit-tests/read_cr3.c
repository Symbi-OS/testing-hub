#include "sym_unit_testing.h"

TEST_CASE(read_cr3) {
  uint64_t cr3 = 0;
  
  sym_elevate();
  asm volatile("mov %%cr3, %0" : "=r"(cr3));
  sym_lower();

  printf("cr3: 0x%lx\n", cr3);
  ASSERT(cr3 != 0);
}

UNIT_TEST_ENTRY_POINT(read_cr3);

