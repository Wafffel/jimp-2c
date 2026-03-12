#include "test_graph.h"
#include "../src/graph.h"
#include "../src/utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void create_dummy_file(const char *path, const char *content) {
  FILE *f = fopen(path, "w");
  if (f == NULL) {
    fprintf(stderr, "Nie można utworzyć pliku tymczasowego dla testu");
    exit(1);
  }
  fprintf(f, "%s", content);
  fclose(f);
}

void test_get_node_index() {
  printf("Testing get_node_index... ");
  Graph g;
  g.nodes_count = 4;
  g.nodes = malloc(4 * sizeof(Node));

  g.nodes[0].id = 10;
  g.nodes[1].id = 25;
  g.nodes[2].id = 40;
  g.nodes[3].id = 100;

  assert(get_node_index(&g, 10) == 0);
  assert(get_node_index(&g, 40) == 2);
  assert(get_node_index(&g, 100) == 3);
  assert(get_node_index(&g, 999) == -1);

  free(g.nodes);
  printf("PASSED\n");
}

void test_load_graph_success() {
  printf("Testing load_graph success... ");
  const char *path = "test_ok.txt";
  create_dummy_file(path, "# Komentarz\n"
                          "L1 1 2 1.5\n"
                          "L2 2 3 0.5\n");

  Graph *g = NULL;
  int status = load_graph((char *)path, &g);

  assert(status == 0);
  assert(g != NULL);
  assert(g->nodes_count == 3);
  assert(g->edges_count == 2);

  int idx1 = get_node_index(g, 1);
  int idx2 = get_node_index(g, 2);
  int first = g->edges[0].first_node_index;
  int second = g->edges[0].second_node_index;

  assert((first == idx1 && second == idx2) ||
         (first == idx2 && second == idx1));
  assert(g->edges[0].weight == 1.5);

  free_graph(g);
  remove(path);
  printf("PASSED\n");
}

void test_load_graph_file_error() {
  printf("Testing load_graph file error (Code 2)... ");
  Graph *g = NULL;
  int status = load_graph("non_existent_file.txt", &g);

  assert(status == FILE_ERROR);
  assert(g == NULL);
  printf("PASSED\n");
}

void test_load_graph_format_error() {
  printf("Testing load_graph format error (Code 3)... ");
  const char *path = "test_bad.txt";
  create_dummy_file(path, "Label 1 2 ThisIsNotANumber\n");

  Graph *g = NULL;
  int status = load_graph((char *)path, &g);

  assert(status == INPUT_FORMAT_ERROR);
  assert(g == NULL);

  remove(path);
  printf("PASSED\n");
}
