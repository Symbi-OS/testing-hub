#include "sym_unit_testing.h"

TEST_CASE(sc_lib_begin_elevated_status) {
  long status = sym_check_elevate();
  ASSERT(status != 0);
}

TEST_CASE(sc_lib_shortcutting) {
  long status = sym_check_elevate();
  ASSERT(status != 0);

  const char* buf = "test\n";
  size_t len = strlen(buf);

  size_t bytes = write(1, buf, len);
  ASSERT(bytes == len);
}

UNIT_TEST_ENTRY_POINT(sc_lib_test);

