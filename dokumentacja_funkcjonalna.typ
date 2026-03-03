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