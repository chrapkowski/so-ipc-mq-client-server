# Zadanie Klient - Server z użyciem kolejki komunikatów #

Adam Chrapkowski

## Treść ##

> Serwer tworzy kolejkę komunikatów i czeka na zgłoszenie od klienta
> zawierające:  `typ komunikatu = 1`, liczby całkowite oraz `numer PID` klienta.
>
> Serwer oblicza iloczyn otrzymanych liczb i zwaraca wynik do klienta.


## Rozwiązanie ##

Rozwiązanie działa dla dowolnej ilości liczb (co najmniej jednej) oraz posługuje się protokołem
tekstowym w komunikacji klient-serwer. Możliwym innym rozwiązaniem jest przekazywanie binarnej
struktury danych, nie mniej jednak protokoły komunikacyjne zwykle są tekstowe, stąd taka decyzja.

### Najważniejsze obiekty to: ###

- struktury `msgbuf` przekazywane i odbierane za pomocą kolejki
- struktury `st_msg` (wskaźnik nań `Msg`) przechowujące dane wykorzystywane w komunikacji
- łańcuchy tekstowe (protokół) reprezentujący struktury `st_msg`, konwersja struktur na protokół
  oraz protokołu na struktury odbywa się odpowiednio za pomocą funkcji `msg_save` oraz `msg_load`.
  Funkcja `msg_save` jest trywialnym (nie buduje drzewa, nie tworzy tablicy symboli) parserem 
  protokołu.
  
### Opis protokołu komunikacji ###

W ogólności protokół ma postać:

    mul: {
      from  : <pid_wysyłającego>,
      to    : <pid_odbiorcy>,
      length: <ilość_przekazywanych_wartości_w_polu_values>,
      values: [
        <wartość_1>,
        <wartość_2>,
        <wartość_3>,
        ...
        <wartość_n>
      ]
    }

Wielkość znaków identyfikatorów `mul`, `from`, `to`, `length` i `values` nie jest rozróżniana,
a białe znaki są pomijane. Kolejność pól `from`, `to` i `length` jest dowolna, a pole `values` winno
być zawsze ostatnie (a przynajmniej występować po polu `length`). Jeśli pole `length` ma wartość
`1`, to nawiasy kwadratowe okalające listę wartości mogą zostać pominięte.

### Uwagi Końcowe ###

Klucz kolejki bazuje na iWęźle katalogu w którym znajdują się programy więc programy `client` i
`server` muszą znajdować się w tym samym katalogu. Serwer można zatrzymać poprzez wysłanie sygnału.
Usunąć zbędną kolejkę za pomocą narzędzia `ipcrm`.

Programy przetestowano za pomocą systemu (`uname -a`):

    Linux linux 3.0.0-14-generic #23-Ubuntu SMP Mon Nov 21 20:28:43 UTC 2011 x86_64 x86_64 x86_64 GNU/Linux

