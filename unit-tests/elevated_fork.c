#define _GNU_SOURCE
#include "sym_unit_testing.h"
#include <sched.h>

uint64_t read_cr3() {
  uint64_t cr3;
  asm volatile("mov %%cr3, %0" : "=r"(cr3));
  return cr3;
}

void taskset_to_core(int core) {
  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(core, &set);

  if (sched_setaffinity(0, sizeof(set), &set) == -1) {
    perror("sched_setaffinity");
    exit(1);
  }
}

/*
  Important notes:
  in order for the elevated fork path to work,
  the process has to be pinned to a single core either
  through the use of 'taskset' or 'sched_setaffinity()'.
*/
TEST_CASE(elevated_fork) {
  // Make sure we are pinned to core 0, but any core would work
  taskset_to_core(0);
  
  sym_elevate();

  if (fork() == 0) {
    printf("parent pid: %i\n", getpid());
    printf("parent cr3: 0x%lx\n", read_cr3());
  } else {
    printf("child pid: %i\n", getpid());
    printf("child cr3: 0x%lx\n", read_cr3());
  }
  
  sym_lower();
}

UNIT_TEST_ENTRY_POINT(elevated_fork);

