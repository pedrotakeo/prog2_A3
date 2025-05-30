#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"
#include "weapon.h"

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}



int main(){
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
    struct environment world; 
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
    struct player player;
    set_player_methods(&player);
    initialize_player_info(world, &player);
    player.sprite = al_load_bitmap("assets/kyara.png");
    must_init(player.sprite, "character");
    al_convert_mask_to_alpha(player.sprite, al_map_rgb(26, 255, 0));
    player.heart = al_load_bitmap("assets/life.png");
    must_init(player.heart, "heart");
    al_convert_mask_to_alpha(player.heart, al_map_rgb(26, 255, 0));

    //projectile BACKUP;
    struct weapon *backup;
    backup = create_weapon();
    must_init(backup, "weapon");
    backup->projectile = al_load_bitmap("assets/projectile.png");
    must_init(backup->projectile, "projectile");
    al_convert_mask_to_alpha(backup->projectile, al_map_rgb(26, 255, 0));

    for (int i = 0; i < 30; i++){
        struct bullet* aux = create_projectile(backup, player);
        load_weapon(backup, aux, player);
    }
    


    //projectile rules
    struct weapon *weapon;
    weapon = create_weapon();
    must_init(weapon, "weapon");
    weapon->projectile = al_load_bitmap("assets/projectile.png");
    must_init(weapon->projectile, "projectile");
    al_convert_mask_to_alpha(weapon->projectile, al_map_rgb(26, 255, 0));

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
        player.rgb[0] = 255;
        player.rgb[1] = 255;
        player.rgb[2] = 255;
        determine_universal_screen_limits(&world);
        determine_universal_player_pos(world, &player);

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

                //KILLS GAME ON "ESC"====================================================================================
                if(al_key_down(&ks, ALLEGRO_KEY_ESCAPE))
                    done = true;

                //PLAYER MOVES LEFT====================================================================================
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
                    player.aim = LEFT;
                }

                //PLAYER MOVES RIGHT====================================================================================
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
                    player.aim = RIGHT;
                    

                }

                //AIM ADJUSTMENT====================================================================================
                if(al_key_down(&ks, ALLEGRO_KEY_W)){
                    player.aim = UP;
                }
                if(al_key_down(&ks, ALLEGRO_KEY_W) && al_key_down(&ks, ALLEGRO_KEY_D)){
                    player.aim = RUP;
                }
                if(al_key_down(&ks, ALLEGRO_KEY_W) && al_key_down(&ks, ALLEGRO_KEY_A)){
                    player.aim = LUP;
                }

                //PLAYER JUMPS====================================================================================
                if((al_key_down(&ks, ALLEGRO_KEY_SPACE)) && player.jump_enable){
                    player.jump(&player);
                }

                player.y -= player.frame_jump;
                
                if(player.y >= world.floor){
                    player.y = world.floor;
                    player.frame_jump = 0;
                    player.jump_enable = true;
                    
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

                //SHOOTER LOGIC====================================================================================
                struct bullet* aux = weapon->first;
                struct bullet* temp;

                if(weapon->cool_down > 0){
                    weapon->cool_down++;
                }
            
                if(weapon->cool_down >= 4){
                    weapon->cool_down = 0;
                }
            
            
                while(aux){
                    temp = aux->next;
                    aux->info.x += aux->info.speed_x;
                    aux->info.y += aux->info.speed_y;
                    aux->info.timer++;
                    if(aux->info.timer >= 20){// se está dentro da tela
                        aux = destroy_bullet(weapon, aux);
                        free(aux);
                    }
            
                    aux = temp;
                }

                if(player.stamina <= 0){
                    if(player.stamina < 0){
                        player.stamina_recount = 0;
                    }
                    player.stamina_recount++;
                    if(player.stamina_recount >= MAX_STAMINA){
                        player.stamina_recount = MAX_STAMINA;
                        player.stamina = MAX_STAMINA;
                    }
                }

                if(al_mouse_button_down(&ms, 1) && weapon->cool_down == 0 && player.stamina > 0){
                    player.stamina--;
                    player.stamina_recount--;
                    if(backup->first){
                        weapon->cool_down++;
                        struct bullet* temp = destroy_bullet(backup, backup->first); // pega bala alocada da lista backup
                        load_weapon(weapon, temp, player);  //carrega na arma principal
    
                        struct bullet* aux = create_projectile(backup, player); 
                        load_weapon(backup, aux, player);  // faz refill na backup
                    }
                    
                }


                //PLAYER COLOR======================================================================================

                if((player.universal_x <= 2700 && player.universal_x >= 2100 || player.universal_x <= 2000 && player.universal_x >= 1400 || player.universal_x <= 4500 && player.universal_x >= 3900) && player.y > world.screen_height/2){
                    player.rgb[0] = 117;
                    player.rgb[1] = 79;
                    player.rgb[2] = 63;
                }

            
                redraw = true;  // set to be redrawn
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue))  // DESENHA A FRAME
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
            

            //STAMINA BAR
            al_draw_filled_rectangle(20, 60, (5 * MAX_STAMINA) + 20, 70, al_map_rgb(255, 0, 0));
            al_draw_filled_rectangle(20, 60, (5 * player.stamina_recount) + 20, 70, al_map_rgb(26, 255, 0));
        
    

            //LIFE
            for(int  i = 0; i < MAX_LIFE; i++){
                if(!player.life[i]){
                    break;
                }
                al_draw_scaled_bitmap(player.heart, 0, 0, 32, 32, (i*30)+20, 15, 30, 30, 0);
            }

            //projectile
            draw_bullet(weapon);

            //PARAMETERS FOR PLATFORM
            //if(00 - world.bkg_off_x >=0 && 800 - world.bkg_off_x <=world.screen_width){
                //al_draw_line(1000 - world.bkg_off_x, 0, 1000 - world.bkg_off_x, world.screen_height, al_map_rgb(66, 32, 34), 2);
            //}
        
           
            
            //player
            al_draw_tinted_scaled_bitmap(player.sprite, al_map_rgb(player.rgb[0], player.rgb[1], player.rgb[2]), player.sprite_off_x, player.sprite_off_y, player.og_dimensions, player.og_dimensions, player.x, player.y, player.dimensions, player.dimensions, 0);
            
            al_draw_text(font, al_map_rgb(35, 66, 32), 20, 50, 0, "STAMINA LEVEL");
            al_draw_text(font, al_map_rgb(66, 32, 34), 20, 5, 0, "LIFE FORCE");

            

            al_flip_display();
            redraw = false;
        }
        
        world.counter++;

    }

    weapon = destroy_weapon(weapon);
    backup = destroy_weapon(backup);
    al_destroy_bitmap(world.bkg);
    al_destroy_bitmap(player.sprite);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}