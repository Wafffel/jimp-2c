#include "adjacency_list.h"
#include "graph.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static int add_neighbor(Neighbor **list, int node_index, double weight);

int create_adjacency_list(Graph *graph, AdjacencyList **adj_list_out) {
  AdjacencyList *adj_list = (AdjacencyList *)malloc(sizeof(AdjacencyList));
  if (adj_list == NULL) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    return MEMORY_ERROR;
  }

  adj_list->nodes_count = graph->nodes_count;
  adj_list->adjacency_list =
      (Neighbor **)calloc(graph->nodes_count, sizeof(Neighbor *));
  adj_list->degrees = (int *)calloc(graph->nodes_count, sizeof(int));

  if (adj_list->adjacency_list == NULL || adj_list->degrees == NULL) {
    free_adjacency_list(adj_list);
    fprintf(stderr, "Error: Memory allocation failed\n");
    return MEMORY_ERROR;
  }

  // Dla każdej krawędzi dodajemy obie relacje (graf nieskierowany)
  for (int i = 0; i < graph->edges_count; i++) {
    int first_index = graph->edges[i].first_node_index;
    int second_index = graph->edges[i].second_node_index;
    double weight = graph->edges[i].weight;

    if (add_neighbor(&adj_list->adjacency_list[first_index], second_index,
                     weight) != SUCCESS ||
        add_neighbor(&adj_list->adjacency_list[second_index], first_index,
                     weight) != SUCCESS) {
      free_adjacency_list(adj_list);
      fprintf(stderr, "Error: Memory allocation failed\n");
      return MEMORY_ERROR;
    }
    adj_list->degrees[first_index]++;
    adj_list->degrees[second_index]++;
  }

  *adj_list_out = adj_list;
  return SUCCESS;
}

void free_adjacency_list(AdjacencyList *adj_list) {
  if (adj_list == NULL)
    return;

  if (adj_list->adjacency_list != NULL) {
    for (int i = 0; i < adj_list->nodes_count; i++) {
      Neighbor *current = adj_list->adjacency_list[i];
      while (current != NULL) {
        Neighbor *next = current->next;
        free(current);
        current = next;
      }
    }
    free(adj_list->adjacency_list);
  }

  free(adj_list->degrees);
  free(adj_list);
}

static int add_neighbor(Neighbor **list, int node_index, double weight) {
  Neighbor *neighbor = (Neighbor *)malloc(sizeof(Neighbor));
  if (neighbor == NULL) {
    return MEMORY_ERROR;
  }
  neighbor->node_index = node_index;
  neighbor->weight = weight;
  neighbor->next = *list;
  *list = neighbor;
  return SUCCESS;
}
