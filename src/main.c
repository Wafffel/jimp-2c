#include "fruchterman.h"
#include "graph.h"
#include "tutte.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Domyślne wartości parametrów
  char *input_file = NULL;
  char *output_file = NULL;
  char *algorithm = "fruchterman";
  char *format = "text";
  int iterations = 1000;
  double temperature = 10.0;
  double size = 1000.0;

  // Parsowanie argumentów wiersza poleceń
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--algorithm") == 0) {
      if (++i < argc)
        algorithm = argv[i];
    } else if (strcmp(argv[i], "-i") == 0 ||
               strcmp(argv[i], "--iterations") == 0) {
      if (++i < argc)
        iterations = atoi(argv[i]);
    } else if (strcmp(argv[i], "-t") == 0 ||
               strcmp(argv[i], "--temperature") == 0) {
      if (++i < argc)
        temperature = atof(argv[i]);
    } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--size") == 0) {
      if (++i < argc)
        size = atof(argv[i]);
    } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--format") == 0) {
      if (++i < argc)
        format = argv[i];
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      printf(
          "=== Graph Layout Tool ===\n"
          "Użycie programu: ./graph [opcje] <input_file> <output_file>\n\n"
          "Opcje:\n"
          "  -a, --algorithm <algorytm>    Wybór algorytmu (fruchterman, "
          "tutte)\n"
          "  -i, --iterations <liczba>     Liczba iteracji (domyślnie: 1000)\n"
          "  -t, --temperature <liczba>    Początkowa temperatura (domyślnie: "
          "10.0)\n"
          "  -s, --size <liczba>           Rozmiar obszaru layoutu (domyślnie: "
          "1000.0)\n"
          "  -f, --format <format>         Format wyjściowy (text, binary)\n"
          "  -h, --help                    Wyświetlenie tej pomocy\n");
      return SUCCESS;
    } else if (input_file == NULL && i == argc - 2) {
      input_file = argv[i];
    } else if (output_file == NULL && i == argc - 1) {
      output_file = argv[i];
    } else {
      fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
      return ARGUMENTS_ERROR;
    }
  }

  if (input_file == NULL || output_file == NULL) {
    fprintf(stderr, "Error: Input and output files must be specified.\n");
    return ARGUMENTS_ERROR;
  }
  if (iterations <= 0) {
    fprintf(stderr, "Error: Invalid iteration value\n");
    return ARGUMENTS_ERROR;
  }
  if (temperature <= 0) {
    fprintf(stderr, "Error: Invalid temperature value\n");
    return ARGUMENTS_ERROR;
  }
  if (size <= 0) {
    fprintf(stderr, "Error: Invalid size value\n");
    return ARGUMENTS_ERROR;
  }

  Graph *graph = NULL;
  int status = load_graph(input_file, &graph);
  if (status != SUCCESS) {
    return status;
  }

  // Uruchomienie wybranego algorytmu layoutu grafu
  if (strcmp(algorithm, "fruchterman") == 0) {
    status = run_fruchterman(graph, temperature, iterations, size);
  } else if (strcmp(algorithm, "tutte") == 0) {
    status = run_tutte(graph, size, iterations);
  } else {
    fprintf(stderr,
            "Error: Unknown algorithm '%s'. Use 'fruchterman' or 'tutte'.\n",
            algorithm);
    free_graph(graph);
    return ARGUMENTS_ERROR;
  }
  if (status != SUCCESS) {
    free_graph(graph);
    return status;
  }

  if (strcmp(format, "text") == 0) {
    status = save_graph_as_text(graph, output_file);
  } else if (strcmp(format, "binary") == 0) {
    status = save_graph_as_binary(graph, output_file);
  } else {
    fprintf(stderr, "Error: Unknown format '%s'. Use 'text' or 'binary'.\n",
            format);
    free_graph(graph);
    return ARGUMENTS_ERROR;
  }
  free_graph(graph);
  return status;
}
