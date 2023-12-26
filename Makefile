
# config
opt = O0
name = jtetris
path = src
builddir = build
args = -std=c99 -Wall -g -lSDL2 -lSDL2_ttf -lSDL2_image -fsanitize=address

# dont touch
src = $(wildcard $(path)/*.c)
obj = $(src:$(path)/%.c=$(builddir)/%.o)

all: $(name)

$(name): $(obj)
	gcc $(args) -o $@ $^ 

$(builddir)/%.o: $(path)/%.c
	mkdir -p $(builddir)
	gcc $(args) -o $@ -c $<

clean:
	rm $(obj) $(name)
	rmdir $(builddir)
