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


Graph* load_graph(char path[]);
int get_node_index(Graph *graph, int node_id);
void save_graph_as_binary(Graph *graph, char path[]);
void save_graph_as_text(Graph *graph, char path[]);
void free_graph(Graph *graph);

#endif
