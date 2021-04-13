# Interakcja

Program pokazuje prostą interakcję polegającą na sterowaniu ruchem obiektu oraz położeniem obserwatora.


Opis algorytmu zastosowanego do uzyskania efektu
- Sprawdź czy przycisk myszy jest wciśnięty. Jeśli tak pobierz pozycję kursora 
i zapisz do zmiennej oraz ustaw odpowiedni status.
- Jeśli mysz została poruszona podczas wciśniętego przycisku oblicz różnicę 
położenia kursora i podstaw jego aktualną pozycję jako poprzednią.
- Przerysuj okno co wywołuję funkcję renderującą.
- W zależności od statusu oblicz azymut i elewację lub zmianę odległości 
obserwatora od obiektu.
- Oblicz składowe położenia obserwatora.
- Zmień położenie zgodnie ze składowymi.
- Wywołaj funkcję rysującą jajko.
