#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"

#define RIGHT     1
#define LEFT     2

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}



int main(){
    struct player player;
    struct environment world;

    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_image_addon(), "image addon");

    //setting world rules
    world.shadow = 7;
    world.gravity = 40;
    world.screen_width = 1280;
    world.screen_height = 720;
    world.floor = world.screen_height - 175;
    world.counter = 0;
    world.bkg_off_x = 0;
    world.bkg_img_og_height = 256;
    world.bkg_img_og_width = 2048;
    world.air_round = 0;
    world.bkg = al_load_bitmap("assets/bkg.png");
    must_init(world.bkg, "bkg");

    ALLEGRO_DISPLAY* disp = al_create_display(world.screen_width, world.screen_height);
    must_init(disp, "display");

    //setting player rules
    set_player_methods(&player);
    player.x = 200;
    player.y = world.floor;
    player.og_dimensions = 64;
    player.dimensions = 100;
    player.speed = 20;
    player.direction = RIGHT;
    player.frame_jump = 0;
    player.jump_enable = true; //jump enabled
    player.sprite_off_x = 0;
    player.sprite_off_y = 64;
    player.shadow_height = 7;
    player.shadow_width = 21;
    player.shadow_mod = 0;
    player.sprite = al_load_bitmap("assets/kyara.png");
    must_init(player.sprite, "kyara");
    al_convert_mask_to_alpha(player.sprite, al_map_rgb(26, 255, 0));

    must_init(al_init_primitives_addon(), "primitives");
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    ALLEGRO_KEYBOARD_STATE ks;

    al_start_timer(timer);
    while(1)
    {   
        player.sprite_off_x = 0;
        if(player.direction == RIGHT){
            player.sprite_off_y = 64;
        }
        else{
            player.sprite_off_y = 0;
        }
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
        
            case ALLEGRO_EVENT_TIMER:
                al_get_keyboard_state(&ks);

                if(player.x >= player.speed && (al_key_down(&ks, ALLEGRO_KEY_LEFT) || al_key_down(&ks, ALLEGRO_KEY_A))){

                    if(player.x > 100 || world.bkg_off_x == 0){
                        player.move(&player, LEFT);
                    }else{
                        if(world.bkg_off_x > 0){
                            world.bkg_off_x -= 10;
                        }
                        else{
                            world.bkg_off_x = 0;
                        }
                    }

                    player.sprite_off_x = 64;
                    if(world.counter % 2){
                        player.sprite_off_x = 128;
                    }
                    player.direction = LEFT;
                }

                if(player.x <= (world.screen_width - player.speed - player.dimensions) && (al_key_down(&ks, ALLEGRO_KEY_RIGHT) || al_key_down(&ks, ALLEGRO_KEY_D))){

                    if(player.x < (world.screen_width - 200) || world.bkg_off_x == (2048 - 456)){
                        player.move(&player, RIGHT);
                    }else{
                        if(world.bkg_off_x < (2048 - 456)){
                            world.bkg_off_x += 10;
                        }
                        else{
                            world.bkg_off_x = 2048 - 456;
                        }
                    }

                    player.sprite_off_x = 64;
                    if(world.counter % 2){
                        player.sprite_off_x = 128;
                    }
                    player.direction = RIGHT;
                }

                if((al_key_down(&ks, ALLEGRO_KEY_SPACE) || al_key_down(&ks, ALLEGRO_KEY_W)) && player.jump_enable){
                    player.jump(&player);
                }

                if(al_key_down(&ks, ALLEGRO_KEY_ESCAPE))
                    done = true;
        
                player.y -= player.frame_jump;
                
                if(player.y >= world.floor){
                    player.y = world.floor;
                    player.frame_jump = 0;
                    player.jump_enable = true;
                    
                }

                if (player.y <= world.floor){
                    player.shadow_mod = (player.y - world.floor) / 100;
                }

                if (player.y <= world.floor - (8 * player.frame_jump)){
                    world.air_round++;
                    if(al_key_down(&ks, ALLEGRO_KEY_SPACE) && world.air_round < 30){
                        player.frame_jump = 0;
                    }
                    else{ 
                        player.frame_jump = -world.gravity;
                    }

                }
 
   

                player.shadow_x = player.x + (player.dimensions/2);
                player.shadow_y = world.floor + 93;
                player.shadow_height = world.shadow + player.shadow_mod;
                player.shadow_width = (player.shadow_height * 3);
                redraw = true;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue))
        {
            if(player.direction == RIGHT){
                player.sprite_off_y = 64;
            }
            else{
                player.sprite_off_y = 0;
            }
            ALLEGRO_VERTEX v[4];
            al_clear_to_color(al_map_rgb(57, 159, 251));

            //bkg
            al_draw_scaled_bitmap(world.bkg, world.bkg_off_x, 0, ((world.bkg_img_og_height * world.screen_width)/world.screen_height), world.bkg_img_og_height, 0, 0, world.screen_width, world.screen_height, 0);
            
            //player shadow
            al_draw_filled_ellipse(player.shadow_x, player.shadow_y, player.shadow_width, player.shadow_height, al_map_rgba_f(0, 0, 0, 0.5));
            
            //player
            al_draw_scaled_bitmap(player.sprite, player.sprite_off_x, player.sprite_off_y, player.og_dimensions, player.og_dimensions, player.x, player.y, player.dimensions, player.dimensions, 0);
            al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %3.1f Y: %3.1f", player.x, player.y);
            

            al_flip_display();
            redraw = false;
        }
        
        world.counter++;

    }

    al_destroy_bitmap(world.bkg);
    al_destroy_bitmap(player.sprite);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}