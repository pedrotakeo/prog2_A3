#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "weapon.h"
#include "player.h"
struct weapon;

#define RIGHT     1
#define LEFT     2
#define UP    3
#define RUP    4
#define LUP    5
#define MAX_STAMINA 50
#define MAX_LIFE 10


struct player{
    float x;
    float y;
    float universal_x;
    int og_dimensions;
    int dimensions;
    int collision_height;
    int sprite_off_x;
    int sprite_off_y;
    int speed;
    int direction;
    int jump_enable;
    float frame_jump;

    int rgb[3];

    int aim;
    int stamina;
    int stamina_recount;

    bool life[MAX_LIFE];


    ALLEGRO_BITMAP *sprite;
    ALLEGRO_BITMAP *heart;

    void (*move)(struct player*, int);

    void (*jump)(struct player*);
};

struct environment{
    int screen_width;
    int screen_height;
    float floor;
    int counter;
    int air_round;

    int screen_limit_L;
    int screen_limit_R;

    int bkg_off_x;
    int bkg_img_og_width;
    int bkg_img_og_height;

    float gravity;

    ALLEGRO_BITMAP *bkg;
};

void determine_universal_screen_limits(struct environment *world);

void determine_universal_player_pos(struct environment world, struct player *player);

void initialize_world_info(struct environment *world);

void initialize_player_info(struct environment world, struct player *player);

void set_player_methods(struct player *player);

void move_player( struct player *player, int direction);

void jump_player(struct player* player);

void game_logic(struct environment *world, struct player *player, struct weapon *weapon, struct weapon *backup, ALLEGRO_KEYBOARD_STATE *ks, ALLEGRO_MOUSE_STATE *ms, int *running_screen, float *og_floor);


#endif