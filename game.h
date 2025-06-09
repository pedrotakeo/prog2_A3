#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "weapon.h"
#include "game.h"
#include "enemy.h"

struct horde;
struct weapon;
struct boss;

#define RIGHT     1
#define LEFT     2
#define UP    3
#define RUP    4
#define LUP    5
#define DOWN    6
#define RDOWN    7
#define LDOWN    8
#define MAX_STAMINA 50
#define MAX_LIFE 10

#define MENU 0
#define GAME 1
#define GAME_OVER 2
#define GAME_BOSS_STATE 3
#define YOU_WIN 4


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

struct player{  //PLAYER CLASS
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
    int life;
    int damage_cooldown;

    int rgb[3];

    int aim;
    int stamina;
    int stamina_recount;


    ALLEGRO_BITMAP *sprite;
    ALLEGRO_BITMAP *heart;

    void (*move)(struct environment*, struct player*, ALLEGRO_KEYBOARD_STATE*, ALLEGRO_MOUSE_STATE*, int, struct horde*);

    void (*jump)(struct environment*, struct player*, ALLEGRO_KEYBOARD_STATE*, float*);

    void (*univ_pos)(struct environment, struct player*);

    void (*aim_adjust)(struct player*, ALLEGRO_KEYBOARD_STATE*);
    
    void (*duck)(struct player*, ALLEGRO_KEYBOARD_STATE*);

    void (*shoot)(struct environment*, struct player*, struct weapon*, struct weapon*, ALLEGRO_KEYBOARD_STATE*, ALLEGRO_MOUSE_STATE*);

    void (*state)(struct environment*, struct player*);
};

struct heart_object{
    float x;
    float y;
    int size;

    int available;
};

void initialize_world_info(struct environment *world);

void initialize_player_info(struct environment world, struct player *player);

void reset_info(struct environment *world, struct player *player, ALLEGRO_MOUSE_STATE *ms);

void pause_game(ALLEGRO_KEYBOARD_STATE *ks, int *running_screen);

void determine_universal_screen_limits(struct environment *world);

void initialize_heart_object(struct environment world, struct heart_object *heart);

// COMBAT STUFF

void player_to_enemy_damage(struct environment *world, struct player *player, struct weapon *weapon, struct horde *horde);

void enemy_to_player_damage(struct environment *world, struct player *player, struct horde *horde, int *running_screen);

void player_to_boss_damage(struct environment world, struct boss *boss, struct weapon *weapon);

void boss_to_player_damage(struct environment *world, struct player *player, struct boss *boss, int *running_screen);

void player_heart_collision(struct environment world, struct player *player, struct heart_object *heart);


//PLAYER STUFF
void set_player_methods(struct player *player);

void determine_universal_screen_limits(struct environment *world);

void determine_universal_player_pos(struct environment world, struct player *player);

void move_player(struct environment *world, struct player *player, ALLEGRO_KEYBOARD_STATE *ks, ALLEGRO_MOUSE_STATE *ms, int game_state, struct horde *horde);

void jump_player(struct environment *world, struct player *player, ALLEGRO_KEYBOARD_STATE *ks, float *og_floor);

void adjust_player_aim(struct player *player, ALLEGRO_KEYBOARD_STATE *ks);

void duck_player(struct player *player, ALLEGRO_KEYBOARD_STATE *ks);

void player_shoot(struct environment *world, struct player *player, struct weapon *weapon, struct weapon *backup, ALLEGRO_KEYBOARD_STATE *ks, ALLEGRO_MOUSE_STATE *ms);

void player_state_dependent_operations(struct environment *world, struct player *player);




#endif