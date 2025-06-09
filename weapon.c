#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "game.h"
#include "weapon.h"


// BULLET LIST IMPLEMENTATION


struct weapon* create_weapon(){
    struct weapon *new;
    if(!(new = malloc (sizeof(struct weapon)))){
        return NULL;
    }
    new->cool_down = 0;
    new->ammo_amt = 0;
    new->dimensions = 35;
    new->og_dimensions = 32;
    
    
    new->first = NULL;

    return new;
}

struct bullet* create_projectile(struct weapon *weapon, struct player player){
    struct bullet *new;
    if(!(new = malloc (sizeof(struct bullet)))){
        return NULL;
    }

    new->info.order = weapon->ammo_amt+1;
    new->info.speed_x = 0;
    new->info.speed_y = 0;
    new->info.x = player.x;
    new->info.y = player.y;
    new->next = NULL;
    new->prev = NULL;
    new->info.timer = 0;
    new->info.shoot = true;

    return new;
}

int load_weapon(struct weapon *weapon, struct bullet *new, struct player player){
    if(!new){
        return weapon->ammo_amt;
    }

    new->info.x = player.x + 15;
    new->info.y = player.y + 40;
    new->info.timer = 0;
    new->info.order = weapon->ammo_amt + 1;
    new->next = NULL;
    new->prev = NULL;
    switch (player.aim){
        case RIGHT:
            new->info.speed_x = 40;
            new->info.speed_y = 0;
            break;
        case LEFT:
            new->info.speed_x = -40;
            new->info.speed_y = 0;
            break;
        case UP:
            new->info.speed_x = 0;
            new->info.speed_y = -40;
            break;
        case RUP:
            new->info.speed_x = 40;
            new->info.speed_y = -40;
            break;
        case LUP:
            new->info.speed_x = -40;
            new->info.speed_y = -40;
            break;
        case DOWN:
            new->info.speed_x = 0;
            new->info.speed_y = 40;
            break;
        case RDOWN:
            new->info.speed_x = 40;
            new->info.speed_y = 40;
            break;
        case LDOWN:
            new->info.speed_x = -40;
            new->info.speed_y = 40;
            break;
        default:
            new->info.speed_x = 0;
            new->info.speed_y = 0;
            break;
    }
    


    if(!weapon->first){
        weapon->first = new;
        weapon->last = new;
        weapon->ammo_amt++;

        return weapon->ammo_amt;
    }

    weapon->last->next = new;
    new->prev = weapon->last;
    weapon->last = new;
    weapon->ammo_amt++;
    return weapon->ammo_amt;

}

struct bullet *destroy_bullet(struct weapon *weapon, struct bullet* rem){
    if(weapon->first == NULL || weapon->last == NULL){
        return NULL;
    }

    if(weapon->first == weapon->last){
        weapon->first = NULL;
        weapon->last = NULL;
        weapon->ammo_amt--;

        return rem;
    }

    if(weapon->first == rem){
        weapon->first = rem->next;
        weapon->first->prev = NULL;
        weapon->ammo_amt--;


        return rem;
    }

    if(weapon->last == rem){
        weapon->last = weapon->last->prev;
        weapon->last->next = NULL;
        weapon->ammo_amt--;


        return rem;
    }


    rem->prev->next = rem->next;
    rem->next->prev = rem->prev;
    weapon->ammo_amt--;

    return rem;

}

struct weapon *destroy_weapon(struct weapon *weapon){
    if(!weapon){
        return NULL;
    }
    struct bullet *aux;
    while(weapon->first){
        aux = destroy_bullet(weapon, weapon->first);
        free(aux);
    }
    al_destroy_bitmap(weapon->projectile);
    free(weapon);
    return NULL;

}



void draw_bullet(struct weapon *weapon){
    struct bullet* aux = weapon->first;

    
    while(aux){
        
        al_draw_scaled_bitmap(weapon->projectile, 0, 0, weapon->og_dimensions, weapon->og_dimensions, aux->info.x, aux->info.y, weapon->dimensions, weapon->dimensions, 0);
        aux = aux->next;

    }

    return;
}