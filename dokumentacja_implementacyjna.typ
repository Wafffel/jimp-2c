#set page(
  paper: "a4",
  margin: (x: 2.5cm, y: 2.5cm),
  numbering: "1",
)

#set text(
  lang: "pl",
  size: 11pt,
)

#set heading(
  numbering: "1.1",
)

#show heading.where(level: 1): it => {
  pagebreak(weak: true)
  v(0.2cm)
  it
  v(0.15cm)
}

#show heading.where(level: 2): it => {
  v(0.15cm)
  it
  v(0.1cm)
}

#set par(justify: true, leading: 0.65em)

#align(center)[
  #v(3cm)
  #block(text(weight: 700, size: 20pt)[
    Dokumentacja implementacyjna
  ])
  #v(1cm)
  #block(text(size: 16pt)[
    System wizualizacji grafow planarnych
  ])
  #v(0.5cm)
  #block(text(size: 14pt)[
    Projekt w jezyku C
  ])
  #v(2cm)
  #block(text(size: 12pt)[
    Autorzy:\
    Krzysztof Wasilewski, Jakub Pietrzkiewicz\
    #v(0.3cm)
    Data: #datetime.today().display("[day].[month].[year]")
  ])
]

#pagebreak()

#outline()

#pagebreak()

= Wstep

== Cel dokumentu

Celem dokumentacji implementacyjnej jest przedstawienie szczegolow budowy aplikacji konsolowej w jezyku C do wyznaczania wspolrzednych wezlow grafu planarnego. Dokument opisuje rzeczywista strukture kodu, przeplyw sterowania, formaty danych wejscia i wyjscia oraz obsluge bledow.

== Zakres

Dokument obejmuje:

- architekture modulowa programu,
- model danych i struktury pamieci,
- implementacje algorytmow Fruchterman-Reingold i Tutte,
- specyfikacje interfejsow wejscia/wyjscia,
- kody powrotu i scenariusze bledowe,
- sposob budowania i uruchamiania testow.

= Architektura systemu

== Podzial na moduly

Implementacja projektu zostala podzielona na nastepujace moduly:

- `main` - parsowanie argumentow CLI i orkiestracja calego przebiegu programu,
- `graph` - odczyt i zapis grafu oraz mapowanie identyfikatorow wezlow,
- `fruchterman` - obliczanie ukladu metoda silowa,
- `tutte` - obliczanie osadzenia w oparciu o iteracyjna relaksacje,
- `adjacency_list` - konwersja grafu do list sasiedztwa dla algorytmu Tutte,
- `utils` - pomocnicza lista jednokierunkowa i wspolne kody statusu.

Kazdy modul ma dedykowany plik naglowkowy (`.h`) zawierajacy publiczny interfejs funkcji oraz plik implementacyjny (`.c`) z logika wewnetrzna.

== Przeplyw wykonania

Podstawowy przebieg programu:

1. Parsowanie argumentow i ustawienie wartosci domyslnych.
2. Walidacja parametrow (`iterations > 0`, `temperature > 0`, `size > 0`).
3. Wczytanie grafu z pliku tekstowego (`load_graph`).
4. Wykonanie wybranego algorytmu (`run_fruchterman` lub `run_tutte`).
5. Zapis wynikow (`save_graph_as_text` albo `save_graph_as_binary`).
6. Zwolnienie pamieci (`free_graph`) i zwrocenie kodu statusu.

Wszystkie bledy propagowane sa przez kody zwracane funkcji.

Fragment implementacji walidacji argumentow wejsciowych:

```c
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
```

Fragment implementacji odpowiedzialny za wybor algorytmu i formatu zapisu:

```c
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
```

= Model danych

== Struktury podstawowe grafu

W module `graph` uzywane sa trzy glowne struktury:

- `Node`
  - `id` - identyfikator logiczny wezla,
  - `x`, `y` - wspolrzedne geometryczne.
- `Edge`
  - `first_node_index`, `second_node_index` - indeksy w tablicy `nodes`,
  - `weight` - waga krawedzi,
  - `label[33]` - etykieta krawedzi, maksymalnie 32 znaki + `\0`.
- `Graph`
  - `nodes` - dynamiczna tablica wezlow,
  - `nodes_count` - liczba wezlow,
  - `edges` - dynamiczna tablica krawedzi,
  - `edges_count` - liczba krawedzi.

== Struktury pomocnicze

Dodatkowe struktury wykorzystywane przez algorytmy:

- `List` (modul `utils`) - jednokierunkowa lista do budowy zbioru unikalnych ID przy odczycie grafu,
- `Neighbor` (modul `adjacency_list`) - pojedynczy element listy sasiadow,
- `AdjacencyList` - tablica list sasiadow i tablica stopni wezlow.

Fragment implementacji budowy listy sasiedztwa:

