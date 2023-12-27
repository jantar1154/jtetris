# Tetris
# Kompilace
1. Ke kompilaci musí být v systému nainstalovaný `libsdl2-image-dev`,`libsdl2-ttf-dev`, `libsdl2`, `make` a `gcc`.
## Debian/Ubuntu-based distribuce
```
# apt install libsdl2 libsdl2-image-dev libsdl2-ttf-dev make gcc
```
## Arch-based distribuce
```
# pacman -S sdl2 sdl2_image sdl2_ttf make gcc
```

2. Přesuňte se do složky, kde se nachází `Makefile`
3. Spusťte příkaz `make`.
4. Pokud cokoliv chybí, `gcc` na to upozorní
5. Hra se dá spustit příkazem `./tetris`
# Přizpůsobení
V hlavní složce hry se nachází složka `asset`, ve které jsou složky `font` a `textures`.
## Font
Soubor ve složce `font` se dá zaměnit za jiný soubor s fontem. Důležité ale je, aby se soubor jmenovat `font.ttf` a byl typu **ttf**!
## Pozadí
Pozadí ve hře se dá změnit změněním souboru `asset/textures/background.jpeg`. Jméno i typ souboru se musí shodovat.