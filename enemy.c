#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "enemy.h"
#include "game.h"
#include "weapon.h"

void initialize_enemy_info(struct environment world, struct horde* horde){
    horde->enemies_remaining = ENEMY_AMT;
    horde->speed = 10;

    for(int i = 0; i < ENEMY_AMT; i ++){
        horde->enemy[i].life = LIFE;
        horde->enemy[i].state = ALIVE;
        horde->enemy[i].round = 0;
        horde->enemy[i].timer = 0;
        //horde->enemy[i].bullet.order = 0;
        //horde->enemy[i].bullet.shoot = false;
        //horde->enemy[i].bullet.speed_x = 40;
        //horde->enemy[i].bullet.speed_y = 0;
        //horde->enemy[i].bullet.timer = 0;
        //horde->enemy[i].bullet.x = 6000;
        //horde->enemy[i].bullet.y = 800;
        horde->enemy[i].target_pos = 0;
        horde->enemy[i].player_pos = 0;
        horde->enemy[i].rgb[0] = 255;
        horde->enemy[i].rgb[1] = 255;
        horde->enemy[i].rgb[2] = 255;
        horde->enemy[i].sprite_off_x = 0;
    }

    //SPECIFICITIES

    //ENEMY 0
    horde->enemy[0].direction = LEFT;
    horde->enemy[0].universal_x = 1300;
    horde->enemy[0].x = horde->enemy[0].universal_x - world.screen_limit_L;
    horde->enemy[0].y = world.screen_height - 175;
    horde->enemy[0].parameter_left = 100;
    horde->enemy[0].parameter_right = 5660;
    horde->enemy[0].sprite_off_y = 0;

    //ENEMY 1
    horde->enemy[1].direction = LEFT;
    horde->enemy[1].universal_x = 1400;
    horde->enemy[1].x = horde->enemy[1].universal_x - world.screen_limit_L;
    horde->enemy[1].y = (world.screen_height/2 )- 135;
    horde->enemy[1].parameter_left = 1400;
    horde->enemy[1].parameter_right = 2000;
    horde->enemy[1].sprite_off_y = 0;

    //ENEMY 2
    horde->enemy[2].direction = RIGHT;
    horde->enemy[2].universal_x = 2400;
    horde->enemy[2].x = horde->enemy[2].universal_x - world.screen_limit_L;
    horde->enemy[2].y = (world.screen_height/2 )- 135;
    horde->enemy[2].parameter_left = 2200;
    horde->enemy[2].parameter_right = 2700;
    horde->enemy[2].sprite_off_y = 64;

    //ENEMY 3
    horde->enemy[3].direction = LEFT;
    horde->enemy[3].universal_x = 2975;
    horde->enemy[3].x = horde->enemy[3].universal_x - world.screen_limit_L;
    horde->enemy[3].y = world.screen_height - 175;
    horde->enemy[3].parameter_left = 100;
    horde->enemy[3].parameter_right = 5660;
    horde->enemy[3].sprite_off_y = 0;

    //ENEMY 4
    horde->enemy[4].direction = RIGHT;
    horde->enemy[4].universal_x = 4450;
    horde->enemy[4].x = horde->enemy[4].universal_x - world.screen_limit_L;
    horde->enemy[4].y = world.screen_height - 175;
    horde->enemy[4].parameter_left = 100;
    horde->enemy[4].parameter_right = 5660;
    horde->enemy[4].sprite_off_y = 64;

    //ENEMY 5  
    horde->enemy[5].direction = LEFT;
    horde->enemy[5].universal_x = 3850;
    horde->enemy[5].x = horde->enemy[5].universal_x - world.screen_limit_L;
    horde->enemy[5].y = (world.screen_height/2 )- 135;
    horde->enemy[5].parameter_left = 3850;
    horde->enemy[5].parameter_right = 4450;
    horde->enemy[5].sprite_off_y = 0;

}

void initialize_boss_info(struct boss *boss){
    boss->life = 200 * MAX_LIFE;
    boss->timer = 0;
    boss->rgb[0] = 255;
    boss->rgb[1] = 255;
    boss->rgb[2] = 255;
}


