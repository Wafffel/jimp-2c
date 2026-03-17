#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include "graph.h"

typedef struct Neighbor {
  int node_index;
  double weight;
  struct Neighbor *next;
} Neighbor;

typedef struct {
  Neighbor **adjacency_list;
  int nodes_count;
  int *degrees;
} AdjacencyList;

int create_adjacency_list(Graph *graph, AdjacencyList **adj_list_out);
void free_adjacency_list(AdjacencyList *adj_list);

#endif
