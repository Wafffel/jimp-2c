#include "test_fruchterman.h"
#include "../src/fruchterman.h"
#include "../src/graph.h"
#include "../src/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// pusty graf
void test_fruchterman_empty_graph()
{
    printf("Testing Fruchterman with empty graph...\n");
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->nodes = NULL;
    graph->nodes_count = 0;
    graph->edges = NULL;
    graph->edges_count = 0;

    int result = run_fruchterman(graph, 10.0, 1000, 1000.0);

    assert(result == SUCCESS);
    free(graph);
    printf("PASSED\n");
}
// mały graf
void test_fruchterman_small_graph()
{
    printf("Testing Fruchterman with small graph...\n");
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->nodes_count = 3;
    graph->edges_count = 2;
    graph->nodes = (Node *)malloc(graph->nodes_count * sizeof(Node));
    graph->edges = (Edge *)malloc(graph->edges_count * sizeof(Edge));

    // Inicjalizacja wierzchołków
    for (int i = 0; i < graph->nodes_count; i++)
    {
        graph->nodes[i].id = i + 1;
        graph->nodes[i].x = 0.0;
        graph->nodes[i].y = 0.0;
    }

    // Inicjalizacja krawędzi
    graph->edges[0].first_node_index = 0;
    graph->edges[0].second_node_index = 1;
    graph->edges[0].weight = 1.0;

    graph->edges[1].first_node_index = 1;
    graph->edges[1].second_node_index = 2;
    graph->edges[1].weight = 1.0;

    int result = run_fruchterman(graph, 10.0, 1000, 1000.0);

    assert(result == SUCCESS);

    for (int i = 0; i < graph->nodes_count; i++)
    {
        assert(graph->nodes[i].x >= 0.0 && graph->nodes[i].x <= 1000.0);
        assert(graph->nodes[i].y >= 0.0 && graph->nodes[i].y <= 1000.0);
    }

    free(graph->nodes);
    free(graph->edges);
    free(graph);
    printf("PASSED\n");
}