void round_0_enemy(struct environment world, struct player player, struct enemy* enemy){

    enemy->sprite_off_x = 64;
    if(world.counter % 2){
        enemy->sprite_off_x = 128;
    }

    if(enemy->direction == LEFT){
        enemy->sprite_off_y = 0;
    }
    else{
        enemy->sprite_off_y = 64;
    }


    if(enemy->round == 0 && player.universal_x > enemy->parameter_left && player.universal_x < enemy->parameter_right){
        enemy->player_pos = player.universal_x;
        enemy->round = 1;
    }

    enemy->target_pos = enemy->player_pos + 10;
    if(enemy->universal_x < enemy->player_pos){
        enemy->target_pos = enemy->player_pos - 10;
    }

}

void round_1_enemy (struct environment world , struct enemy* enemy){

    if(enemy->round != 1){
        return;
    }
    
    if(enemy->target_pos < enemy->universal_x && enemy->target_pos >= enemy->parameter_left){
        enemy->direction = LEFT;
        enemy->x -= 10;
        enemy->universal_x = world.screen_limit_L + enemy->x;


        if(enemy->universal_x < enemy->target_pos){
            enemy->universal_x = enemy->target_pos;
            enemy->x = enemy->target_pos -  world.screen_limit_L;

        }

        if(enemy->universal_x < enemy->parameter_left && enemy->y == (world.screen_height/2 )- 135){
            enemy->universal_x = enemy->parameter_left;
            enemy->x = enemy->parameter_left -  world.screen_limit_L;

        }

    }

    if(enemy->target_pos > enemy->universal_x && enemy->target_pos <= enemy->parameter_right){
        enemy->direction = RIGHT;
        enemy->x += 10;
        enemy->universal_x = world.screen_limit_L + enemy->x;


        if(enemy->universal_x > enemy->target_pos){
            enemy->universal_x = enemy->target_pos;
            enemy->x = enemy->target_pos -  world.screen_limit_L;
        }

        if(enemy->universal_x > enemy->parameter_right && enemy->y == (world.screen_height/2 )- 135){
            enemy->universal_x = enemy->parameter_right;
            enemy->x = enemy->parameter_right -  world.screen_limit_L;

        }

    }

    //enemy->bullet.x += enemy->bullet.speed_x;

    if(enemy->universal_x == enemy->target_pos){    //IF IN POSITION DONE
        /*if(enemy->direction == RIGHT){
            enemy->bullet.speed_x = 20;
        }
        else{
            enemy->bullet.speed_x = -20;
        }

        enemy->bullet.x = enemy->x + 15;
        enemy->bullet.y = enemy->y + 40;*/

        enemy->round = 0;
        enemy->timer = 0;
        return;
    }

}

/*void round_2_enemy(struct environment world, struct player player, struct enemy* enemy){
    if(enemy->round != 2){
        return;
    }

    if(enemy->timer = 10000){
        enemy->timer = 0;
        enemy->round = 0;
        return;
    }

    if(enemy->timer = 90){
        enemy->bullet.shoot = false;
    }

    if(enemy->timer == 0){
        if(enemy->x < player.x){
            enemy->direction = RIGHT;
        }
        else{
            enemy->direction = LEFT;
        }
    }

    if(enemy->direction == RIGHT){
        enemy->bullet.x += enemy->bullet.speed_x;
    }
    else{
        enemy->bullet.x -= enemy->bullet.speed_x;
    }

    if(enemy->timer = 30){
        enemy->bullet.shoot = true;
        enemy->bullet.x = enemy->x + 15;
        enemy->bullet.y = enemy->y + 40;
    }


    enemy->timer++;
}*/

void enemy_logic(struct environment world, struct player player, struct horde *horde){
    for(int i = 0; i < ENEMY_AMT; i++){
        if(horde->enemy[i].state == ALIVE && horde->enemy[i].universal_x + 64 > world.screen_limit_L &&  horde->enemy[i].universal_x < world.screen_limit_R){ //if alive and within bounds
            round_0_enemy(world, player, &horde->enemy[i]);
            round_1_enemy(world, &horde->enemy[i]);
            horde->enemy[i].bullet.shoot = true;

        }
        else{
            horde->enemy[i].bullet.shoot = false;
        }
    }

}

void update_enemy_pos(struct environment world, struct horde *horde){
    for(int i = 0; i < ENEMY_AMT; i++){
        if(horde->enemy[i].state == ALIVE){ //if alive and within bounds
            horde->enemy[i].x = horde->enemy[i].universal_x - world.screen_limit_L;
            horde->enemy[i].rgb[0] = 255;
            horde->enemy[i].rgb[1] = 255;
            horde->enemy[i].rgb[2] = 255;
        }
    }
}







