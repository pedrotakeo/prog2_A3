CC = gcc 
objs = main.o game.o weapon.o enemy.o

LIB = $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --libs --cflags)
 
main: $(objs)
	$(CC) -o main $(objs) $(LIB)

main.o:   main.c
game.o: game.c game.h
weapon.o: weapon.c weapon.h
enemy.o: enemy.c enemy.h
 
clean:
	-rm -f $(objs) main *~
 