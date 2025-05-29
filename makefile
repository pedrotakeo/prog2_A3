CC = gcc 
objs = hello.o player.o weapon.o # (...)add more libraries here

LIB = $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --libs --cflags)
 
# regra default (primeira regra)
vina: $(objs)
	$(CC) -o hello $(objs) $(LIB)
 
 
# regras de compilação
hello.o:   hello.c
player.o: player.c player.h
weapon.o: weapon.c weapon.h
# (...) add more libraries here
 
clean:
	-rm -f $(objs) hello *~ -r ./.vscode 
 