#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
    int id;
    double x, y;
} Node;

typedef struct {
    int first_node_index; int second_node_index;
    double weight;
    char label[33];
} Edge;

typedef struct {
    Node* nodes;
    int nodes_count;
    Edge* edges;
    int edges_count;
} Graph;


void save_graph_as_binary();
void save_graph_as_text();
void load_graph();

#endif
