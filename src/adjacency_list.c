#include "adjacency_list.h"
#include "graph.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

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

  for (int i = 0; i < graph->edges_count; i++) {
    int first_index = graph->edges[i].first_node_index;
    int second_index = graph->edges[i].second_node_index;
    double weight = graph->edges[i].weight;

    Neighbor *neighbor1 = (Neighbor *)malloc(sizeof(Neighbor));
    if (neighbor1 == NULL) {
      free_adjacency_list(adj_list);
      fprintf(stderr, "Error: Memory allocation failed\n");
      return MEMORY_ERROR;
    }
    neighbor1->node_index = second_index;
    neighbor1->weight = weight;
    neighbor1->next = adj_list->adjacency_list[first_index];
    adj_list->adjacency_list[first_index] = neighbor1;
    adj_list->degrees[first_index]++;

    Neighbor *neighbor2 = (Neighbor *)malloc(sizeof(Neighbor));
    if (neighbor2 == NULL) {
      free_adjacency_list(adj_list);
      fprintf(stderr, "Error: Memory allocation failed\n");
      return MEMORY_ERROR;
    }
    neighbor2->node_index = first_index;
    neighbor2->weight = weight;
    neighbor2->next = adj_list->adjacency_list[second_index];
    adj_list->adjacency_list[second_index] = neighbor2;
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
