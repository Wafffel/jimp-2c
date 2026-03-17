#include "tutte.h"
#include "adjacency_list.h"
#include "graph.h"
#include "utils.h"
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int node;
  int parent;
} BFSNode;

static int is_connected_without_cycle(AdjacencyList *adj_list, int *cycle,
                                      int cycle_len);
static int count_common_neighbors(int u, int v, AdjacencyList *adj);
static int find_cycle_perimeter(AdjacencyList *adj_list, int **cycle_out,
                                int *cycle_len_out);
static int find_cycle_bfs(AdjacencyList *adj_list, int **cycle_out,
                          int *cycle_len_out);
static int find_cycle(AdjacencyList *adj_list, int **cycle, int *cycle_len);
static void place_cycle_on_square(Graph *graph, int *cycle, int cycle_len,
                                  double size);
static void handle_graph_below_four_nodes(Graph *graph, double size);

int run_tutte(Graph *graph, double size, int max_iterations) {
  int status;
  if (graph->nodes_count < 4) {
    handle_graph_below_four_nodes(graph, size);
    return SUCCESS;
  }

  AdjacencyList *adj_list;
  status = create_adjacency_list(graph, &adj_list);
  if (status != SUCCESS) {
    fprintf(stderr, "Mamory allocation failed\n");
    return status;
  }

  int *cycle = NULL;
  int cycle_len = 0;

  status = find_cycle(adj_list, &cycle, &cycle_len);
  if (status != SUCCESS) {
    free_adjacency_list(adj_list);
    return status;
  }

  place_cycle_on_square(graph, cycle, cycle_len, size);

  bool *is_cycle = (bool *)calloc(graph->nodes_count, sizeof(bool));
  if (is_cycle == NULL) {
    free(cycle);
    free_adjacency_list(adj_list);
    fprintf(stderr, "Memory allocation failed\n");
    return MEMORY_ERROR;
  }

  for (int i = 0; i < cycle_len; i++) {
    is_cycle[cycle[i]] = true;
  }

  for (int i = 0; i < graph->nodes_count; i++) {
    if (!is_cycle[i]) {
      graph->nodes[i].x = size / 2.0;
      graph->nodes[i].y = size / 2.0;
    }
  }

  const double epsilon = 0.0001;
  for (int iter = 0; iter < max_iterations; iter++) {
    double max_displacement = 0.0;

    for (int i = 0; i < graph->nodes_count; i++) {
      if (is_cycle[i])
        continue;

      double sum_weighted_x = 0.0;
      double sum_weighted_y = 0.0;
      double sum_weights = 0.0;

      Neighbor *neighbor = adj_list->adjacency_list[i];
      while (neighbor != NULL) {
        int neighbor_idx = neighbor->node_index;
        double weight = neighbor->weight;

        sum_weighted_x += weight * graph->nodes[neighbor_idx].x;
        sum_weighted_y += weight * graph->nodes[neighbor_idx].y;
        sum_weights += weight;

        neighbor = neighbor->next;
      }

      if (sum_weights > 0.0) {
        double new_x = sum_weighted_x / sum_weights;
        double new_y = sum_weighted_y / sum_weights;

        double dx = new_x - graph->nodes[i].x;
        double dy = new_y - graph->nodes[i].y;
        double displacement = sqrt(dx * dx + dy * dy);

        if (displacement > max_displacement)
          max_displacement = displacement;

        graph->nodes[i].x = new_x;
        graph->nodes[i].y = new_y;
      }
    }
    if (max_displacement < epsilon)
      break;
  }

  free(is_cycle);
  free(cycle);
  free_adjacency_list(adj_list);
  return SUCCESS;
}

static int find_cycle(AdjacencyList *adj_list, int **cycle, int *cycle_len) {
  if (find_cycle_perimeter(adj_list, cycle, cycle_len) == SUCCESS) {
    return SUCCESS;
  }
  return find_cycle_bfs(adj_list, cycle, cycle_len);
}

static int find_cycle_perimeter(AdjacencyList *adj_list, int **cycle_out,
                                int *cycle_len_out) {
  int n = adj_list->nodes_count;
  int start_node = 0;
  int min_deg = adj_list->degrees[0];
  for (int i = 1; i < n; i++) {
    if (adj_list->degrees[i] < min_deg) {
      min_deg = adj_list->degrees[i];
      start_node = i;
    }
  }

  int *path = (int *)malloc(n * sizeof(int));
  if (!path)
    return MEMORY_ERROR;

  int current = start_node, previous = -1, count = 0;
  path[count++] = current;

  while (count < n) {
    int best_next = -1, min_common = INT_MAX, min_deg_val = INT_MAX;
    bool found = false;

    if (count > 4) {
      Neighbor *check = adj_list->adjacency_list[current];
      while (check) {
        if (check->node_index == start_node) {
          *cycle_out = path;
          *cycle_len_out = count;
          return SUCCESS;
        }
        check = check->next;
      }
    }

    Neighbor *neighbor = adj_list->adjacency_list[current];
    while (neighbor != NULL) {
      int v = neighbor->node_index;
      bool visited = false;
      for (int j = 0; j < count; j++)
        if (path[j] == v)
          visited = true;

      if (v != previous && !visited) {
        int common = count_common_neighbors(current, v, adj_list);
        int deg = adj_list->degrees[v];
        if (common < min_common ||
            (common == min_common && deg < min_deg_val)) {
          min_common = common;
          min_deg_val = deg;
          best_next = v;
          found = true;
        }
      }
      neighbor = neighbor->next;
    }
    if (!found)
      break;
    previous = current;
    current = best_next;
    path[count++] = current;
  }
  free(path);
  return ALGORITHM_ERROR;
}

