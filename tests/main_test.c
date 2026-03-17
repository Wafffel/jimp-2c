#include "test_adjacency_list.h"
#include "test_graph.h"
#include "test_tutte.h"
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

  test_get_node_index();
  test_load_graph_success();
  test_load_graph_file_error();
  test_load_graph_format_error();
  test_load_graph_with_comments();
  test_load_graph_with_inline_comments();
  test_load_graph_with_mixed_comments();

  test_adjacency_list_creation();
  test_adjacency_list_degrees();
  test_adjacency_list_empty_graph();

  test_tutte_small_graph();
  test_tutte_complete_graph();

  fprintf(stdout, "All tests passed!\n");

  return 0;
}
