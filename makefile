CC = gcc 
objs = main.o game.o weapon.o enemy.o # (...)add more libraries here

LIB = $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --libs --cflags)
 
# regra default (primeira regra)
vina: $(objs)
	$(CC) -o main $(objs) $(LIB)
 
 
# regras de compilação
main.o:   main.c
game.o: game.c game.h
weapon.o: weapon.c weapon.h
enemy.o: enemy.c enemy.h
# (...) add more libraries here
 
clean:
	-rm -f $(objs) main *~ -r ./.vscode 
 