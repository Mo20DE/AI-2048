# Bachelorprojekt 2048-Puzzle

## Beschreibung

Dieses Repository repräsentiert die Implementierung von Algorithmen zur Lösung des 2048-Puzzles im Rahmen einer Bachelorarbeit mit dem Titel ***2048 lösen: MCTS und Expectimax mit domänenspezifischen Heuristiken im Vergleich*** am Fachgebiet Neurotechnologie der Technischen Universität Berlin.


## Algorithmen und Heuristiken

Konkret wurden drei Algorithmen implementiert. Zum einen Expectimax mit einer Transpositionstabelle, die redundante Zustände erkennt und zum anderen klassisches MCTS bzw. MCTSE (MCTS Expectimax). Um die Suche zielgerichteter zu gestalten, wurden drei Heuristiken in einer Evaluationsfunktion kombiniert. Diese bewertet einen Zustand, indem diesem ein numerischer Wert zugeordnet wird. Die verwendeten Heuristiken lauten wie folgt:

1. **Monotonicity:** Diese Heuristik bewertet, wie geordnet das Spielbrett ist, d.h. sind die Kachelwerte auf- bzw. absteigend entlang der Zeilen bzw. Spalten, handelt es sich um einen monotonen Brettzustand.

2. **Smoothness:**  Damit sich Kacheln verschmelzen können, müssen diese benachbart sein und den gleichen Wert haben. Genau solche Spielbretter versucht die Smoothness-Heuristik zu priorisieren, indem versucht wird, die Differenz von benachbarten Kacheln so klein wie möglich zu halten.

3. **Empty-Cells:** Wie der Name bereits andeutet, ermittelt diese Heuristik die Anzahl leerer Felder, die sich auf dem Brett befinden. Intuitiv versucht ein menschlicher Spieler, diesen Wert zu maximieren, denn je mehr leere Felder existieren, desto unwahrscheinlicher ist es, dass das Spiel plötzlich endet.

Die Evaluationsfunktion mit den drei genannten Heuristiken kommt bei der Expectimax-Suche zum einsatz. Für MCTS(E) wurde ausschließlich die Empty-Cells-Heuristik verwendet, da MCTS(E) mit dieser die besten Ergebnisse erzielte.

## Installation und Ausführung

Um die Agenten zu testen, kann mit einem Befehl und zusätzlich zwei spezifischen Parametern das Projekt gebaut und ausgeführt werden. Der erste Parameter ist `Algorithm` mit den Optionenen: `0` (Expectimax), `1` (MCTSE) oder `2` (MCTS). Und der zweite ist `Depth` (für Expectimax), mit einem Wert zwischen `1` und `10` bzw. `Iterations` (für MCTS(E)) mit einem Wert zwischen `100`und `4000`.

```
./build_and_run.sh <Algorithm> <Depth | Iterations> 
```

Ein Beispiel wäre folgender Befehl, mit dem eine Simulation des Expectimax-Agenten mit Tiefenlimit 3 ausgeführt wird.

```
./build_and_run.sh 0 3
```

