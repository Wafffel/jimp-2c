#include "test_graph.h"
#include <stdio.h>

void mute_stderr() {
#ifdef _WIN32
  freopen("NUL", "w", stderr);
#else
  freopen("/dev/null", "w", stderr);
#endif
}

int main() {
  fprintf(stdout, "Running tests...\n");

  mute_stderr();

  // Graph tests
  test_get_node_index();
  test_load_graph_success();
  test_load_graph_file_error();
  test_load_graph_format_error();

  fprintf(stdout, "All tests passed!\n");

  return 0;
}
