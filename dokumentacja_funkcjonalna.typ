#set page(
  paper: "a4",
  margin: (x: 2.5cm, y: 2.5cm),
  numbering: "1",
)

#set text(
  lang: "pl",
  size: 11pt,
)

#set heading(numbering: "1.1")

#align(center)[
  #v(3cm)
  #block(text(weight: 700, size: 20pt)[
    Dokumentacja funkcjonalna
  ])
  #v(1cm)
  #block(text(size: 16pt)[
    System wizualizacji grafów planarnych
  ])
  #v(0.5cm)
  #block(text(size: 14pt)[
    Projekt w języku C
  ])
  #v(2cm)
  #block(text(size: 12pt)[
    Autorzy:\
    #v(0.3cm)
    Data: #datetime.today().display("[day].[month].[year]")
  ])
]

#pagebreak()

#outline()

#pagebreak()

= Wstęp

== Cel projektu

Celem projektu jest stworzenie aplikacji konsolowej w języku C, która umożliwia wizualizację grafów planarnych poprzez wyznaczanie optymalnych współrzędnych dla ich węzłów. Program przyjmuje na wejściu graf opisany w postaci listy krawędzi i generuje plik z współrzędnymi węzłów, które pozwalają na czytelną wizualizację struktury grafu.

Aplikacja implementuje dwa różne algorytmy układania grafów, co umożliwia porównanie ich efektywności i jakości generowanych wizualizacji. Program jest sterowany z linii poleceń za pomocą argumentów i opcji, co zapewnia elastyczność i możliwość automatyzacji.

== Dostępne funkcjonalności

Program oferuje następujące funkcjonalności:

- Wczytywanie grafów planarnych z plików tekstowych w formacie listy krawędzi
- Wyznaczanie współrzędnych węzłów za pomocą wybranych algorytmów układania grafów
- Generowanie wyników w formacie tekstowym lub binarnym (do wyboru przez użytkownika)
- Obsługa dwóch algorytmów: Fruchterman-Reingold oraz Tutte embeddings
- Konfiguracja parametrów działania programu za pomocą opcji linii poleceń

= Instrukcja użytkowania

== Instalacja i kompilacja

Program wymaga kompilatora C (np. GCC), narzędzia make oraz standardowej biblioteki C. 

Kompilacja programu:
```bash
make
```

Czyszczenie plików kompilacji:
```bash
make clean
```

== Sposób uruchamiania

Program jest uruchamiany z linii poleceń. składnia wygląda następująco:

```bash
./graph [opcje] <plik_wejściowy> <plik_wyjściowy>
```

gdzie:
- `<plik_wejściowy>` - ścieżka do pliku z definicją grafu
- `<plik_wyjściowy>` - ścieżka do pliku wynikowego, gdzie program zapisze wynik działania

= Parametry i opcje uruchamiania

== Obowiązkowe argumenty

- `<plik_wejściowy>` - ścieżka do pliku tekstowego zawierającego definicję grafu w formacie listy krawędzi
- `<plik_wyjściowy>` - ścieżka do pliku, w którym zostaną zapisane wyniki (współrzędne węzłów)

== Opcjonalne argumenty

- `-a <algorytm>` lub `--algorithm <algorytm>` - wybór algorytmu układania grafu
  - Dostępne wartości: `fruchterman`, `tutte`
  - Domyślnie: `fruchterman`

- `-f <format>` lub `--format <format>` - wybór formatu pliku wyjściowego
  - Dostępne wartości: `text`, `binary`
  - Domyślnie: `text`

- `-i <iteracje>` lub `--iterations <iteracje>` - liczba iteracji dla algorytmów iteracyjnych (np. Fruchterman-Reingold)
  - Wartość: liczba całkowita dodatnia
  - Domyślnie: `1000`

// tu pewnie temperaatura

- `-h` lub `--help` - wyświetla pomoc i dostępne opcje

== Przykłady wywołania

Podstawowe wywołanie z domyślnymi parametrami:
```bash
./graph input.txt output.txt
```

Wybór algorytmu Tutte:
```bash
./graph -a tutte input.txt output.txt
```

Zapisanie wyniku w formacie binarnym:
```bash
./graph -f binary input.txt output.bin
```

Konfiguracja parametrów algorytmu Fruchterman-Reingold:
```bash
./graph -a fruchterman -i 2000 input.txt output.txt
```

Łączenie wielu opcji:
```bash
./graph --algorithm tutte --format binary input.txt output.bin
```

= Format danych

== Format pliku wejściowego

Plik wejściowy jest plikiem tekstowym zawierającym listę krawędzi grafu. Każda linia opisuje jedną krawędź w następującym formacie:

```
<nazwa_krawędzi> <wierzchołek_A> <wierzchołek_B> <waga_krawędzi>
```

