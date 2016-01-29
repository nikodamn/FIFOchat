Aby uruchomic aplikacje nalezy skompilowac plik Makefile przez podanie komendy:

make -f Makefile

Uruchomienie programu polega na otworzeniu pliku 'serwer' na jednej maszynie (lub w jednym oknie programu) oraz jednego lub kilku plikow 'klient'. Menu obslugi aplikacji znajduje sie juz po stronie klienta.

Pliki *.c zawieraja:
117209_s.c - aplikacja serwera z funkcjami pomocnicznymi
117209_k.c - aplikacja klienta z funkcjami pomocnicznymi
117209_config.c - program zawierajacy struktury, z ktorych korzystaja zarowno serwer, jak i klient oraz zainkludowane wszystkie niezbedne biblioteki.