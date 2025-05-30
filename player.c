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
    player->universal_x = 200;
    player->og_dimensions = 64;
    player->dimensions = 100;
    player->collision_height = player->dimensions;
    player->speed = 20;
    player->direction = RIGHT;
    player->aim = RIGHT;
    player->frame_jump = 0;
    player->jump_enable = true; //jump enabled
    player->sprite_off_x = 0;
    player->sprite_off_y = 64;
    player->stamina = MAX_STAMINA;
    player->stamina_recount = MAX_STAMINA;

    player->rgb[0] = 255;
    player->rgb[1] = 255;
    player->rgb[2] = 255;

    for(int  i = 0; i < MAX_LIFE; i++){
        player->life[i] = true;
    }
}

void initialize_world_info(struct environment *world){
    world->gravity = 40;
    world->screen_width = 1280;
    world->screen_height = 720;
    world->floor = world->screen_height - 175;
    world->counter = 0;
    world->bkg_off_x = 0;
    world->bkg_img_og_height = 256;
    world->bkg_img_og_width = 2048;
    world->air_round = 0;
}



void jump_player(struct player* player){
    player->jump_enable = false;
    player->frame_jump = 60;
}

void determine_universal_screen_limits(struct environment *world){
    int total_width_scaled = (world->screen_height * 2048)/256;

    world->screen_limit_L = (world->bkg_off_x * total_width_scaled)/2048;
    world->screen_limit_R = world->screen_limit_L + world->screen_width;

}

void determine_universal_player_pos(struct environment world, struct player *player){

    player->universal_x = world.screen_limit_L + player->x;
}