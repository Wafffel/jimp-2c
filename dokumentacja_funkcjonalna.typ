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
    Krzysztof Wasilewski, Jakub Pietrzkiewicz\
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

Aplikacja implementuje dwa różne algorytmy układania grafów, co umożliwia porównanie ich efektywności i jakości generowanych wizualizacji. Program jest sterowany z linii poleceń za pomocą argumentów, co zapewnia elastyczność i możliwość automatyzacji.

== Dostępne funkcjonalności

Program oferuje następujące funkcjonalności:

- Wczytywanie grafów planarnych z plików tekstowych w formacie listy krawędzi
- Wyznaczanie współrzędnych węzłów za pomocą wybranego algorytmu układania grafów
- Generowanie wyników w formacie tekstowym lub binarnym (do wyboru przez użytkownika)
- Obsługa dwóch algorytmów: Fruchterman-Reingold oraz Tutte Embedding
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

Program jest uruchamiany z linii poleceń. Składnia wygląda następująco:

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

- `-i <iteracje>` lub `--iterations <iteracje>` - liczba iteracji dla algorytmu Fruchterman-Reingold
  - Wartość: liczba całkowita dodatnia
  - Domyślnie: `1000`

- `-t <temperatura>` lub `--temperature <temperatura>` - wartość początkowej temperatury dla algorytmu Fruchterman-Reingold
  - Wartość: liczba zmiennoprzecinkowa dodatnia
  - Domyślnie: `10.0`

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
# Przykładowy graf
AB   1  2  1.54
BC   2  3  1.0
CD   3  4  1.17
DA   4  1  1.93
AC   1  3  1.0
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

Plik wyjściowy w formacie binarnym zawiera te same dane co format tekstowy, ale zapisane w reprezentacji binarnej dla efektywniejszego przechowywania i szybszego wczytywania.

Każdy węzeł jest reprezentowany przez 20 bajtów, gdzie:
- 4 bajty: identyfikator wierzchołka (int)
- 8 bajtów: współrzędna X (double)
- 8 bajtów: współrzędna Y (double)

Wszystkie wartości zapisane są w kolejności little-endian.

= Ograniczenia i wymagania

== Wymagania systemowe i sprzętowe

- *Architektura:* Wymagany procesor Little-Endian (np. x86_64).
- *Środowisko:* Wymagany kompilator GCC i narzędzie Make.
- *Pamięć:* Przechowywanie grafu w pamięci RAM; dla standardowych grafów (mniej niż 5000 węzłów) zużycie nie przekracza 1 GB.

== Ograniczenia danych i struktury grafu
- *Wierzchołki:* Identyfikatory muszą być dodatnimi liczbami całkowitymi.
- *Krawędzie:* Maksymalna długość etykiety to 32 znaki.
- *Struktura grafu:* Program obsługuje wyłącznie grafy spójne; brak wsparcia dla pętli własnych (węzeł połączony z samym sobą) oraz multigrafów (wiele krawędzi między tą samą parą węzłów).

== Specyfika algorytmów
- *Fruchterman-Reingold:* Złożoność $O((V^2 + E) dot I)$ sprawia, że przy bardzo dużych grafach czas obliczeń rośnie drastycznie.
- *Tutte:* Wymaga grafu planarnego i najlepiej 3-spójnego (takiego, którego nie da się rozspójnić usunięciem mniej niż trzech wierzchołków); brak spełnienia tego warunku może spowodować nakładanie się wierzchołków.

= Opis algorytmów

== Algorytm Fruchterman-Reingold

Algorytm Fruchterman-Reingold jest klasycznym przykładem algorytmu siłowego (force-directed), stosowanego do wizualizacji grafów. Opiera się on na modelu fizycznym, w którym wierzchołki traktowane są jako obiekty oddziałujące siłami, natomiast krawędzie odwzorowują działanie sprężyn łączących wybrane pary węzłów. Celem algorytmu jest wyznaczenie takiego rozmieszczenia wierzchołków, aby układ osiągnął stan równowagi odpowiadający minimalnej energii.

*Zasada działania*

W modelu przyjmuje się następujące założenia:

- Wszystkie węzły w grafie odpychają się wzajemnie.
- Węzły połączone krawędzią przyciągają się do siebie.

Algorytm działa iteracyjnie i obejmuje następujące etapy:

1. Losowe rozmieszczenie węzłów w obszarze roboczym.
2. Obliczenie sił działających na każdy węzeł:
  - siły odpychającej od wszystkich pozostałych węzłów,
  - siły przyciągającej od węzłów połączonych krawędzią.
3. Przemieszczenie węzłów zgodnie z wypadkową sił.
4. Obniżenie temperatury ograniczającej maksymalne przemieszczenie węzłów.

Kroki 2-4 są powtarzane do momentu osiągnięcia stanu równowagi lub wykonania zadanej liczby iteracji.

*Parametry algorytmu*

- *Liczba iteracji* - określa czas działania algorytmu.
- *Temperatura początkowa* - wyznacza maksymalne przemieszczenie węzłów w początkowych iteracjach.
- *Waga krawędzi* - im większa waga, tym silniejsze przyciąganie między węzłami.

*Opis matematyczny*

#h(2em) *a)* *Optymalny dystans*

Optymalna odległość między węzłami dana jest wzorem:

$
  k = sqrt(frac("Obszar", |V|))
$

gdzie:

- $"Obszar"$ - całkowite pole powierzchni roboczej,
- $|V|$ - liczba węzłów w grafie.

#h(2em) *b)* *Reguła odpychania*

Siła odpychania dla dwóch węzłów oddalonych o odległość $d$:

