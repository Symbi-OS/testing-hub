#include "sym_unit_testing.h"

typedef size_t (*ksys_write_t)(unsigned int fd, const char *buf, size_t count);

TEST_CASE(ksys_write) {
  ksys_write_t ksys_write = (ksys_write_t)sym_get_fn_address("ksys_write");
  ASSERT(ksys_write != NULL);

  const char* msg = "ksys_write shortcut used!\n";
  size_t msg_len = strlen(msg);

  sym_elevate();
  size_t bytes = ksys_write(1, msg, msg_len);
  sym_lower();

  ASSERT(bytes == msg_len);
}

UNIT_TEST_ENTRY_POINT(ksys_write);

