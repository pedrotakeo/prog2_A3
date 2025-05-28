#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"

void set_player_methods(struct player *player){
    player->move = move_player;

    player->jump = jump_player;

}

void move_player( struct player *player, int direction){

    if(direction == RIGHT){
        player->x += player->speed;
    }
    else{
        player->x -= player->speed;
    }
}


void jump_player(struct player* player){
    player->jump_enable = false;
    player->frame_jump = 60;
}