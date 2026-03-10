#include "graph.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Graph *load_graph(char path[])
{
  FILE *file = fopen(path, "r");
  if (file == NULL)
  {
    fprintf(stderr, "Error: Cannot open input file: %s", path);
    exit(FILE_ERROR);
  }

  Graph *graph = (Graph *)malloc(sizeof(Graph));
  if (graph == NULL)
  {
    fprintf(stderr, "Error: Memory allocation failed\n");
    fclose(file);
    exit(MEMORY_ERROR);
  }

  char label[33];
  int first_node, second_node;
  double weight;
  int edges_count = 0;

  List *unique_nodes = NULL;
  int nodes_count = 0;

  char c;
  while ((c = fgetc(file)) != EOF)
  {
    if (c == '#')
    {
      while ((c = fgetc(file)) != EOF && c != '\n')
        continue;
      continue;
    }
    if (c == '\n' || c == ' ' || c == '\t' || c == '\r')
      continue;

    ungetc(c, file);
    if (fscanf(file, "%32s %d %d %lf", label, &first_node, &second_node, &weight) == 4)
    {
      edges_count++;
      int ids[2] = {first_node, second_node};
      for (int i = 0; i < 2; i++)
      {
        if (!list_contains(unique_nodes, ids[i]))
        {
          list_prepend(&unique_nodes, ids[i]);
          nodes_count++;
        }
      }
    }
  }

  graph->nodes_count = nodes_count;
  graph->edges_count = edges_count;
  graph->nodes = (Node *)malloc(nodes_count * sizeof(Node));
  graph->edges = (Edge *)malloc(edges_count * sizeof(Edge));
  if (graph->nodes == NULL || graph->edges == NULL)
  {
    fprintf(stderr, "Error: Memory allocation failed\n");
    free_graph(graph);
    list_free(unique_nodes);
    fclose(file);
    exit(MEMORY_ERROR);
  }
  int node_index = 0;
  for (List *cur = unique_nodes; cur != NULL; cur = cur->next)
  {
    graph->nodes[node_index].id = cur->data;
    graph->nodes[node_index].x = 0.0;
    graph->nodes[node_index].y = 0.0;
    node_index++;
  }
  list_free(unique_nodes);

  rewind(file);
  int edge_index = 0;
  while ((c = fgetc(file)) != EOF)
  {
    if (c == '#')
    {
      while ((c = fgetc(file)) != EOF && c != '\n')
        continue;
      continue;
    }
    if (c == '\n' || c == ' ' || c == '\t' || c == '\r')
      continue;

    ungetc(c, file);
    if (fscanf(file, "%32s %d %d %lf", label, &first_node, &second_node, &weight) == 4)
    {
      graph->edges[edge_index].first_node_index = get_node_index(graph, first_node);
      graph->edges[edge_index].second_node_index = get_node_index(graph, second_node);
      graph->edges[edge_index].weight = weight;
      strncpy(graph->edges[edge_index].label, label, 32);
      graph->edges[edge_index].label[32] = '\0';
      edge_index++;
    }
  }

  fclose(file);
  return graph;
}

int get_node_index(Graph *graph, int node_id)
{
  for (int i = 0; i < graph->nodes_count; i++)
    if (graph->nodes[i].id == node_id)
      return i;
  return -1;
}

void save_graph_as_text(Graph *graph, char path[])
{
  FILE *file = fopen(path, "w");
  if (file == NULL)
  {
    fprintf(stderr, "Error: Cannot create output file: %s", path);
    exit(FILE_ERROR);
  }
  for (int i = 0; i < graph->nodes_count; i++)
    fprintf(file, "%d %lf %lf\n", graph->nodes[i].id, graph->nodes[i].x, graph->nodes[i].y);
  fclose(file);
}

void save_graph_as_binary(Graph *graph, char path[])
{
  FILE *file = fopen(path, "wb");
  if (file == NULL)
  {
    fprintf(stderr, "Error: Cannot create output file: %s", path);
    exit(FILE_ERROR);
  }
  for (int i = 0; i < graph->nodes_count; i++)
  {
    fwrite(&graph->nodes[i].id, sizeof(int), 1, file);
    fwrite(&graph->nodes[i].x, sizeof(double), 1, file);
    fwrite(&graph->nodes[i].y, sizeof(double), 1, file);
  }
  fclose(file);
}

void free_graph(Graph *graph)
{
  free(graph->nodes);
  free(graph->edges);
  free(graph);
}
