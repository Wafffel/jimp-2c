#ifndef TEST_GRAPH_H
#define TEST_GRAPH_H

void ceate_dummy_file(const char *path, const char *content);
void test_get_node_index();
void test_load_graph_success();
void test_load_graph_file_error();
void test_load_graph_format_error();
void test_load_graph_with_comments();
void test_load_graph_with_inline_comments();
void test_load_graph_with_mixed_comments();

#endif
