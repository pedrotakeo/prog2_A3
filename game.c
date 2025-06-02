#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "game.h"
#include "weapon.h"
#include "enemy.h"

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
    player->life = MAX_LIFE;
    player->damage_cooldown = 0;

    player->rgb[0] = 255;
    player->rgb[1] = 255;
    player->rgb[2] = 255;
}

void initialize_world_info(struct environment *world){
    world->gravity = 25;
    world->screen_width = 1280;
    world->screen_height = 720;
    world->floor = world->screen_height - 175;
    world->counter = 0;
    world->bkg_off_x = 0;
    world->bkg_img_og_height = 256;
    world->bkg_img_og_width = 2048;
    world->air_round = 0;

}

void set_player_methods(struct player *player){
    player->move = move_player;

    player->jump = jump_player;

    player->aim_adjust = adjust_player_aim;

    player->duck = duck_player;

    player->shoot = player_shoot;

    player->univ_pos = determine_universal_player_pos;

    player->state = player_state_dependent_operations;

}

void move_player(struct environment *world, struct player *player, ALLEGRO_KEYBOARD_STATE *ks, ALLEGRO_MOUSE_STATE *ms, int game_state){


    if(game_state ==  GAME){
        //PLAYER MOVES LEFT====================================================================================
        if(player->x >= player->speed && (al_key_down(ks, ALLEGRO_KEY_LEFT) || al_key_down(ks, ALLEGRO_KEY_A)) && (!al_key_down(ks, ALLEGRO_KEY_S))){

    
            if(player->x <= world->screen_width/2 && player->universal_x >= (world->screen_width/2)+(player->dimensions/2)){
                world->bkg_off_x -= 10;
            }
            else if(player->universal_x >= 100){

                player->x -= player->speed;
            }

            player->sprite_off_x = 64;
            if(world->counter % 3){
                player->sprite_off_x = 128;
            }

            if(al_mouse_button_down(ms, 1)){
                player->sprite_off_x = 192;
                if(world->counter % 3){
                    player->sprite_off_x = 256;
                }
            }
            player->direction = LEFT;
            player->aim = LEFT;
        }

        //PLAYER MOVES RIGHT====================================================================================
        if(player->x <= (world->screen_width - player->speed - player->dimensions) && (al_key_down(ks, ALLEGRO_KEY_RIGHT) || al_key_down(ks, ALLEGRO_KEY_D)) && (!al_key_down(ks, ALLEGRO_KEY_S))){

            if(player->x >= world->screen_width/2 && player->universal_x <= 5760 - (world->screen_width/2) - (player->dimensions/2)){
                world->bkg_off_x += 10;
            }
            else if(player->universal_x <= 5660){
                player->x += player->speed;
            }

            player->sprite_off_x = 64;
            if(world->counter % 3){
                player->sprite_off_x = 128;
            }

            if(al_mouse_button_down(ms, 1)){
                player->sprite_off_x = 192;
                if(world->counter % 3){
                    player->sprite_off_x = 256;
                }
            }
            player->direction = RIGHT;
            player->aim = RIGHT;


        }
    }
    else if(game_state == GAME_BOSS_STATE){
        player->speed = 15;
        if(player->x >= 0 && player->x + 64 <= world->screen_width && player->y >= 0 && player->y + 64 <= world->screen_width){
            if(al_key_down(ks, ALLEGRO_KEY_A)){
                player->x -= player->speed;
                if(player->x < 0){
                    player->x = 0;
                }
            }
            if(al_key_down(ks, ALLEGRO_KEY_D)){
                player->x += player->speed;
                if(player->x + 64 > world->screen_width){
                    player->x = world->screen_width - 64;
                }
            }
            if(al_key_down(ks, ALLEGRO_KEY_W)){
                player->y -= player->speed;
                if(player->y < 0){
                    player->y = 0;
                }
            }
            if(al_key_down(ks, ALLEGRO_KEY_S)){
                player->y += player->speed;
                if(player->y + 64 > world->screen_height){
                    player->y = world->screen_height - 64;
                }
            }
            
        }
        
    }
    
    
}