gdzie:
- `<nazwa_krawędzi>` - etykieta krawędzi (ciąg znaków bez spacji)
- `<wierzchołek_A>` - identyfikator pierwszego wierzchołka (liczba całkowita dodatnia)
- `<wierzchołek_B>` - identyfikator drugiego wierzchołka (liczba całkowita dodatnia)
- `<waga_krawędzi>` - waga krawędzi (liczba zmiennoprzecinkowa)

Poszczególne pola są oddzielone spacjami lub tabulatorami. Puste linie oraz linie zaczynające się od znaku `#` są ignorowane (komentarze).

Przykład pliku wejściowego:
```
# Przykładowy graf - kwadrat
AB   1  2  1.0
BC   2  3  1.0
CD   3  4  1.0
DA   4  1  1.0
AC   1  3  1.414
```

== Format pliku wyjściowego tekstowego

Plik wyjściowy w formacie tekstowym zawiera listę współrzędnych węzłów. Każda linia opisuje pozycję jednego węzła:

```
<wierzchołek> <współrzędna_x> <współrzędna_y>
```

gdzie:
- `<wierzchołek>` - identyfikator wierzchołka (liczba całkowita)
- `<współrzędna_x>` - współrzędna X (liczba zmiennoprzecinkowa)
- `<współrzędna_y>` - współrzędna Y (liczba zmiennoprzecinkowa)

Przykład pliku wyjściowego:
```
1 0.0 0.0
2 1.0 0.0
3 1.0 1.0
4 0.0 1.0
```

== Format pliku wyjściowego binarnego

// to do zastanowienia

Plik wyjściowy w formacie binarnym zawiera te same dane co format tekstowy, ale zapisane w reprezentacji binarnej dla efektywniejszego przechowywania i szybszego wczytywania.

Struktura pliku binarnego:
1. Nagłówek (4 bajtów):
   - 4 bajty: liczba wierzchołków (int)

2. Dane wierzchołków (24 bajty na wierzchołek):
   - 4 bajty: identyfikator wierzchołka (int)
   - 8 bajtów: współrzędna X (double)
   - 8 bajtów: współrzędna Y (double)
   - 4 bajty: padding/zarezerwowane

Wszystkie wartości zapisane są w kolejności big-endian.

= Ograniczenia

// ograniczenia do zmiany pewnie 

== Ograniczenia techniczne

- Maksymalna liczba wierzchołków: 10 000
- Maksymalna liczba krawędzi: 50 000
- Identyfikatory wierzchołków: liczby całkowite dodatnie (1-2147483647)
- Długość nazw krawędzi: maksymalnie 64 znaki
- Maksymalna długość linii w pliku: 1024 znaki

== Ograniczenia algorytmiczne

- Algorytm Tutte wymaga grafu planarnego i 3-spójnego
- Nie obsługiwane są multigraf ani pętle własne
- Algorytm Fruchterman-Reingold: złożoność O(n² x iteracje) - dla grafów > 1000 węzłów zalecane mniejsze liczby iteracji
- Algorytm Tutte: złożoność O(n³)

== Ograniczenia systemowe

- Wymagany system UNIX/Linux/macOS (Windows wymaga MinGW/Cygwin)
- Wymagany kompilator zgodny ze standardem C99 lub nowszym
- Program wyznacza tylko współrzędne - wizualizacja wymaga dodatkowego narzędzia

= Opis algorytmów

// to do zostanowia 

== Algorytm Fruchterman-Reingold

Algorytm Fruchterman-Reingold jest jednym z najpopularniejszych algorytmów force-directed do układania grafów. Traktuje on graf jako system fizyczny, gdzie:
- Węzły są punktami masowymi, które się odpychają
- Krawędzie działają jak sprężyny, które przyciągają połączone węzły

Algorytm działa iteracyjnie:
1. Rozpoczyna od losowego rozmieszczenia węzłów
2. W każdej iteracji oblicza siły działające na każdy węzeł:
   - Siłę odpychającą od wszystkich innych węzłów
   - Siłę przyciągającą od węzłów połączonych krawędzią
3. Przemieszcza węzły zgodnie z wypadkową siłą
4. Stopniowo zmniejsza "temperaturę" systemu (wielkość możliwych przemieszczeń)

Parametry algorytmu:
- Liczba iteracji: określa jak długo algorytm będzie działał
- Temperatura początkowa: kontroluje początkową wielkość przemieszczeń
- Stała sprężystości: wpływa na odległość między węzłami

Zalety: dobra jakość wizualizacji, dostosowuje się do struktury grafu
Wady: może wymagać wielu iteracji dla dużych grafów

== Algorytm Tutte embeddings

