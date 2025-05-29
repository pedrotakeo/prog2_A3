#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"
#include "gun.h"

#define MAX_AMMO 4

struct projectile{
    int order;
    float x;
    float y;
    int speed_x;
    int speed_y;
};

struct casing{
    struct projectile info;
    
    struct casing *clock;
};

struct ammo{
    int ammo_amt;

    ALLEGRO_BITMAP *projectile;
    int og_dimensions;
    int dimensions;

    struct casing *magazine;
};

struct ammo* create_ammo(char *file_name){
    struct ammo *new;
    if(!(new = malloc (sizeof(struct ammo)))){
        return NULL;
    }
    new->ammo_amt = 0;
    new->dimensions = 35;
    new->og_dimensions = 32;
    
    
    new->magazine = NULL;

    return new;
}

struct casing* create_projectile(struct ammo *ammo){
    struct casing *new;
    if(!(new = malloc (sizeof(struct projectile)))){
        return NULL;
    }

    new->info.order = ammo->ammo_amt+1;
    new->info.speed_x = 0;
    new->info.speed_y = 0;
    new->info.x = 0;
    new->info.y = 0;

    return new;
}

struct projectile* clock_projectile(struct ammo *ammo, struct player player){
    struct casing *shoot = ammo->magazine;

}