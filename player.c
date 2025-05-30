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

void initialize_player_info(struct environment world, struct player *player){
    player->x = 200;
    player->y = world.floor;
    player->og_dimensions = 64;
    player->dimensions = 100;
    player->speed = 20;
    player->direction = RIGHT;
    player->aim = RIGHT;
    player->frame_jump = 0;
    player->jump_enable = true; //jump enabled
    player->sprite_off_x = 0;
    player->sprite_off_y = 64;
    player->stamina = MAX_STAMINA;
    player->stamina_recount = MAX_STAMINA;

    for(int  i = 0; i < MAX_LIFE; i++){
        player->life[i] = true;
    }
}


void jump_player(struct player* player){
    player->jump_enable = false;
    player->frame_jump = 60;
}