$
  f_r (d) = -k^2 / d
$

#h(2em) *c)* *Reguła przyciągania (z uwzględnieniem wag)*

Siła przyciągania dla węzłów połączonych krawędzią o wadze $w$:

$
  f_a (d) = w dot d^2 / k
$

#h(2em) *d)* *Wypadkowa sił*

Dla każdego węzła obliczany jest wektor wypadkowy:

$
  D_v = sum f_r + sum f_a
$

#h(2em) *e)* *Stabilizacja układu (chłodzenie)*

Aby zapobiec oscylacjom, maksymalne przesunięcie w pojedynczej iteracji ograniczone jest przez temperaturę $t$. Wartość ta maleje w kolejnych iteracjach aż do zera.

*Zalety i wady*

*Zalety:*
- generuje estetyczne i często symetryczne układy,
- zmniejsza liczbę przecięć krawędzi,
- ułatwia analizę struktury sieci dzięki grupowaniu silnie powiązanych węzłów,
- ma charakter uniwersalny.

*Wady:*
- wysoka złożoność obliczeniowa dla dużych grafów (każdy węzeł oddziałuje z każdym),
- możliwość zatrzymania w lokalnym minimum energii przy niekorzystnym doborze temperatury lub początkowego rozmieszczenia.
== Algorytm Tutte embeddings

Algorytm Tutte'a wyznacza współrzędne wierzchołków grafu planarnego. Jego działanie opiera się na matematycznym modelu równowagi sił przyciągania, co pozwala na uzyskanie przejrzystej i uporządkowanej wizualizacji.

=== Mechanizm automatycznego kotwiczenia

Aby umożliwić jednoznaczne rozpięcie grafu na płaszczyźnie, program stosuje system czterech statycznych punktów podparcia. Proces ten przebiega w sposób zautomatyzowany:

*Identyfikacja wierzchołków bazowych:* Aplikacja analizuje strukturę połączeń i wybiera cztery wierzchołki o najwyższym stopniu (posiadające najwięcej sąsiadów). Pełnią one rolę "kotwic" rozciągających graf.

*Definicja obszaru roboczego:* Wybrane wierzchołki zostają na stałe przypisane do narożników kwadratu o boku 1000 jednostek. Ich współrzędne są niezmienne w trakcie trwania obliczeń i wynoszą odpowiednio: $(0,0)$, $(1000,0)$, $(1000,1000)$ oraz $(0,1000)$.

*Statyczność ramy:* Wierzchołki bazowe są wyłączone z procesu iteracyjnego, co zmusza pozostałą część grafu do dopasowania się do sztywnych granic obszaru roboczego.

=== Matematyczny model wyznaczania współrzędnych

Pozycje wszystkich wierzchołków wewnętrznych są wyznaczane drogą rozwiązywania układu równań liniowych. Algorytm dąży do osiągnięcia stanu, w którym każdy wierzchołek $i$ znajduje się dokładnie w ważonym środku ciężkości swoich sąsiadów.

Współrzędne $(x, y)$ każdego wolnego wierzchołka obliczane są według wzorów:

$ x_i = frac(sum_(j in N(i)) w_(i j) dot x_j, sum_(j in N(i)) w_(i j)) $

$ y_i = frac(sum_(j in N(i)) w_(i j) dot y_j, sum_(j in N(i)) w_(i j)) $

Legenda oznaczeń:
- $x_i, y_i$ - wyznaczane współrzędne wierzchołka $i$
- $N(i)$ - zbiór wierzchołków sąsiadujących bezpośrednio z wierzchołkiem $i$
- $w_(i j)$ - waga krawędzi łączącej wierzchołek $i$ z wierzchołkiem $j$ (pobrana z pliku wejściowego)

=== Proces iteracyjnej stabilizacji układu

Wyznaczenie współrzędnych nie jest operacją jednorazową, lecz procesem dążenia do równowagi  Przebiega on w następujący sposób:

*Inicjalizacja:* Wierzchołki ramy trafiają do narożników kwadratu, a wszystkie pozostałe węzły są wstępnie umieszczane w centrum obszaru roboczego na pozycji $(500, 500)$.

*Iteracja:* Program wielokrotnie przebiega przez listę wolnych wierzchołków, aktualizując ich pozycje na podstawie aktualnych położeń ich sąsiadów.

*Warunek stopu:* Obliczenia kończą się, gdy maksymalne przesunięcie wierzchołka w danej iteracji spadnie poniżej zadanego progu precyzji $epsilon = 0.0001$. Oznacza to, że układ osiągnął stabilność i wierzchołki znalazły swoje docelowe miejsca.

=== Funkcjonalne właściwości rozwiązania

Zastosowanie powyższej metody gwarantuje użytkownikowi uzyskanie wyników o następujących cechach:

*Domknięcie wypukłe:* Algorytm gwarantuje, że żaden wierzchołek ani krawędź nie znajdzie się poza wyznaczoną ramą kwadratową.

*Wypukłość ścian:* Wszystkie wewnętrzne obszary ograniczone krawędziami zostaną przedstawione jako wielokąty wypukłe, co zapewnia wysoką czytelność i estetykę wizualizacji.

*Reprezentacja wag:* Wyższe wagi skutkują mniejszą odległością między wierzchołkami, co pozwala na intuicyjną analizę skupisk w grafie.

*Determinizm:* Wynik nie jest zależny od żadnych zmiennych losowych, te same argumenty wejściowe zawsze wygenerują identyczny układ współrzędnych.

= Obsługa błędów

== Komunikaty o błędach

Program wyświetla komunikaty o błędach na standardowe wyjście błędów (stderr).

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
