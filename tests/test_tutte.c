#include "test_tutte.h"
#include "../src/graph.h"
#include "../src/tutte.h"
#include "../src/utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_tutte_small_graph() {
  printf("Testing Tutte with small graph... ");
  
  Graph *g = (Graph *)malloc(sizeof(Graph));
  g->nodes_count = 2;
  g->edges_count = 1;
  g->nodes = (Node *)malloc(2 * sizeof(Node));
  g->edges = (Edge *)malloc(1 * sizeof(Edge));
  
  g->nodes[0].id = 1;
  g->nodes[1].id = 2;
  
  g->edges[0].first_node_index = 0;
  g->edges[0].second_node_index = 1;
  g->edges[0].weight = 1.0;
  
  int result = run_tutte(g, 1000.0, 100);
  
  assert(result == SUCCESS);
  assert(g->nodes[0].x == 0.0);
  assert(g->nodes[0].y == 0.0);
  assert(g->nodes[1].x == 1000.0);
  assert(g->nodes[1].y == 1000.0);
  
  free_graph(g);
  
  printf("PASSED\n");
}

void test_tutte_complete_graph() {
  printf("Testing Tutte with complete  graph... ");
  
  Graph *g = (Graph *)malloc(sizeof(Graph));
  g->nodes_count = 7;
  g->edges_count = 21;
  g->nodes = (Node *)malloc(7 * sizeof(Node));
  g->edges = (Edge *)malloc(21 * sizeof(Edge));
  
  for (int i = 0; i < 7; i++) {
    g->nodes[i].id = i + 1;
    g->nodes[i].x = 0.0;
    g->nodes[i].y = 0.0;
  }
  
  int edge_idx = 0;
  for (int i = 0; i < 7; i++) {
    for (int j = i + 1; j < 7; j++) {
      g->edges[edge_idx].first_node_index = i;
      g->edges[edge_idx].second_node_index = j;
      g->edges[edge_idx].weight = 1.0;
      edge_idx++;
    }
  }
  
  double size = 1000.0;
  int result = run_tutte(g, size, 500);
  
  assert(result == SUCCESS);
  
  for (int i = 0; i < 7; i++) {
    assert(g->nodes[i].x >= 0.0 && g->nodes[i].x <= size);
    assert(g->nodes[i].y >= 0.0 && g->nodes[i].y <= size);
  }
  
  int boundary_nodes = 0;
  int internal_nodes = 0;
  for (int i = 0; i < 7; i++) {
    int on_boundary = (g->nodes[i].x == 0.0 || g->nodes[i].x == size ||
                       g->nodes[i].y == 0.0 || g->nodes[i].y == size);
    if (on_boundary) {
      boundary_nodes++;
    } else {
      internal_nodes++;
    }
  }
  
  assert(boundary_nodes >= 3);
  assert(internal_nodes >= 0);
  
  free_graph(g);
  
  printf("PASSED\n");
}