void jump_player(struct environment *world, struct player *player, ALLEGRO_KEYBOARD_STATE *ks, float *og_floor){

    if((al_key_down(ks, ALLEGRO_KEY_SPACE)) && player->jump_enable && (!al_key_down(ks, ALLEGRO_KEY_S))){
        *og_floor = world->floor;
        player->jump_enable = false;
        player->frame_jump = 60;

    }

    if(!player->jump_enable){
        player->sprite_off_x = 384;
    }

    player->y -= player->frame_jump;

    if(player->y >= world->floor){
        *og_floor = world->floor;
        world->air_round = 0;
        player->y = world->floor;
        player->frame_jump = 0;
        player->jump_enable = true;

    }

    if (player->y <= (*og_floor) - (8 * player->frame_jump)){
        world->air_round++;
        if(world->air_round < 3){
            player->frame_jump = 0;
        }
        else{ 
            player->frame_jump = -(1.5 * world->gravity);
        }

    }
}

void determine_universal_screen_limits(struct environment *world){
    int total_width_scaled = (world->screen_height * 2048)/256;

    world->screen_limit_L = (world->bkg_off_x * total_width_scaled)/2048;
    world->screen_limit_R = world->screen_limit_L + world->screen_width;

}

void determine_universal_player_pos(struct environment world, struct player *player){

    player->universal_x = world.screen_limit_L + player->x;
}


void reset_info(struct environment *world, struct player *player, ALLEGRO_MOUSE_STATE *ms){
    player->collision_height = player->dimensions;
    player->rgb[0] = 255;
    player->rgb[1] = 255;
    player->rgb[2] = 255;
    determine_universal_screen_limits(world);
    determine_universal_player_pos(*world, player);

    // SE PLAYER ESTA PARA DIREITA, SPRITESHEET METADE DE BAIXO
    // SE PLAYER ESTA PARA ESQUERDA, SPRITESHEET METADE DE CIMA
    player->sprite_off_x = 0;
    if(player->direction == RIGHT){
        player->sprite_off_y = 64;
    }
    else{
        player->sprite_off_y = 0;
    }

    if(al_mouse_button_down(ms, 1) && world->counter > 5){
        player->sprite_off_x = 192;
    }
}

void pause_game(ALLEGRO_KEYBOARD_STATE *ks, int *running_screen){
    if(al_key_down(ks, ALLEGRO_KEY_ESCAPE)){
        *running_screen = 0; // abre menu
    }

}

void adjust_player_aim(struct player *player, ALLEGRO_KEYBOARD_STATE *ks){
    if(al_key_down(ks, ALLEGRO_KEY_W)){
        player->aim = UP;
    }
    if(al_key_down(ks, ALLEGRO_KEY_W) && al_key_down(ks, ALLEGRO_KEY_D)){
        player->aim = RUP;
    }
    if(al_key_down(ks, ALLEGRO_KEY_W) && al_key_down(ks, ALLEGRO_KEY_A)){
        player->aim = LUP;
    }
    if(al_key_down(ks, ALLEGRO_KEY_S)){
        player->aim = DOWN;
    }
    if(al_key_down(ks, ALLEGRO_KEY_S) && al_key_down(ks, ALLEGRO_KEY_D)){
        player->aim = RDOWN;
    }
    if(al_key_down(ks, ALLEGRO_KEY_S) && al_key_down(ks, ALLEGRO_KEY_A)){
        player->aim = LDOWN;
    }
}

void duck_player(struct player *player, ALLEGRO_KEYBOARD_STATE *ks){
    if(al_key_down(ks, ALLEGRO_KEY_S)){
        player->sprite_off_x = 320;
        player->collision_height = player->collision_height/2;

    }
}

void player_shoot(struct environment *world, struct player *player, struct weapon *weapon, struct weapon *backup, ALLEGRO_KEYBOARD_STATE *ks, ALLEGRO_MOUSE_STATE *ms){
    struct bullet* aux = weapon->first;
    struct bullet* temp;

    if(weapon->cool_down > 0){
        weapon->cool_down++;
    }

    if(weapon->cool_down >= 4){
        weapon->cool_down = 0;
    }


    while(aux){
        temp = aux->next;
        aux->info.x += aux->info.speed_x;
        aux->info.y += aux->info.speed_y;
        aux->info.timer++;
        if(aux->info.timer >= 30){// se está dentro da tela
            aux = destroy_bullet(weapon, aux);
            free(aux);
        }

        aux = temp;
    }

    if(player->stamina <= 0){
        if(player->stamina < 0){
            player->stamina_recount = 0;
        }
        player->stamina_recount = player->stamina_recount + 2;
        if(player->stamina_recount >= MAX_STAMINA){
            player->stamina_recount = MAX_STAMINA;
            player->stamina = MAX_STAMINA;
        }
    }

    if(al_mouse_button_down(ms, 1) && weapon->cool_down == 0 && player->stamina > 0 && world->counter > 5){
        player->stamina--;
        player->stamina_recount--;
        if(backup->first){
            weapon->cool_down++;
            struct bullet* temp = destroy_bullet(backup, backup->first); // pega bala alocada da lista backup
            load_weapon(weapon, temp, (*player));  //carrega na arma principal

            struct bullet* aux = create_projectile(backup, (*player)); 
            load_weapon(backup, aux, (*player));  // faz refill na backup
        }

    }
}

