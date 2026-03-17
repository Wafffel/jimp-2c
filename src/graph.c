#include "graph.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int compare_nodes_by_id(const void *a, const void *b);

int load_graph(char path[], Graph **graph_out) {
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    fprintf(stderr, "Error: Cannot open input file: %s\n", path);
    return FILE_ERROR;
  }

  char label[33];
  int first_node, second_node;
  double weight;
  int edges_count = 0;
  int nodes_count = 0;
  List *unique_nodes = NULL;
  int line_number = 1;
  char c;
  while ((c = fgetc(file)) != EOF) {
    if (c == '#') {
      while ((c = fgetc(file)) != EOF && c != '\n')
        continue;
      if (c == '\n')
        line_number++;
      continue;
    }
    if (c == '\n') {
      line_number++;
      continue;
    }
    if (c == ' ' || c == '\t' || c == '\r')
      continue;

    ungetc(c, file);
    int result = fscanf(file, "%32s %d %d %lf", label, &first_node,
                        &second_node, &weight);

    if (result == 4) {
      while ((c = fgetc(file)) != EOF && c != '\n') {
        if (c == '#') {
          while ((c = fgetc(file)) != EOF && c != '\n')
            continue;
          break;
        }
      }
      if (c == '\n')
        line_number++;

      edges_count++;
      int ids[2] = {first_node, second_node};
      for (int i = 0; i < 2; i++) {
        int node_id = ids[i];
        if (!list_contains(unique_nodes, node_id)) {
          list_prepend(&unique_nodes, node_id);
          nodes_count++;
        }
      }
    } else if (result != EOF) {
      list_free(unique_nodes);
      fclose(file);
      return INPUT_FORMAT_ERROR;
    }
  }

  Graph *graph = (Graph *)malloc(sizeof(Graph));
  if (graph == NULL) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    fclose(file);
    return MEMORY_ERROR;
  }

  graph->nodes_count = nodes_count;
  graph->edges_count = edges_count;
  graph->nodes = (Node *)malloc(nodes_count * sizeof(Node));
  graph->edges = (Edge *)malloc(edges_count * sizeof(Edge));

  if (graph->nodes == NULL || graph->edges == NULL) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    free_graph(graph);
    list_free(unique_nodes);
    fclose(file);
    return MEMORY_ERROR;
  }

  int node_index = 0;
  for (List *current = unique_nodes; current != NULL; current = current->next) {
    graph->nodes[node_index].id = current->data;
    graph->nodes[node_index].x = 0.0;
    graph->nodes[node_index].y = 0.0;
    node_index++;
  }
  list_free(unique_nodes);
  unique_nodes = NULL;

  qsort(graph->nodes, graph->nodes_count, sizeof(Node), compare_nodes_by_id);

  rewind(file);
  int edge_index = 0;
  line_number = 1;
  while ((c = fgetc(file)) != EOF) {
    if (c == '#') {
      while ((c = fgetc(file)) != EOF && c != '\n')
        continue;
      line_number++;
      continue;
    }
    if (c == '\n') {
      line_number++;
      continue;
    }
    if (c == ' ' || c == '\t' || c == '\r')
      continue;

    ungetc(c, file);
    if (fscanf(file, "%32s %d %d %lf", label, &first_node, &second_node,
               &weight) == 4) {
      while ((c = fgetc(file)) != EOF && c != '\n') {
        if (c == '#') {
          while ((c = fgetc(file)) != EOF && c != '\n')
            continue;
          break;
        }
      }

      graph->edges[edge_index].first_node_index =
          get_node_index(graph, first_node);
      graph->edges[edge_index].second_node_index =
          get_node_index(graph, second_node);
      graph->edges[edge_index].weight = weight;
      strncpy(graph->edges[edge_index].label, label, 32);
      graph->edges[edge_index].label[32] = '\0';
      edge_index++;
    }
  }

  fclose(file);
  *graph_out = graph;
  return SUCCESS;
}

int save_graph_as_text(Graph *graph, char path[]) {
  FILE *file = fopen(path, "w");
  if (file == NULL) {
    fprintf(stderr, "Error: Cannot create output file: %s\n", path);
    return FILE_ERROR;
  }
  for (int i = 0; i < graph->nodes_count; i++) {
    fprintf(file, "%d %lf %lf\n", graph->nodes[i].id, graph->nodes[i].x,
            graph->nodes[i].y);
  }
  fclose(file);
  return SUCCESS;
}

int save_graph_as_binary(Graph *graph, char path[]) {
  FILE *file = fopen(path, "wb");
  if (file == NULL) {
    fprintf(stderr, "Error: Cannot create output file: %s\n", path);
    return FILE_ERROR;
  }
  for (int i = 0; i < graph->nodes_count; i++) {
    fwrite(&graph->nodes[i].id, sizeof(int), 1, file);
    fwrite(&graph->nodes[i].x, sizeof(double), 1, file);
    fwrite(&graph->nodes[i].y, sizeof(double), 1, file);
  }
  fclose(file);
  return SUCCESS;
}

int free_graph(Graph *graph) {
  if (graph == NULL)
    return SUCCESS;
  if (graph->nodes != NULL)
    free(graph->nodes);
  if (graph->edges != NULL)
    free(graph->edges);
  free(graph);
  return SUCCESS;
}

int get_node_index(Graph *graph, int node_id) {
  Node key = {.id = node_id, .x = 0.0, .y = 0.0};
  Node *result = (Node *)bsearch(&key, graph->nodes, graph->nodes_count,
                                 sizeof(Node), compare_nodes_by_id);
  if (result == NULL)
    return -1;
  return (int)(result - graph->nodes);
}

static int compare_nodes_by_id(const void *a, const void *b) {
  const Node *node_a = (const Node *)a;
  const Node *node_b = (const Node *)b;
  return node_a->id - node_b->id;
}