Algorytm Tutte embeddings (znany również jako Tutte's spring theorem) to algebraiczna metoda układania grafów planarnych. Algorytm:
1. Wybiera węzły zewnętrzne (tworzące brzeg) i ustala je na obwodzie wypukłego wielokąta
2. Pozostałe węzły (wewnętrzne) umieszcza w pozycjach będących środkiem ciężkości ich sąsiadów
3. Rozwiązuje układ równań liniowych, aby znaleźć optymalne pozycje

Matematycznie, dla każdego węzła wewnętrznego `v`:
```
x(v) = (Σ x(u)) / deg(v)
y(v) = (Σ y(u)) / deg(v)
```
gdzie suma jest po wszystkich sąsiadach `u` węzła `v`.

Parametry algorytmu:
- Kształt wielokąta dla węzłów brzegowych (domyślnie: okrąg)
- Wybór węzłów brzegowych (domyślnie: automatyczny na podstawie struktury grafu)

Zalety: gwarantuje brak przecięć krawędzi dla grafów planarnych, szybki (rozwiązanie układu równań)
Wady: wymaga grafu planarnego i 3-spójnego dla optymalnych rezultatów

= Obsługa błędów 

== Komunikaty o błędach 

Program wyświetla komunikaty o błędach na standardowe wyjście błędów (stderr). Każdy komunikat zawiera opis problemu i sugestię rozwiązania.

Główne kategorie błędów:

*Błędy argumentów:*
- `Error: Invalid number of arguments` - nieprawidłowa liczba argumentów
- `Error: Unknown option: <opcja>` - nieznana opcja
- `Error: Invalid algorithm name: <nazwa>` - nieprawidłowa nazwa algorytmu
- `Error: Invalid format: <format>` - nieprawidłowy format wyjściowy
- `Error: Invalid iteration count` - nieprawidłowa liczba iteracji (musi być > 0)
- `Error: Invalid temperature value` - nieprawidłowa wartość temperatury

*Błędy plików:*
- `Error: Cannot open input file: <plik>` - nie można otworzyć pliku wejściowego
- `Error: Cannot create output file: <plik>` - nie można utworzyć pliku wyjściowego
- `Error: File read error at line <numer>` - błąd odczytu w linii

*Błędy danych:*
- `Error: Invalid graph format at line <numer>` - nieprawidłowy format opisu grafu
- `Error: Duplicate edge: <A>-<B>` - duplikacja krawędzi
- `Error: Self-loop detected at vertex <v>` - wykryto pętlę własną
- `Error: Graph is empty` - graf nie zawiera krawędzi
- `Error: Graph is not planar` - graf nie jest planarny (dla algorytmu Tutte)
- `Error: Graph is not 3-connected` - graf nie jest 3-spójny (dla algorytmu Tutte)

*Błędy pamięci:*
- `Error: Memory allocation failed` - brak pamięci

== Zwracane kody powrotu programu

Program zwraca następujące kody wyjścia:

- `0` - sukces, program zakończył się prawidłowo
- `1` - błąd argumentów wiersza poleceń
- `2` - błąd otwarcia/utworzenia pliku
- `3` - błąd w formacie danych wejściowych
- `4` - błąd algorytmu (brak zbieżności, nieprawidłowe dane)
- `5` - błąd alokacji pamięci
- `99` - nieznany błąd wewnętrzny

= Przykłady użycia

== Przykład 1: Prosty graf kwadratowy

Plik wejściowy `square.txt`:
```
# Graf w kształcie kwadratu
AB  1  2  1.0
BC  2  3  1.0
CD  3  4  1.0
DA  4  1  1.0
```

Wywołanie programu:
```bash
./graph square.txt square_out.txt
```

Plik wyjściowy `square_out.txt`:
```
1 0.0 0.0
2 1.0 0.0
3 1.0 1.0
4 0.0 1.0
```

Wynik: węzły ułożone w kwadrat, gotowe do wizualizacji.

== Przykład 2: Graf z wagami i algorytm Tutte

Plik wejściowy `complex.txt`:
```
# Bardziej złożony graf
e1   1  2  1.0
e2   1  3  1.0
e3   2  3  1.0
e4   2  4  1.5
e5   3  4  1.2
e6   1  4  1.8
```

Wywołanie programu z algorytmem Tutte:
```bash
./graph -a tutte complex.txt complex_out.txt
```

Wynik: węzły ułożone zgodnie z algorytmem Tutte, gwarantujący brak przecięć dla grafu planarnego.

== Przykład 3: Eksport binarny z konfiguracją parametrów

Plik wejściowy `large.txt`:
```
# Większy graf - 10 węzłów, 15 krawędzi
e1    1   2  1.0
e2    1   3  1.0
e3    2   4  1.0
e4    2   5  1.0
e5    3   6  1.0
e6    3   7  1.0
e7    4   8  1.0
e8    5   8  1.0
e9    6   9  1.0
e10   7   9  1.0
e11   8  10  1.0
e12   9  10  1.0
e13   1   5  1.5
e14   3   4  1.5
e15   6   8  1.5
```

Wywołanie programu z pełną konfiguracją:
```bash
./graph --algorithm fruchterman --iterations 3000 \
               --temperature 20.0 --format binary \
               large.txt large_out.bin
```

Wynik: plik binarny `large_out.bin` zawierający współrzędne 10 węzłów, obliczone za pomocą algorytmu Fruchterman-Reingold z 3000 iteracjami i wyższą temperaturą początkową dla lepszej eksploracji przestrzeni rozwiązań.