void player_state_dependent_operations(struct environment *world, struct player *player){
    bool in_platform_range = (player->universal_x <= 2700 && player->universal_x >= 2100 || player->universal_x <= 2000 && player->universal_x >= 1350 || player->universal_x <= 4450 && player->universal_x >= 3850);
    if(in_platform_range && player->y > world->screen_height/2){
        player->rgb[0] = 117;
        player->rgb[1] = 79;
        player->rgb[2] = 63;
    }

    if(in_platform_range && player->y <= (world->screen_height/2 )- 130 ){
        world->floor = (world->screen_height/2 )- 135;
    }
    else{
        world->floor = world->screen_height - 175;
    }
}

void player_to_enemy_damage(struct environment *world, struct player *player, struct weapon *weapon, struct horde *horde){
    struct bullet* aux = weapon->first;
    struct bullet* temp;
    while(aux){
        temp = aux->next;
        for(int i = 0; i < ENEMY_AMT; i++){
            if(horde->enemy[i].state == ALIVE && horde->enemy[i].universal_x > world->screen_limit_L &&  horde->enemy[i].universal_x < world->screen_limit_R){ //if alive and within bounds
                if (aux->info.x < horde->enemy[i].x + 64 && aux->info.x > horde->enemy[i].x && aux->info.y < horde->enemy[i].y + 64 && aux->info.y > horde->enemy[i].y){
                    aux = destroy_bullet(weapon, aux);
                    free(aux);
                    horde->enemy[i].life--;
                    horde->enemy[i].rgb[0] = 255;
                    horde->enemy[i].rgb[1] = 0;
                    horde->enemy[i].rgb[2] = 0;
                    if(!horde->enemy[i].life){
                        horde->enemy[i].state = DEAD;
                    }

                }

            }
        }

        aux = temp;
    }
}

void enemy_to_player_damage(struct environment *world, struct player *player, struct weapon *weapon, struct horde *horde, int *running_screen){
    if(player->damage_cooldown > 0){
        player->damage_cooldown--;
        if(player->damage_cooldown % 3){
            player->rgb[0] = 255;
            player->rgb[1] = 0;
            player->rgb[2] = 0;
        }
        return;
    }

    for(int i = 0; i < ENEMY_AMT; i++){
        bool touch1 = player->x + 64 >= horde->enemy[i].x && player->x + 64 <= horde->enemy[i].x + 64 && player->y + 64 >= horde->enemy[i].y && player->y + 64 <= horde->enemy[i].y + 64;
        bool touch2 = player->x >= horde->enemy[i].x && player->x <= horde->enemy[i].x + 64 && player->y >= horde->enemy[i].y && player->y <= horde->enemy[i].y + 64;;

        if(horde->enemy[i].state == ALIVE && horde->enemy[i].universal_x > world->screen_limit_L &&  horde->enemy[i].universal_x < world->screen_limit_R){ //if alive and within bounds
            if (touch1 || touch2){
                player->damage_cooldown = 12;
                player->life--;
                player->rgb[0] = 255;
                player->rgb[1] = 0;
                player->rgb[2] = 0;
                if(!player->life){
                    *running_screen = 2;
                }

            }


        }
    }
}


void player_to_boss_damage(struct environment world, struct boss *boss, struct weapon *weapon){
    struct bullet* aux = weapon->first;
    struct bullet* temp;
    while(aux){
        temp = aux->next;
        if(aux->info.x > world.screen_width - 192){
            aux = destroy_bullet(weapon, aux);
            free(aux);
            boss->life--;
            boss->rgb[0] = 255;
            boss->rgb[1] = 0;
            boss->rgb[2] = 0;
        }
        

        aux = temp;
    }
}