```c
for (int i = 0; i < graph->edges_count; i++) {
  int first_index = graph->edges[i].first_node_index;
  int second_index = graph->edges[i].second_node_index;
  double weight = graph->edges[i].weight;

  if (add_neighbor(&adj_list->adjacency_list[first_index], second_index,
                   weight) != SUCCESS ||
      add_neighbor(&adj_list->adjacency_list[second_index], first_index,
                   weight) != SUCCESS) {
    free_adjacency_list(adj_list);
    return MEMORY_ERROR;
  }
  adj_list->degrees[first_index]++;
  adj_list->degrees[second_index]++;
}
```

= Interfejsy modulow

== Interfejs funkcji modulu graph

Funkcje publiczne:

- `int load_graph(char path[], Graph **graph_out)`
- `int save_graph_as_binary(Graph *graph, char path[])`
- `int save_graph_as_text(Graph *graph, char path[])`
- `int free_graph(Graph *graph)`
- `int get_node_index(Graph *graph, int node_id)`

Wczytywanie grafu realizowane jest w dwoch przebiegach przez plik:

- przebieg 1: zliczenie krawedzi i identyfikacja unikalnych wierzcholkow,
- przebieg 2: wlasciwe mapowanie krawedzi na indeksy tablicowe.

Wezly sa sortowane po `id` i wyszukiwane binarnie (`bsearch`), co przyspiesza mapowanie identyfikatorow.

Fragment implementacji mapowania identyfikatora wezla na indeks tablicy:

```c
int get_node_index(Graph *graph, int node_id) {
  Node key = {.id = node_id};
  Node *result = (Node *)bsearch(&key, graph->nodes, graph->nodes_count,
                                 sizeof(Node), compare_nodes_by_id);
  return (result == NULL) ? -1 : (int)(result - graph->nodes);
}
```

== Interfejs funkcji modulu fruchterman

- `int run_fruchterman(Graph *graph, double initial_temperature, int max_iterations, double size)`

== Interfejs funkcji modulu tutte

- `int run_tutte(Graph *graph, double size, int max_iterations)`

== Interfejs funkcji modulu adjacency_list

- `int create_adjacency_list(Graph *graph, AdjacencyList **adj_list_out)`
- `void free_adjacency_list(AdjacencyList *adj_list)`

== Interfejs funkcji modulu utils

- `int list_contains(List *head, int value)`
- `int list_prepend(List **head, int value)`
- `void list_free(List *head)`

Modul zawiera rowniez wspolne kody statusu:

- `SUCCESS = 0`
- `ARGUMENTS_ERROR = 1`
- `FILE_ERROR = 2`
- `INPUT_FORMAT_ERROR = 3`
- `ALGORITHM_ERROR = 4`
- `MEMORY_ERROR = 5`

= Specyfikacja danych wejsciowych i wyjsciowych

== Format wejsciowy

Program przyjmuje tekstowy plik wejsciowy o rekordach:

```
<label> <nodeA> <nodeB> <weight>
```

Szczegoly parsowania:

- `label` jest wczytywany przez `%32s` (maksymalnie 32 znaki),
- `nodeA`, `nodeB` sa typu `int`,
- `weight` jest typu `double`,
- linie komentarza rozpoczynajace sie od `#` sa pomijane,
- komentarze inline po danych rowniez sa poprawnie pomijane,
- biale znaki (`space`, `tab`, `CR`, `LF`) sa tolerowane.

Fragment implementacji odczytu linii wejsciowej:

```c
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
  return INPUT_FORMAT_ERROR;
}
```

== Format wyjsciowy tekstowy

Kazdy rekord opisuje pojedynczy wezel:

```
<node_id> <x> <y>
```

Fragment implementacji zapisu tekstowego:

```c
for (int i = 0; i < graph->nodes_count; i++) {
  fprintf(file, "%d %lf %lf\n", graph->nodes[i].id,
          graph->nodes[i].x, graph->nodes[i].y);
}
```

== Format wyjsciowy binarny

Plik binarny to ciag rekordow stalej dlugosci 20 bajtow:

- 4 bajty: `int id`,
- 8 bajtow: `double x`,
- 8 bajtow: `double y`.

Fragment implementacji zapisu binarnego:

```c
for (int i = 0; i < graph->nodes_count; i++) {
  fwrite(&graph->nodes[i].id, sizeof(int), 1, file);
  fwrite(&graph->nodes[i].x, sizeof(double), 1, file);
  fwrite(&graph->nodes[i].y, sizeof(double), 1, file);
}
```

= Implementacja algorytmow

== Fruchterman-Reingold

Algorytm uzywa modelu silowego:

- sila odpychania miedzy kazda para wezlow,
- sila przyciagania dla wezlow polaczonych krawedzia,
- ograniczenie pozycji do kwadratu o boku `size`.

Wzory uzyte w kodzie:

- $k = sqrt(frac("area", |V|))$ gdzie $"area" = "size" dot "size"$
- $f_r = frac(k^2, d)$
- $f_a = w dot frac(d^2, k)$

Fragment implementacji obliczania sil odpychania:

```c
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
```

Fragment implementacji aktualizacji pozycji i ograniczenia do obszaru:

