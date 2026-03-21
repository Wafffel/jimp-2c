#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "fruchterman.h"
#include "tutte.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    // Domyślne wartości parametrów
    char *input_file = NULL;
    char *output_file = NULL;
    char *algorithm = "fruchterman";
    char *format = "tekst";
    int iterations = 1000;
    double temperature = 10.0;
    double size = 1000.0;

    // Argument parsing here (2)
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--algorithm") == 0)
        {
            if (i + 1 < argc)
                algorithm = argv[i + 1];
        }
        else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--iterations") == 0)
        {
            if (i + 1 < argc && atoi(argv[i + 1]) > 0)
                iterations = atoi(argv[i + 1]);
            else if (i + 1 < argc && atoi(argv[i + 1] <= 0))
            {
                fprintf(stderr, "Error: Iterations must be a positive integer.\n");
                return ARGUMENTS_ERROR;
            }
        }
        else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--temperature") == 0)
        {
            if (i + 1 < argc && atof(argv[i + 1]) > 0)
                temperature = atof(argv[i + 1]);
            else if (i + 1 < argc && atof(argv[i + 1]) <= 0)
            {
                fprintf(stderr, "Error: Temperature must be a positive number.\n");
                return ARGUMENTS_ERROR;
            }
        }
        else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--size") == 0)
        {
            if (i + 1 < argc && atof(argv[i + 1]) > 0)
                size = atof(argv[i + 1]);
            else if (i + 1 < argc && atof(argv[i + 1]) <= 0)
            {
                fprintf(stderr, "Error: Size must be a positive number.\n");
                return ARGUMENTS_ERROR;
            }
        }
        else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--format") == 0)
        {
            if (i + 1 < argc)
                format = argv[i + 1];
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            printf("=== Graph Layout Tool ===\n");
            printf("Użycie programu: ./graph [opcje] <input_file> <output_file>\n\n");
            printf("Opcje: \n");
            printf("  -a, --algorithm <algorytm>    Wybór algorytmu (fruchterman, tutte)\n");
            printf("  -i, --iterations <liczba>     Liczba iteracji (domyślnie: 1000)\n");
            printf("  -t, --temperature <liczba>    Początkowa temperatura (domyślnie: 10.0)\n");
            printf("  -s, --size <liczba>           Rozmiar obszaru layoutu (domyślnie: 1000.0)\n");
            printf("  -f, --format <format>         Format wyjściowy (tekst, binarny)\n");
            printf("  -h, --help                    Wyświetlenie tej pomocy\n");
            return SUCCESS;
        }
        else
        {
            if (input_file == NULL)
                input_file = argv[i];
            else if (output_file == NULL)
                output_file = argv[i];
        }
    }

    if (input_file == NULL || output_file == NULL)
    {
        fprintf(stderr, "Error: Input and output files must be specified.\n");
        return ARGUMENTS_ERROR;
    }

    // Initialize and load graph
    Graph *graph = NULL;
    int status = load_graph(input_file, &graph);
    if (status != SUCCESS)
    {
        return status;
    }
    // Run the selected graph layout algorithm (Tutte or Fruchterman-Reingold)
    if (strcmp(algorithm, "fruchterman") == 0)
    {
        status = run_fruchterman(graph, temperature, iterations, size);
    }
    else if (strcmp(algorithm, "tutte") == 0)
    {
        status = run_tutte(graph, size, iterations);
    }
    else
    {
        fprintf(stderr, "Error: Unknown algorithm '%s'. Use 'fruchterman' or 'tutte'.\n", algorithm);
        free_graph(graph);
        return ARGUMENTS_ERROR;
    }
    if (status != SUCCESS)
    {
        free_graph(graph);
        return status;
    }

    // Save the resulting graph layout
    if (strcmp(format, "text") == 0)
    {
        status = save_graph_as_text(graph, output_file);
    }
    else if (strcmp(format, "binary") == 0)
    {
        status = save_graph_as_binary(graph, output_file);
    }
    else
    {
        fprintf(stderr, "Error: Unknown format '%s'. Use 'text' or 'binary'.\n", format);
        free_graph(graph);
        return ARGUMENTS_ERROR;
    }
    // Free allocated memory
    free_graph(graph);
    return status;
}
