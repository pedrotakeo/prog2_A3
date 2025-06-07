#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "game.h"
#include "weapon.h"

struct environment;

#define ENEMY_AMT 6
#define DEAD 0
#define ALIVE 1
#define LIFE 10

struct enemy_bullet{
    float x;
    float y;
    int speed;
    int aim;
};

struct enemy{
    float x;
    float y;
    float universal_x;
    int life;
    int state;
    int direction;
    int round;
    int timer;

    int sprite_off_x;
    int sprite_off_y;

    int rgb[3];

    float target_pos;
    float player_pos;

    float parameter_left;
    float parameter_right;

    struct enemy_bullet bullet;

};

struct horde{
    int enemies_remaining;
    int speed;

    struct enemy enemy[ENEMY_AMT];
    ALLEGRO_BITMAP *enemy_sprite;
    ALLEGRO_BITMAP *proj_sprite;
};

struct boss_attack{
    float x;
    float y;
    int height;
    int width;
    int appear;
    int speed;
    int attack_timer;
};

struct boss{
    float y;
    int life;
    int timer;
    int direction;
    int attack_timer;
    int death_timer;

    ALLEGRO_BITMAP *enemy_sprite;

    int rgb[3];
    struct boss_attack attack[4];

};


void initialize_enemy_info(struct environment world, struct horde* horde);

void enemy_logic(struct environment world, struct player player, struct horde *horde);

void update_enemy_pos(struct environment world, struct horde *horde);

void initialize_boss_info(struct boss *boss, struct environment world);

void boss_attack_logic(struct boss *boss, struct environment world);


#endif