```c
for (int i = 0; i < liczba_w; i++) {
  double wek_sily = sqrt(sila_x[i] * sila_x[i] + sila_y[i] * sila_y[i]);
  if (wek_sily > 0) {
    double przesuniecie = fmin(wek_sily, temperature);
    graph->nodes[i].x += (sila_x[i] / wek_sily) * przesuniecie;
    graph->nodes[i].y += (sila_y[i] / wek_sily) * przesuniecie;
  }

  if (graph->nodes[i].x < 0.0)
    graph->nodes[i].x = 0.0;
  if (graph->nodes[i].x > size)
    graph->nodes[i].x = size;
  if (graph->nodes[i].y < 0.0)
    graph->nodes[i].y = 0.0;
  if (graph->nodes[i].y > size)
    graph->nodes[i].y = size;
}
```

== Tutte

Implementacja Tutte przebiega etapami:

1. Dla grafow z mniej niz 4 wezlami stosowane sa pozycje specjalne.
2. Dla wiekszych grafow tworzona jest lista sasiedztwa.
3. Wyznaczany jest cykl brzegowy.
4. Wezly cyklu rozmieszczane sa rownomiernie na obwodzie kwadratu.
5. Wezly wewnetrzne inicjalizowane sa w srodku obszaru.
6. Iteracyjna relaksacja przesuwa kazdy wezel wewnetrzny do wazonego srodka sasiadow.

W praktyce wyznaczenie ramy odbywa sie dwuetapowo:

- najpierw funkcja `find_cycle_perimeter` probuje znalezc cykl brzegowy heurystycznie,
- jezeli to sie nie uda, `find_cycle_backup` wybiera 4 wezly o najwyzszych stopniach jako rame awaryjna.

Heurystyka startuje od wezla o najmniejszym stopniu i buduje sciezke po sasiadach. Preferowany jest kandydat z mniejsza liczba wspolnych sasiadow z wezlem biezacym (a przy remisie z mniejszym stopniem). Po domknieciu sciezki do wezla startowego otrzymujemy cykl, ktory jest rozmieszczany na obwodzie kwadratu.

Fragment implementacji przelaczania miedzy wariantem podstawowym i awaryjnym:

```c
static int find_cycle(AdjacencyList *adj_list, int **cycle, int *cycle_len) {
  return (find_cycle_perimeter(adj_list, cycle, cycle_len) == SUCCESS)
             ? SUCCESS
             : find_cycle_backup(adj_list, cycle, cycle_len);
}
```

Fragment implementacji pojedynczego kroku relaksacji:

```c
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
  graph->nodes[i].x = sum_weighted_x / sum_weights;
  graph->nodes[i].y = sum_weighted_y / sum_weights;
}
```

Fragment implementacji rozmieszczenia cyklu na obwodzie kwadratu:

```c
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
```

= Zarzadzanie pamiecia

== Zwalnianie

Pamiec jest zwalniana warstwowo: najpierw struktury zagniezdzone (listy sasiadow), potem tablice i na koncu obiekt nadrzedny. To ogranicza ryzyko wyciekow i podwojnego zwalniania.

W przypadku bledow alokacji program od razu przerywa dalsze kroki i wykonuje cleanup tylko dla zasobow, ktore zostaly juz poprawnie utworzone.

Fragment implementacji zwalniania struktury grafu:

```c
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
```

Fragment implementacji zwalniania listy sasiedztwa:

```c
for (int i = 0; i < adj_list->nodes_count; i++) {
  Neighbor *current = adj_list->adjacency_list[i];
  while (current != NULL) {
    Neighbor *next = current->next;
    free(current);
    current = next;
  }
}
free(adj_list->adjacency_list);
free(adj_list->degrees);
free(adj_list);
```

= Obsluga bledow

== Komunikaty i kody powrotu

Program raportuje bledy na `stderr` i konczy dzialanie kodem:

- `0` - sukces,
- `1` - blad argumentow/parametrow,
- `2` - blad operacji plikowych,
- `3` - blad formatu danych wejsciowych,
- `4` - blad algorytmu,
- `5` - blad alokacji pamieci.

Typowe przypadki bledow:

- niepoprawna flaga lub brak plikow wejsciowego/wyjsciowego -> `ARGUMENTS_ERROR`,
- nieudane otwarcie pliku -> `FILE_ERROR`,
- bledny rekord w pliku grafu -> `INPUT_FORMAT_ERROR`,
- brak pamieci podczas tworzenia struktur pomocniczych -> `MEMORY_ERROR`.

= Kompilacja i testowanie

== Kompilacja

Projekt wykorzystuje `Makefile`:

- `make` - buduje aplikacje glowna do `bin/graph`,
- `make clean` - usuwa katalog `bin`,
- `make test` - buduje i uruchamia testy jednostkowe.

== Testy

Zestaw testow obejmuje m.in. odczyt grafu, mapowanie identyfikatorow, dzialanie list sasiedztwa oraz podstawowe przypadki dla obu algorytmow ukladania.
