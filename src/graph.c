#include "graph.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int compare_nodes_by_id(const void *a, const void *b);
static void skip_whitespace_and_comments(FILE *file, char *c, int *line_number);

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

  // Pierwsze przejście: zliczanie krawędzi i identyfikacja unikalnych wierzchołków
  while ((c = fgetc(file)) != EOF) {
    skip_whitespace_and_comments(file, &c, &line_number);
    if (c == EOF)
      break;

    ungetc(c, file);
    int result = fscanf(file, "%32s %d %d %lf", label, &first_node,
                        &second_node, &weight);

    if (result == 4) {
      edges_count++;
      int ids[2] = {first_node, second_node};
      for (int i = 0; i < 2; i++) {
        if (!list_contains(unique_nodes, ids[i])) {
          list_prepend(&unique_nodes, ids[i]);
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
    list_free(unique_nodes);
    fclose(file);
    return MEMORY_ERROR;
  }

  graph->nodes_count = nodes_count;
  graph->edges_count = edges_count;
  graph->nodes = (Node *)malloc(nodes_count * sizeof(Node));
  graph->edges = (Edge *)malloc(edges_count * sizeof(Edge));

  if (graph->nodes == NULL || graph->edges == NULL) {
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

  // Sortowanie wierzchołków po ID umożliwia szybkie wyszukiwanie binarne
  qsort(graph->nodes, graph->nodes_count, sizeof(Node), compare_nodes_by_id);

  // Drugie przejście: wczytywanie krawędzi i mapowanie ID wierzchołków na indeksy
  rewind(file);
  int edge_index = 0;
  line_number = 1;
  while ((c = fgetc(file)) != EOF) {
    skip_whitespace_and_comments(file, &c, &line_number);
    if (c == EOF)
      break;

    ungetc(c, file);
    if (fscanf(file, "%32s %d %d %lf", label, &first_node, &second_node,
               &weight) == 4) {
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
  if (file == NULL)
    return FILE_ERROR;
  for (int i = 0; i < graph->nodes_count; i++) {
    fprintf(file, "%d %lf %lf\n", graph->nodes[i].id, graph->nodes[i].x,
            graph->nodes[i].y);
  }
  fclose(file);
  return SUCCESS;
}

int save_graph_as_binary(Graph *graph, char path[]) {
  FILE *file = fopen(path, "wb");
  if (file == NULL)
    return FILE_ERROR;
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
  if (graph->nodes)
    free(graph->nodes);
  if (graph->edges)
    free(graph->edges);
  free(graph);
  return SUCCESS;
}

int get_node_index(Graph *graph, int node_id) {
  Node key = {.id = node_id};
  Node *result = (Node *)bsearch(&key, graph->nodes, graph->nodes_count,
                                 sizeof(Node), compare_nodes_by_id);
  return (result == NULL) ? -1 : (int)(result - graph->nodes);
}

static int compare_nodes_by_id(const void *a, const void *b) {
  return ((Node *)a)->id - ((Node *)b)->id;
}

// Pomija białe znaki i komentarze w pliku wejściowym (linie zaczynające się od #)
static void skip_whitespace_and_comments(FILE *file, char *c,
                                         int *line_number) {
  while (*c != EOF) {
    if (*c == ' ' || *c == '\t' || *c == '\r') {
      *c = fgetc(file);
    } else if (*c == '\n') {
      (*line_number)++;
      *c = fgetc(file);
    } else if (*c == '#') {
      while ((*c = fgetc(file)) != EOF && *c != '\n')
        ;
    } else {
      break;
    }
  }
}
