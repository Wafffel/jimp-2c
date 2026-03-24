#include "fruchterman.h"
#include "graph.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double random_double(double min, double max) {
  return min + (max - min) * ((double)rand() / RAND_MAX);
}

// Algorytm Fruchtermana-Reingolda: model sił fizycznych do wizualizacji grafów
int run_fruchterman(Graph *graph, double initial_temperature,
                    int max_iterations, double size) {
  if (graph == NULL || graph->nodes_count <= 0) {
    return SUCCESS;
  }
  int liczba_w = graph->nodes_count;
  int liczba_k = graph->edges_count;
  if (liczba_w == 0 || liczba_k == 0) {
    fprintf(stderr, "Error: Graph must have at least one node and one edge.\n");
    return ARGUMENTS_ERROR;
  }
  double area = size * size;
  // k - optymalna odległość między wierzchołkami
  double k = sqrt(area / liczba_w);
  double temperature = initial_temperature;
  srand(time(NULL));

  // Losowa inicjalizacja pozycji wierzchołków
  for (int i = 0; i < liczba_w; i++) {
    graph->nodes[i].x = random_double(0.0, size);
    graph->nodes[i].y = random_double(0.0, size);
  }

  double *sila_x = (double *)calloc(liczba_w, sizeof(double));
  double *sila_y = (double *)calloc(liczba_w, sizeof(double));
  if (sila_x == NULL || sila_y == NULL) {
    fprintf(stderr, "Error: Memory allocation failed.\n");
    free(sila_x);
    free(sila_y);
    return MEMORY_ERROR;
  }

  for (int iter = 0; iter < max_iterations; iter++) {
    for (int i = 0; i < liczba_w; i++) {
      sila_x[i] = 0.0;
      sila_y[i] = 0.0;
    }
    // Siły odpychania
    for (int a = 0; a < liczba_w; a++) {
      for (int b = 0; b < liczba_w; b++) {
        if (a != b) {
          double dx = graph->nodes[a].x - graph->nodes[b].x;
          double dy = graph->nodes[a].y - graph->nodes[b].y;
          double odleglosc = sqrt(dx * dx + dy * dy) + 1e-9;
          double fr = (k * k) / odleglosc;
          sila_x[a] += (dx / odleglosc) * fr;
          sila_y[a] += (dy / odleglosc) * fr;
        }
      }
    }
    // Siły przyciągania
    for (int e = 0; e < liczba_k; e++) {
      int a = graph->edges[e].first_node_index;
      int b = graph->edges[e].second_node_index;
      double waga = graph->edges[e].weight;
      double dx = graph->nodes[a].x - graph->nodes[b].x;
      double dy = graph->nodes[a].y - graph->nodes[b].y;
      double odleglosc = sqrt(dx * dx + dy * dy) + 1e-9;
      double fa = waga * (odleglosc * odleglosc) / k;
      sila_x[a] -= (dx / odleglosc) * fa;
      sila_y[a] -= (dy / odleglosc) * fa;
      sila_x[b] += (dx / odleglosc) * fa;
      sila_y[b] += (dy / odleglosc) * fa;
    }
    // Aktualizacja pozycji
    for (int i = 0; i < liczba_w; i++) {
      double wek_sily = sqrt(sila_x[i] * sila_x[i] + sila_y[i] * sila_y[i]);
      if (wek_sily > 0) {
        double przesuniecie = fmin(wek_sily, temperature);
        graph->nodes[i].x += (sila_x[i] / wek_sily) * przesuniecie;
        graph->nodes[i].y += (sila_y[i] / wek_sily) * przesuniecie;
      }
      // Ograniczenie do obszaru
      if (graph->nodes[i].x < 0.0)
        graph->nodes[i].x = 0.0;
      if (graph->nodes[i].x > size)
        graph->nodes[i].x = size;
      if (graph->nodes[i].y < 0.0)
        graph->nodes[i].y = 0.0;
      if (graph->nodes[i].y > size)
        graph->nodes[i].y = size;
    }
    // Chłodzenie
    temperature -= initial_temperature / max_iterations;
    if (temperature < 0.0) {
      temperature = 0.0;
    }
  }
  free(sila_x);
  free(sila_y);
  return SUCCESS;
}
