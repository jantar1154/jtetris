
# config
opt = O0
name = jtetris
path = src/
args = -std=c99 -Wall -g -lSDL2 -lSDL2_ttf -fsanitize=address

build:
	gcc $(args) -$(opt) $(path)*.c -o $(name)

clean:
	rm $(name)
