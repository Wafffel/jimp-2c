#include "test_adjacency_list.h"
#include "../src/adjacency_list.h"
#include "../src/graph.h"
#include "../src/utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_adjacency_list_creation() {
  printf("Testing adjacency list creation... ");
  
  Graph *g = (Graph *)malloc(sizeof(Graph));
  g->nodes_count = 3;
  g->edges_count = 2;
  g->nodes = (Node *)malloc(3 * sizeof(Node));
  g->edges = (Edge *)malloc(2 * sizeof(Edge));
  
  g->nodes[0].id = 1;
  g->nodes[1].id = 2;
  g->nodes[2].id = 3;
  
  g->edges[0].first_node_index = 0;
  g->edges[0].second_node_index = 1;
  g->edges[0].weight = 1.5;
  
  g->edges[1].first_node_index = 1;
  g->edges[1].second_node_index = 2;
  g->edges[1].weight = 2.0;
  
  AdjacencyList *adj_list;
  int result = create_adjacency_list(g, &adj_list);
  
  assert(result == SUCCESS);
  assert(adj_list != NULL);
  assert(adj_list->nodes_count == 3);
  assert(adj_list->degrees[0] == 1);
  assert(adj_list->degrees[1] == 2);
  assert(adj_list->degrees[2] == 1);
  
  Neighbor *neighbor = adj_list->adjacency_list[1];
  int count = 0;
  while (neighbor != NULL) {
    count++;
    neighbor = neighbor->next;
  }
  assert(count == 2);
  
  free_adjacency_list(adj_list);
  free_graph(g);
  
  printf("PASSED\n");
}

void test_adjacency_list_degrees() {
  printf("Testing adjacency list degree calculation... ");
  
  Graph *g = (Graph *)malloc(sizeof(Graph));
  g->nodes_count = 5;
  g->edges_count = 6;
  g->nodes = (Node *)malloc(5 * sizeof(Node));
  g->edges = (Edge *)malloc(6 * sizeof(Edge));
  
  for (int i = 0; i < 5; i++) {
    g->nodes[i].id = i + 1;
  }
  
  g->edges[0].first_node_index = 0;
  g->edges[0].second_node_index = 1;
  g->edges[0].weight = 1.0;
  
  g->edges[1].first_node_index = 1;
  g->edges[1].second_node_index = 2;
  g->edges[1].weight = 1.0;
  
  g->edges[2].first_node_index = 2;
  g->edges[2].second_node_index = 3;
  g->edges[2].weight = 1.0;
  
  g->edges[3].first_node_index = 3;
  g->edges[3].second_node_index = 4;
  g->edges[3].weight = 1.0;
  
  g->edges[4].first_node_index = 4;
  g->edges[4].second_node_index = 0;
  g->edges[4].weight = 1.0;
  
  g->edges[5].first_node_index = 0;
  g->edges[5].second_node_index = 2;
  g->edges[5].weight = 1.5;
  
  AdjacencyList *adj_list;
  int result = create_adjacency_list(g, &adj_list);
  
  assert(result == SUCCESS);
  assert(adj_list->degrees[0] == 3);
  assert(adj_list->degrees[1] == 2);
  assert(adj_list->degrees[2] == 3);
  assert(adj_list->degrees[3] == 2);
  assert(adj_list->degrees[4] == 2);
  
  free_adjacency_list(adj_list);
  free_graph(g);
  
  printf("PASSED\n");
}

void test_adjacency_list_empty_graph() {
  printf("Testing adjacency list with empty graph... ");
  
  Graph *g = (Graph *)malloc(sizeof(Graph));
  g->nodes_count = 0;
  g->edges_count = 0;
  g->nodes = NULL;
  g->edges = NULL;
  
  AdjacencyList *adj_list;
  int result = create_adjacency_list(g, &adj_list);
  
  assert(result == SUCCESS);
  assert(adj_list != NULL);
  assert(adj_list->nodes_count == 0);
  
  free_adjacency_list(adj_list);
  free_graph(g);
  
  printf("PASSED\n");
}
