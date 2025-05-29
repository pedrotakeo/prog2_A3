#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"
#include "weapon.h"


struct weapon* create_weapon(){
    struct weapon *new;
    if(!(new = malloc (sizeof(struct weapon)))){
        return NULL;
    }
    new->ammo_amt = 0;
    new->dimensions = 10;
    new->og_dimensions = 32;
    
    
    new->first = NULL;

    return new;
}

struct bullet* create_projectile(struct weapon *weapon, struct player player){
    struct bullet *new;
    if(!(new = malloc (sizeof(struct projectile)))){
        return NULL;
    }

    new->info.order = weapon->ammo_amt+1;
    new->info.speed_x = 0;
    new->info.speed_y = 0;
    new->info.x = player.x;
    new->info.y = player.y;
    new->next = NULL;
    new->prev = NULL;

    return new;
}

int load_weapon(struct weapon *weapon, struct player player){
    struct bullet *new = create_projectile(weapon, player);
    if(!new){
        return weapon->ammo_amt;
    }

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

int destroy_bullet(struct weapon *weapon, struct bullet* rem){
    if(weapon->first == NULL || weapon->last == NULL){
        return weapon->ammo_amt;
    }

    if(weapon->first == weapon->last){
        weapon->first == NULL;
        weapon->last == NULL;
        weapon->ammo_amt--;

        free(rem);
        return weapon->ammo_amt;
    }

    if(weapon->first == rem){
        weapon->first == weapon->first->next;
        weapon->first->prev == NULL;
        weapon->ammo_amt--;

        free(rem);
        return weapon->ammo_amt;
    }

    if(weapon->last == rem){
        weapon->last = weapon->last->prev;
        weapon->last->next = NULL;
        weapon->ammo_amt--;

        free(rem);
        return weapon->ammo_amt;
    }


    rem->prev->next = rem->next;
    rem->next->prev = rem->prev;
    weapon->ammo_amt--;

    free(rem);
    return weapon->ammo_amt;

}

struct weapon *destroy_weapon(struct weapon *weapon){
    if(!weapon){
        return NULL;
    }

    while(weapon->first){
        destroy_bullet(weapon, weapon->first);
    }

    free(weapon);
    return NULL;

}

void update_bullet_trajectories(struct weapon *weapon, struct environment world){
    struct bullet* aux = weapon->first;
    struct bullet* temp;
    while(aux){
        temp = aux->next;
        aux->info.x += aux->info.speed_x;
        aux->info.y += aux->info.speed_y;
        if(aux->info.x <=  world.screen_width && aux->info.x >= 0 && aux->info.y <=  world.screen_height && aux->info.y >= 0){// se está dentro da tela
            destroy_bullet(weapon, aux);
        }

        aux = temp;
    }
    return;
}

void draw_bullet(struct weapon *weapon){
    struct bullet* aux = weapon->first;
    while(aux){
        al_draw_scaled_bitmap(weapon->projectile, 0, 0, weapon->og_dimensions, weapon->og_dimensions, aux->info.x, aux->info.y, weapon->dimensions, weapon->dimensions, 0);
        aux = aux->next;

    }

    return;
}