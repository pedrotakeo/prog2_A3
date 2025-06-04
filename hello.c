#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "game.h"
#include "weapon.h"
#include "enemy.h"

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}



int main(){
    int running_screen = MENU;

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

    //MENU SETTINGS=========================================================================
    int button_width_play = 0;
    int button_height_play = 0;
    int button_width_leave = 0;
    int button_height_leave = 0;

    ALLEGRO_BITMAP* bkg_menu = al_load_bitmap("assets/bkg_blur.png");
    must_init(bkg_menu, "blur"); 

    ALLEGRO_BITMAP* play_bt = al_load_bitmap("assets/play.png");
    must_init(play_bt, "play"); 
    al_convert_mask_to_alpha(play_bt, al_map_rgb(78, 255, 0));

    ALLEGRO_BITMAP* leave_bt = al_load_bitmap("assets/leave.png");
    must_init(leave_bt, "leave"); 
    al_convert_mask_to_alpha(leave_bt, al_map_rgb(78, 255, 0));

    ALLEGRO_BITMAP* main_title = al_load_bitmap("assets/main_title.png");
    must_init(main_title, "maintitle"); 
    al_convert_mask_to_alpha(main_title, al_map_rgb(26, 255, 0));

    ALLEGRO_BITMAP* game_over = al_load_bitmap("assets/game_over.png");
    must_init(game_over, "game_over"); 
    al_convert_mask_to_alpha(game_over, al_map_rgb(26, 255, 0));



    //GAME SETTINGS=========================================================================
    struct environment world; 
    initialize_world_info(&world);
    world.bkg = al_load_bitmap("assets/bkg.png");
    must_init(world.bkg, "bkg");

    ALLEGRO_DISPLAY* disp = al_create_display(world.screen_width, world.screen_height);
    must_init(disp, "display");

    float og_floor = world.floor;

    //setting player rules======================================================================================================
    struct player player;
    set_player_methods(&player);
    initialize_player_info(world, &player);
    player.sprite = al_load_bitmap("assets/kyara.png");
    must_init(player.sprite, "character");
    al_convert_mask_to_alpha(player.sprite, al_map_rgb(26, 255, 0));
    player.heart = al_load_bitmap("assets/life.png");
    must_init(player.heart, "heart");
    al_convert_mask_to_alpha(player.heart, al_map_rgb(26, 255, 0));


    //projectile BACKUP======================================================================================================
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
    
    //projectile rules======================================================================================================
    struct weapon *weapon;
    weapon = create_weapon();
    must_init(weapon, "weapon");
    weapon->projectile = al_load_bitmap("assets/projectile.png");
    must_init(weapon->projectile, "projectile");
    al_convert_mask_to_alpha(weapon->projectile, al_map_rgb(26, 255, 0));

    //ENEMY RULES======================================================================================================
    struct horde horde;
    initialize_enemy_info(world, &horde);
    horde.enemy_sprite = al_load_bitmap("assets/enemy.png");
    must_init(horde.enemy_sprite, "enemy");
    al_convert_mask_to_alpha(horde.enemy_sprite, al_map_rgb(26, 255, 0));

    struct boss boss;
    initialize_boss_info(&boss);
    boss.enemy_sprite = al_load_bitmap("assets/boss.png");
    must_init(boss.enemy_sprite, "boss");
    al_convert_mask_to_alpha(boss.enemy_sprite, al_map_rgb(26, 255, 0));

    ALLEGRO_BITMAP* bkg_boss = al_load_bitmap("assets/bkg_boss.png");
    must_init(bkg_boss, "bkg_boss"); 

    ALLEGRO_BITMAP* bosstxt = al_load_bitmap("assets/bosstxt.png");
    must_init(bosstxt, "bosstxt"); 
    al_convert_mask_to_alpha(bosstxt, al_map_rgb(26, 255, 0));

    ALLEGRO_BITMAP* t3 = al_load_bitmap("assets/t3.png");
    must_init(t3, "t3"); 
    al_convert_mask_to_alpha(t3, al_map_rgb(26, 255, 0));

    ALLEGRO_BITMAP* t2 = al_load_bitmap("assets/t2.png");
    must_init(t2, "t2"); 
    al_convert_mask_to_alpha(t2, al_map_rgb(26, 255, 0));

    ALLEGRO_BITMAP* t1 = al_load_bitmap("assets/t1.png");
    must_init(t1, "t1"); 
    al_convert_mask_to_alpha(t1, al_map_rgb(26, 255, 0));

    ALLEGRO_BITMAP* go = al_load_bitmap("assets/go.png");
    must_init(go, "go"); 
    al_convert_mask_to_alpha(go, al_map_rgb(26, 255, 0));

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
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
        
            case ALLEGRO_EVENT_TIMER:
                if(running_screen == MENU){
                    al_get_keyboard_state(&ks);
                    al_get_mouse_state(&ms);

                    button_width_play = 200;
                    button_height_play = 67;
                    button_width_leave = 200;
                    button_height_leave = 67;

                    if((ms.x >= ((world.screen_width/2) - 100) && ms.x <= ((world.screen_width/2) + 100) && ms.y >= 460 && ms.y <= 527)){
                        if(!al_mouse_button_down(&ms, 1)){
                            button_width_play = 220;
                            button_height_play = 73;
                        }
                        else{
                            running_screen = GAME;
                            world.counter = 0;
                        }
                    }

                    if((ms.x >= ((world.screen_width/2) - 100) && ms.x <= ((world.screen_width/2) + 100) && ms.y >= 550 && ms.y <= 617)){
                        if(!al_mouse_button_down(&ms, 1)){
                            button_width_leave = 220;
                            button_height_leave = 73;
                        }
                        else{
                            done = true;
                            break;
                        }
                    }

                    if(al_key_down(&ks, ALLEGRO_KEY_ENTER)){
                        running_screen = GAME;
                        world.counter = 0;
                    }

                    redraw = true;
                }

                else if(running_screen == GAME_OVER){
                    al_get_keyboard_state(&ks);
                    al_get_mouse_state(&ms);

                    button_width_leave = 200;
                    button_height_leave = 67;

                    if((ms.x >= ((world.screen_width/2) - 100) && ms.x <= ((world.screen_width/2) + 100) && ms.y >= 500 && ms.y <= 567)){
                        if(!al_mouse_button_down(&ms, 1)){
                            button_width_leave = 220;
                            button_height_leave = 73;
                        }
                        else{
                            done = true;
                            break;
                        }
                    }

                    redraw = true;
                }

                else if(running_screen == GAME){
                    al_get_keyboard_state(&ks);
                    al_get_mouse_state(&ms);

                    // RESETA ALTERAÇOES TEMPORARIAS
                    reset_info(&world, &player, &ms);

                    //OPENS MENU ON "ESC"====================================================================================
                    pause_game(&ks, &running_screen);

                    //PLAYER MOVES LEFT====================================================================================
                    player.move(&world, &player, &ks, &ms, running_screen);

                    //AIM ADJUSTMENT====================================================================================
                    player.aim_adjust(&player, &ks);

                    //PLAYER JUMPS====================================================================================
                    player.jump(&world, &player, &ks, &og_floor);

                    //PLAYER DUCK======================================================================================
                    player.duck(&player, &ks);

                    //SHOOTER LOGIC====================================================================================
    
                    player.shoot(&world, &player, weapon, backup, &ks, &ms);

                    //PLAYER COLOR and LEVEL FLOOR======================================================================================

                    player.state(&world, &player);

                    //ENEMY STUFF===============================================================================================

                    update_enemy_pos(world, &horde);
                    enemy_logic(world, player, &horde);

                    player_to_enemy_damage(&world, &player, weapon, &horde);
                    enemy_to_player_damage(&world, &player, weapon, &horde, &running_screen);

                    if(horde.enemies_remaining == 0 || al_key_down(&ks, ALLEGRO_KEY_B)){
                        player.aim = RIGHT;
                        running_screen = GAME_BOSS_STATE;
                        player.x = 100;
                        player.y = world.screen_height/2;
                        world.counter = 0;
                        boss.rgb[0] = 255;
                        boss.rgb[1] = 255;
                        boss.rgb[2] = 255;
                    }
            
                    redraw = true; // set to be redrawn
                }

                else if(running_screen == GAME_BOSS_STATE){
                    al_get_keyboard_state(&ks);
                    al_get_mouse_state(&ms);
                    // RESETA ALTERAÇOES TEMPORARIAS
                    boss.rgb[0] = 255;
                    boss.rgb[1] = 255;
                    boss.rgb[2] = 255;
                    
                    boss.timer++;

                    switch(boss.timer){
                        case 10:
                            boss.y = 50;
                            break;
                        case 20:
                            boss.y = 100;
                            break;
                        case 30:
                            boss.y = 150;
                            break;
                        case 40:
                            boss.y = 100;
                            break;
                        case 50:
                            boss.y = 50;
                            break;
                        case 60:
                            boss.y = 0;
                            boss.timer = 0;
                            break;
                    }
                    
                    if(world.counter > 120){
                        reset_info(&world, &player, &ms);

                        //OPENS MENU ON "ESC"====================================================================================
                        pause_game(&ks, &running_screen);

                        //PLAYER MOVES LEFT====================================================================================
                        player.move(&world, &player, &ks, &ms, running_screen);

    
                        //SHOOTER LOGIC====================================================================================

                        player.shoot(&world, &player, weapon, backup, &ks, &ms);

                        //BOSS STUFF===============================================================================================

                        player_to_boss_damage(world, &boss, weapon);
                        //enemy_to_player_damage(&world, &player, weapon, &horde, &running_screen);
                        if(boss.life == 0){
                            running_screen = GAME_OVER;
                        }
                    }
                    
            
                    redraw = true; // set to be redrawn
                }
                
                
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue))  // DESENHA A FRAME
        {
            //MENU SCREEN
            if(running_screen ==  MENU){
                al_clear_to_color(al_map_rgb(255, 255, 255));

                al_draw_scaled_bitmap(bkg_menu, 0, 0, 1280, 720, 0, 0, world.screen_width, world.screen_height, 0);
                
                al_draw_text(font, al_map_rgb(255, 255, 255), world.screen_width - 320, world.screen_height - 12, 0, "Ken no subarashii sabaku taikai - V1.0");

                al_draw_scaled_bitmap(play_bt, 0, 0, 120, 40, (world.screen_width/2) - (button_width_play/2), 460, button_width_play, button_height_play, 0);
                al_draw_scaled_bitmap(leave_bt, 0, 0, 120, 40, (world.screen_width/2) - (button_width_leave/2), 550, button_width_leave, button_height_leave, 0);

                al_draw_scaled_bitmap(main_title, 0, 0, 448, 256, (world.screen_width/2) - 250, (world.screen_width/2) - 550, 500, 286, 0);
                
            }

            //GAME_OVER SCREEN
            if(running_screen ==  GAME_OVER){
                al_clear_to_color(al_map_rgb(0, 0, 0));

                al_draw_scaled_bitmap(leave_bt, 0, 0, 120, 40, (world.screen_width/2) - (button_width_leave/2), 500, button_width_leave, button_height_leave, 0);

                al_draw_scaled_bitmap(game_over,0, 0, 512, 128, 0, (world.screen_height/2) - 160, world.screen_width, 320, 0);                
                
            }

            //GAME SCREEN
            if(running_screen ==  GAME){
                

                if(player.direction == RIGHT){
                    player.sprite_off_y = 64;
                }
                else{
                    player.sprite_off_y = 0;
                }

            
                al_clear_to_color(al_map_rgb(57, 159, 251));

                //bkg
                al_draw_scaled_bitmap(world.bkg, world.bkg_off_x, 0, ((world.bkg_img_og_height * world.screen_width)/world.screen_height), world.bkg_img_og_height, 0, 0, world.screen_width, world.screen_height, 0);
            
                al_draw_textf(font, al_map_rgb(0, 55, 200), world.screen_width - 220, 15, 0, "ENEMIES REMANING: %1d/%1d ", horde.enemies_remaining, ENEMY_AMT);

                //STAMINA BAR
                al_draw_filled_rectangle(20, 50, (5 * MAX_STAMINA) + 20, 60, al_map_rgb(255, 0, 0));
                if(player.stamina == 0){
                    al_draw_filled_rectangle(20, 50, (5 * player.stamina_recount) + 20, 60, al_map_rgb(71, 21, 22));
                }
                else{
                    al_draw_filled_rectangle(20, 50, (5 * player.stamina_recount) + 20, 60, al_map_rgb(26, 255, 0));
                }
                
        
                //LIFE
                for(int  i = 0; i < MAX_LIFE; i++){  //HEART CONTAINERS
                    al_draw_tinted_scaled_bitmap(player.heart, al_map_rgb(71, 21, 22), 0, 0, 32, 32, (i*30)+20, 10, 30, 30, 0);
                }

                for(int  i = 0; i < player.life; i++){ // ACTUAL HEARTS
                    al_draw_scaled_bitmap(player.heart, 0, 0, 32, 32, (i*30)+20, 10, 30, 30, 0);
                }
                
                //projectile
                draw_bullet(weapon);

                                          
                //player
                al_draw_tinted_scaled_bitmap(player.sprite, al_map_rgb(player.rgb[0], player.rgb[1], player.rgb[2]), player.sprite_off_x, player.sprite_off_y, player.og_dimensions, player.og_dimensions, player.x, player.y, player.dimensions, player.dimensions, 0);
                
                //ENEMIES
                for(int i = 0; i < ENEMY_AMT; i++){
                    if(horde.enemy[i].state == ALIVE){
                        al_draw_tinted_scaled_bitmap(horde.enemy_sprite, al_map_rgb(horde.enemy[i].rgb[0], horde.enemy[i].rgb[1], horde.enemy[i].rgb[2]), horde.enemy[i].sprite_off_x, horde.enemy[i].sprite_off_y, player.og_dimensions, player.og_dimensions, horde.enemy[i].x, horde.enemy[i].y, player.dimensions, player.dimensions, 0);
                        //if(horde.enemy[i].bullet.shoot){
                            //al_draw_scaled_bitmap(enemy_tama, 0, 0, 32, 32, horde.enemy[i].bullet.x, horde.enemy[i].bullet.y, 32, 32, 0);
                        //}
                    }
                   
                }

            }

            if(running_screen ==  GAME_BOSS_STATE){
                
                player.sprite_off_y = 64;
                player.sprite_off_x = 448;
               
            
                al_clear_to_color(al_map_rgb(57, 159, 251));

                //bkg
                al_draw_scaled_bitmap(bkg_boss, world.bkg_off_x, 0, ((world.bkg_img_og_height * world.screen_width)/world.screen_height), world.bkg_img_og_height, 0, 0, world.screen_width, world.screen_height, 0);
            

                //STAMINA BAR
                al_draw_filled_rectangle(20, 50, (5 * MAX_STAMINA) + 20, 60, al_map_rgb(255, 0, 0));
                if(player.stamina == 0){
                    al_draw_filled_rectangle(20, 50, (5 * player.stamina_recount) + 20, 60, al_map_rgb(71, 21, 22));
                }
                else{
                    al_draw_filled_rectangle(20, 50, (5 * player.stamina_recount) + 20, 60, al_map_rgb(26, 255, 0));
                }
                
        
                //LIFE
                for(int  i = 0; i < MAX_LIFE; i++){  //HEART CONTAINERS
                    al_draw_tinted_scaled_bitmap(player.heart, al_map_rgb(71, 21, 22), 0, 0, 32, 32, (i*30)+20, 10, 30, 30, 0);
                }

                for(int  i = 0; i < player.life; i++){ // ACTUAL HEARTS
                    al_draw_scaled_bitmap(player.heart, 0, 0, 32, 32, (i*30)+20, 10, 30, 30, 0);
                }
                
                //projectile
                draw_bullet(weapon);

                                          
                //player
                al_draw_tinted_scaled_bitmap(player.sprite, al_map_rgb(player.rgb[0], player.rgb[1], player.rgb[2]), player.sprite_off_x, player.sprite_off_y, player.og_dimensions, player.og_dimensions, player.x, player.y, player.dimensions, player.dimensions, 0);
                
                if(boss.life > 0){
                    al_draw_tinted_scaled_bitmap(boss.enemy_sprite, al_map_rgb(boss.rgb[0],boss.rgb[1],boss.rgb[2]), 0, 0, 64, 256, world.screen_width - 192, boss.y, 128, 4*128, 0);
                }

                if(world.counter < 30){
                    if(world.counter % 2){
                        al_draw_scaled_bitmap(bosstxt,0, 0, 512, 128, 0, (world.screen_height/2) - 160, world.screen_width, 320, 0);
                    }
                    else{
                        al_draw_tinted_scaled_bitmap(bosstxt, al_map_rgb(255, 0, 0),0, 0, 512, 128, 0, (world.screen_height/2) - 160, world.screen_width, 320, 0);
                    }
                }
                if(world.counter > 30 && world.counter < 60){
                    al_draw_scaled_bitmap(t3,0, 0, 512, 128, 0, (world.screen_height/2) - 160, world.screen_width, 320, 0);
                }
                if(world.counter > 60 && world.counter < 90){
                    al_draw_scaled_bitmap(t2,0, 0, 512, 128, 0, (world.screen_height/2) - 160, world.screen_width, 320, 0);
                }
                if(world.counter > 90 && world.counter < 120){
                    al_draw_scaled_bitmap(t1,0, 0, 512, 128, 0, (world.screen_height/2) - 160, world.screen_width, 320, 0);
                }

                if(world.counter > 120 && world.counter < 150){
                    al_draw_scaled_bitmap(go,0, 0, 512, 128, 0, (world.screen_height/2) - 160, world.screen_width, 320, 0);
                }
            }
            
            

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