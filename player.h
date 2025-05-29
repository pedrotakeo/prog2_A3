#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#define RIGHT     1
#define LEFT     2
#define UP    3
#define RUP    4
#define LUP    5


struct player{
    float x;
    float y;
    int og_dimensions;
    int dimensions;
    int sprite_off_x;
    int sprite_off_y;
    int speed;
    int direction;
    int jump_enable;
    float frame_jump;

    int aim;

    int shadow_x;
    int shadow_y;
    int shadow_height;
    int shadow_width;
    int shadow_mod;

    ALLEGRO_BITMAP *sprite;

    void (*move)(struct player*, int);

    void (*jump)(struct player*);
};

struct environment{
    int shadow;
    int screen_width;
    int screen_height;
    float floor;
    int counter;
    int air_round;

    int bkg_off_x;
    int bkg_img_og_width;
    int bkg_img_og_height;

    float gravity;

    ALLEGRO_BITMAP *bkg;
};

void initialize_player_info(struct environment world, struct player *player);

void set_player_methods(struct player *player);

void move_player( struct player *player, int direction);

void jump_player(struct player* player);


#endif