#ifndef __weapon_H__
#define __weapon_H__

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"
#include "weapon.h"

#define MAX_weapon 4

struct projectile{
    int order;
    float x;
    float y;
    int speed_x;
    int speed_y;
};

struct bullet{
    struct projectile info;
    
    struct bullet *next;
    struct bullet *prev;
};

struct weapon{
    int ammo_amt;

    ALLEGRO_BITMAP *projectile;
    int og_dimensions;
    int dimensions;

    struct bullet *first;
    struct bullet *last;
};

struct weapon* create_weapon();

struct bullet* create_projectile(struct weapon *weapon, struct player player);

int load_weapon(struct weapon *weapon, struct player player);

int destroy_bullet(struct weapon *weapon, struct bullet* rem);

struct weapon *destroy_weapon(struct weapon *weapon);

void update_bullet_trajectories(struct weapon *weapon, struct environment world);

void draw_bullet(struct weapon *weapon);

#endif