static int find_cycle_bfs(AdjacencyList *adj_list, int **cycle_out,
                          int *cycle_len_out) {
  int n = adj_list->nodes_count;
  for (int start = 0; start < n; start++) {
    int *parent = (int *)malloc(n * sizeof(int));
    int *dist = (int *)malloc(n * sizeof(int));
    BFSNode *queue = (BFSNode *)malloc(n * sizeof(BFSNode));
    if (!parent || !dist || !queue) {
      fprintf(stderr, "Memory allocation failed\n");
      return MEMORY_ERROR;
    }

    for (int i = 0; i < n; i++) {
      parent[i] = -1;
      dist[i] = -1;
    }

    int front = 0, rear = 0;
    queue[rear++] = (BFSNode){start, -1};
    dist[start] = 0;

    while (front < rear) {
      int u = queue[front++].node;
      Neighbor *neighbor = adj_list->adjacency_list[u];
      while (neighbor != NULL) {
        int v = neighbor->node_index;
        if (dist[v] == -1) {
          dist[v] = dist[u] + 1;
          parent[v] = u;
          queue[rear++] = (BFSNode){v, u};
        } else if (parent[u] != v && dist[u] + dist[v] + 1 >= 3) {
          int len = dist[u] + dist[v] + 1;
          int *temp_cycle = (int *)malloc(len * sizeof(int));
          int pos = 0, curr = u;
          while (curr != -1) {
            temp_cycle[pos++] = curr;
            curr = parent[curr];
          }
          for (int i = 0; i < pos / 2; i++) {
            int tmp = temp_cycle[i];
            temp_cycle[i] = temp_cycle[pos - 1 - i];
            temp_cycle[pos - 1 - i] = tmp;
          }
          curr = v;
          while (curr != start) {
            temp_cycle[pos++] = curr;
            curr = parent[curr];
          }

          if (is_connected_without_cycle(adj_list, temp_cycle, len)) {
            *cycle_out = temp_cycle;
            *cycle_len_out = len;
            free(parent);
            free(dist);
            free(queue);
            return SUCCESS;
          }
          free(temp_cycle);
        }
        neighbor = neighbor->next;
      }
    }
    free(parent);
    free(dist);
    free(queue);
  }
  return ALGORITHM_ERROR;
}

static int is_connected_without_cycle(AdjacencyList *adj_list, int *cycle,
                                      int cycle_len) {
  int n = adj_list->nodes_count;
  int *in_cycle = (int *)calloc(n, sizeof(int));
  int *visited = (int *)calloc(n, sizeof(int));
  int *queue = (int *)malloc(n * sizeof(int));
  for (int i = 0; i < cycle_len; i++)
    in_cycle[cycle[i]] = 1;

  int start = -1;
  for (int i = 0; i < n; i++)
    if (!in_cycle[i]) {
      start = i;
      break;
    }
  if (start == -1) {
    free(in_cycle);
    free(visited);
    free(queue);
    return 1;
  }

  int front = 0, rear = 0;
  queue[rear++] = start;
  visited[start] = 1;
  int count = 1;

  while (front < rear) {
    int u = queue[front++];
    Neighbor *neighbor = adj_list->adjacency_list[u];
    while (neighbor != NULL) {
      int v = neighbor->node_index;
      if (!in_cycle[v] && !visited[v]) {
        visited[v] = 1;
        queue[rear++] = v;
        count++;
      }
      neighbor = neighbor->next;
    }
  }
  bool result = (count == n - cycle_len);
  free(in_cycle);
  free(visited);
  free(queue);
  return result;
}

static int count_common_neighbors(int u, int v, AdjacencyList *adj) {
  int common = 0;
  Neighbor *nu = adj->adjacency_list[u];
  while (nu != NULL) {
    Neighbor *nv = adj->adjacency_list[v];
    while (nv != NULL) {
      if (nu->node_index == nv->node_index)
        common++;
      nv = nv->next;
    }
    nu = nu->next;
  }
  return common;
}

static void place_cycle_on_square(Graph *graph, int *cycle, int cycle_len,
                                  double size) {
  double perimeter = 4.0 * size;
  double step = perimeter / cycle_len;
  for (int i = 0; i < cycle_len; i++) {
    double pos = i * step;
    double x, y;
    if (pos < size) {
      x = pos;
      y = 0.0;
    } else if (pos < 2.0 * size) {
      x = size;
      y = pos - size;
    } else if (pos < 3.0 * size) {
      x = size - (pos - 2.0 * size);
      y = size;
    } else {
      x = 0.0;
      y = size - (pos - 3.0 * size);
    }
    graph->nodes[cycle[i]].x = x;
    graph->nodes[cycle[i]].y = y;
  }
}

static void handle_graph_below_four_nodes(Graph *graph, double size) {
  if (graph->nodes_count == 1) {
    graph->nodes[0].x = size / 2.0;
    graph->nodes[0].y = size / 2.0;
  } else if (graph->nodes_count == 2) {
    graph->nodes[0].x = 0.0;
    graph->nodes[0].y = 0.0;
    graph->nodes[1].x = size;
    graph->nodes[1].y = size;
  } else if (graph->nodes_count == 3) {
    graph->nodes[0].x = 0.0;
    graph->nodes[0].y = 0.0;
    graph->nodes[1].x = size;
    graph->nodes[1].y = 0.0;
    graph->nodes[2].x = size / 2.0;
    graph->nodes[2].y = size;
  }
}
