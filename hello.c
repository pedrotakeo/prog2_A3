#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"

#define RIGHT     1
#define LEFT     2
#define UP    3
#define RUP    4
#define LUP    5

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}



int main(){
    struct player player;
    struct environment world;
    struct projectile proj;

    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");

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
    initialize_player_info(world, &player);
    player.sprite = al_load_bitmap("assets/kyara.png");
    must_init(player.sprite, "character");
    al_convert_mask_to_alpha(player.sprite, al_map_rgb(26, 255, 0));

    //projectile rules
    proj.og_dimensions = 32;
    proj.dimensions = 35;
    proj.x = 0;
    proj.y = player.y + player.og_dimensions/2;
    proj.speed_x = 40;
    proj.speed_y = 0;
    proj.shoot = false;
    proj.projectile = al_load_bitmap("assets/projectile.png");
    must_init(proj.projectile, "projectile");
    al_convert_mask_to_alpha(proj.projectile, al_map_rgb(26, 255, 0));

    must_init(al_init_primitives_addon(), "primitives");
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    ALLEGRO_KEYBOARD_STATE ks;
    ALLEGRO_MOUSE_STATE ms;

    al_start_timer(timer);
    while(1)
    {   
        if(proj.x >= world.screen_width || proj.x <= 0 || proj.y <= 0 || proj.y >= world.screen_height){
            proj.shoot = false;
            proj.x = player.x;
        }

        proj.x += proj.speed_x;
        proj.y += proj.speed_y;
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
                al_get_mouse_state(&ms);

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
                    if(!proj.shoot){
                        player.aim = LEFT;
                    }
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
                    if(!proj.shoot){
                        player.aim = RIGHT;
                    }
                    
                }

                if((al_key_down(&ks, ALLEGRO_KEY_SPACE)) && player.jump_enable){
                    player.jump(&player);
                }

                if(al_key_down(&ks, ALLEGRO_KEY_W) && !proj.shoot){
                    player.aim = UP;
                }
                if(al_key_down(&ks, ALLEGRO_KEY_W) && al_key_down(&ks, ALLEGRO_KEY_D) && !proj.shoot){
                    player.aim = RUP;
                }
                if(al_key_down(&ks, ALLEGRO_KEY_W) && al_key_down(&ks, ALLEGRO_KEY_A) && !proj.shoot){
                    player.aim = LUP;
                }

                if(al_mouse_button_down(&ms, 1)){
                    if(!proj.shoot){
                        proj.x = player.x+35;
                    }

            
                    proj.y = player.y + player.og_dimensions/2;
                    proj.shoot = true;
                    
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

                switch (player.aim)
                {
                case RIGHT:
                    proj.speed_x = 40;
                    proj.speed_y = 0;
                    
                    break;
                case LEFT:
                    proj.speed_x = -40;
                    proj.speed_y = 0;
                    break;
                case UP:
                    proj.speed_x = 0;
                    proj.speed_y = -40;
                    break;
                case RUP:
                    proj.speed_x = 25;
                    proj.speed_y = -25;
                    break;
                case LUP:
                    proj.speed_x = -25;
                    proj.speed_y = -25;
                    break;
                
                default:
                    break;
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

            //projectile
            if(proj.shoot){
                al_draw_scaled_bitmap(proj.projectile, 0, 0, proj.og_dimensions, proj.og_dimensions, proj.x, proj.y, proj.dimensions, proj.dimensions, 0);

            }
            
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