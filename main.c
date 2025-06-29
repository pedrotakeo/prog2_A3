#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "game.h"
#include "weapon.h"
#include "enemy.h"


int main(){
    int running_screen = MENU;

    al_init();
    al_install_keyboard();
    al_install_mouse();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    ALLEGRO_FONT* font = al_create_builtin_font();

    al_init_image_addon();

    //MENU SETTINGS=========================================================================
    int button_width_play = 0;
    int button_height_play = 0;
    int button_width_leave = 0;
    int button_height_leave = 0;

    ALLEGRO_BITMAP* bkg_menu = al_load_bitmap("assets/bkg_blur.png");

    ALLEGRO_BITMAP* play_bt = al_load_bitmap("assets/play.png");
    al_convert_mask_to_alpha(play_bt, al_map_rgb(78, 255, 0));

    ALLEGRO_BITMAP* leave_bt = al_load_bitmap("assets/leave.png");
    al_convert_mask_to_alpha(leave_bt, al_map_rgb(78, 255, 0));

    ALLEGRO_BITMAP* main_title = al_load_bitmap("assets/main_title.png");
    al_convert_mask_to_alpha(main_title, al_map_rgb(26, 255, 0));

    ALLEGRO_BITMAP* game_over = al_load_bitmap("assets/game_over.png");
    al_convert_mask_to_alpha(game_over, al_map_rgb(26, 255, 0));

    ALLEGRO_BITMAP* win_bkg = al_load_bitmap("assets/win.png");



    //GAME SETTINGS=========================================================================
    struct environment world; 
    initialize_world_info(&world);
    world.bkg = al_load_bitmap("assets/bkg.png");

    ALLEGRO_DISPLAY* disp = al_create_display(world.screen_width, world.screen_height);

    float og_floor = world.floor;

    //setting player rules======================================================================================================
    struct player player;
    set_player_methods(&player);
    initialize_player_info(world, &player);

    player.sprite = al_load_bitmap("assets/kyara.png");
    al_convert_mask_to_alpha(player.sprite, al_map_rgb(26, 255, 0));

    player.heart = al_load_bitmap("assets/life.png");
    al_convert_mask_to_alpha(player.heart, al_map_rgb(26, 255, 0));

    struct heart_object heart;
    initialize_heart_object(world, &heart);


    //projectile BACKUP======================================================================================================
    struct weapon *backup;
    backup = create_weapon();

    backup->projectile = al_load_bitmap("assets/projectile.png");
    al_convert_mask_to_alpha(backup->projectile, al_map_rgb(26, 255, 0));

    for (int i = 0; i < 30; i++){
        struct bullet* aux = create_projectile(backup, player);
        load_weapon(backup, aux, player);
    }
    
    //projectile rules======================================================================================================
    struct weapon *weapon;
    weapon = create_weapon();

    weapon->projectile = al_load_bitmap("assets/projectile.png");
    al_convert_mask_to_alpha(weapon->projectile, al_map_rgb(26, 255, 0));

    //ENEMY RULES======================================================================================================
    struct horde horde;
    initialize_enemy_info(world, &horde);

    horde.enemy_sprite = al_load_bitmap("assets/enemy.png");
    al_convert_mask_to_alpha(horde.enemy_sprite, al_map_rgb(26, 255, 0));

    horde.proj_sprite = al_load_bitmap("assets/proj_enemy.png");
    al_convert_mask_to_alpha(horde.proj_sprite, al_map_rgb(26, 255, 0));

    struct boss boss;
    initialize_boss_info(&boss, world);

    boss.enemy_sprite = al_load_bitmap("assets/boss.png");
    al_convert_mask_to_alpha(boss.enemy_sprite, al_map_rgb(26, 255, 0));

    ALLEGRO_BITMAP* bkg_boss = al_load_bitmap("assets/bkg_boss.png");

    ALLEGRO_BITMAP* bosstxt = al_load_bitmap("assets/bosstxt.png");
    al_convert_mask_to_alpha(bosstxt, al_map_rgb(26, 255, 0));

    ALLEGRO_BITMAP* t3 = al_load_bitmap("assets/t3.png");
    al_convert_mask_to_alpha(t3, al_map_rgb(26, 255, 0));

    ALLEGRO_BITMAP* t2 = al_load_bitmap("assets/t2.png");
    al_convert_mask_to_alpha(t2, al_map_rgb(26, 255, 0));

    ALLEGRO_BITMAP* t1 = al_load_bitmap("assets/t1.png");
    al_convert_mask_to_alpha(t1, al_map_rgb(26, 255, 0));

    ALLEGRO_BITMAP* go = al_load_bitmap("assets/go.png");
    al_convert_mask_to_alpha(go, al_map_rgb(26, 255, 0));

    al_init_primitives_addon();
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
                //MENU ======================================================================================
                if(running_screen == MENU){  // main menu and puse screen
                    al_get_keyboard_state(&ks);
                    al_get_mouse_state(&ms);

                    button_width_play = 200;
                    button_height_play = 67;
                    button_width_leave = 200;
                    button_height_leave = 67;

                    if((ms.x >= ((world.screen_width/2) - 100) && ms.x <= ((world.screen_width/2) + 100) && ms.y >= 460 && ms.y <= 527)){
                        if(!al_mouse_button_down(&ms, 1)){ //not pressed
                            button_width_play = 220;
                            button_height_play = 73;
                        }
                        else{
                            running_screen = GAME;
                            world.counter = 0;
                        }
                    }

                    if((ms.x >= ((world.screen_width/2) - 100) && ms.x <= ((world.screen_width/2) + 100) && ms.y >= 550 && ms.y <= 617)){
                        if(!al_mouse_button_down(&ms, 1)){  //not pressed
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
                //YOU LOSE ======================================================================================
                else if(running_screen == GAME_OVER){  // bad ending (LOSS)
                    al_get_keyboard_state(&ks);
                    al_get_mouse_state(&ms);

                    button_width_leave = 200;
                    button_height_leave = 67;

                    if((ms.x >= ((world.screen_width/2) - 100) && ms.x <= ((world.screen_width/2) + 100) && ms.y >= 500 && ms.y <= 567) || (al_key_down(&ks, ALLEGRO_KEY_ESCAPE)) && world.counter > 30){

                        if(!al_mouse_button_down(&ms, 1)){  //not pressed
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
                //YOU WIN ======================================================================================
                else if(running_screen == YOU_WIN){  // good ending (WIN)
                    al_get_keyboard_state(&ks);
                    al_get_mouse_state(&ms);

                    button_width_leave = 200;
                    button_height_leave = 67;

                    if((ms.x >= ((world.screen_width/2) - 100) && ms.x <= ((world.screen_width/2) + 100) && ms.y >= 500 && ms.y <= 567) || (al_key_down(&ks, ALLEGRO_KEY_ESCAPE)) && world.counter > 30){
                        if(!al_mouse_button_down(&ms, 1)){ //not pressed
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
                //GAME ======================================================================================
                else if(running_screen == GAME){  // main stage
                    al_get_keyboard_state(&ks);
                    al_get_mouse_state(&ms);
                    
                    for(int i = 0; i < ENEMY_AMT; i ++){
                        horde.enemy[i].bullet.speed = 25;
                    }

                    // RESETA ALTERAÇOES TEMPORARIAS
                    reset_info(&world, &player, &ms);

                    //OPENS MENU ON "ESC"====================================================================================
                    pause_game(&ks, &running_screen);

                    //PLAYER MOVES LEFT====================================================================================
                    player.move(&world, &player, &ks, &ms, running_screen, &horde);

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
                    enemy_to_player_damage(&world, &player, &horde, &running_screen);
                    player_heart_collision(world, &player, &heart);

                    if(horde.enemies_remaining == 0 || al_key_down(&ks, ALLEGRO_KEY_B)){  // resets info and goes to next stage when all enemies are dead
                        player.aim = RIGHT;
                        running_screen = GAME_BOSS_STATE;
                        player.x = 100;
                        player.y = world.screen_height/2;
                        world.counter = 0;
                        player.rgb[0] = 255;
                        player.rgb[1] = 255;
                        player.rgb[2] = 255;

                        boss.timer = 0;
                    }
            
                    redraw = true; // set to be redrawn
                }
                //BOSS ======================================================================================
                else if(running_screen == GAME_BOSS_STATE){  // BOSS phase
                    al_get_keyboard_state(&ks);
                    al_get_mouse_state(&ms);
                    // RESETA ALTERAÇOES TEMPORARIAS
                    boss.rgb[0] = 255;
                    boss.rgb[1] = 255;
                    boss.rgb[2] = 255;

                    if(world.counter > 120  && boss.life > 0){

                        if(boss.direction == DOWN){   // BOSS ANIMATION
                            boss.timer++;
                        }
                        else{
                            boss.timer--;
                        }
                                      
                        if(boss.timer < 0){
                            boss.direction = DOWN;
                            boss.timer = 0;
                        }
                        if(boss.timer > 30){
                            boss.direction = UP;
                            boss.timer = 30;
                        }

                        boss_attack_logic(&boss, world);

                        reset_info(&world, &player, &ms);

                        //OPENS MENU ON "ESC"====================================================================================
                        pause_game(&ks, &running_screen);

                        //PLAYER MOVES LEFT====================================================================================
                        player.move(&world, &player, &ks, &ms, running_screen, NULL);

    
                        //SHOOTER LOGIC====================================================================================

                        player.shoot(&world, &player, weapon, backup, &ks, &ms);

                        //BOSS STUFF===============================================================================================

                        player_to_boss_damage(world, &boss, weapon);
                        boss_to_player_damage(&world, &player, &boss, &running_screen);
                        
                    }

                    if(boss.life == 0){  // BOSS DEATH ANIMATION
                        boss.death_timer++;
                        if(boss.death_timer > 45){
                            running_screen = YOU_WIN;
                            world.counter = 0;
                        }
                        boss.timer = boss.timer + 2;
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
            if(running_screen ==  MENU){  // PRINT MENU INFO
                al_clear_to_color(al_map_rgb(255, 255, 255));

                al_draw_scaled_bitmap(bkg_menu, 0, 0, 1280, 720, 0, 0, world.screen_width, world.screen_height, 0);
                
                al_draw_text(font, al_map_rgb(255, 255, 255), world.screen_width - 320, world.screen_height - 12, 0, "Ken no subarashii sabaku taikai - V1.0");

                al_draw_scaled_bitmap(play_bt, 0, 0, 120, 40, (world.screen_width/2) - (button_width_play/2), 460, button_width_play, button_height_play, 0);
                al_draw_scaled_bitmap(leave_bt, 0, 0, 120, 40, (world.screen_width/2) - (button_width_leave/2), 550, button_width_leave, button_height_leave, 0);

                al_draw_scaled_bitmap(main_title, 0, 0, 448, 256, (world.screen_width/2) - 250, (world.screen_width/2) - 550, 500, 286, 0);
                
            }

            //GAME_OVER SCREEN
            if(running_screen ==  GAME_OVER){ // PRINT DEATH INFO
                al_clear_to_color(al_map_rgb(0, 0, 0));

                al_draw_scaled_bitmap(leave_bt, 0, 0, 120, 40, (world.screen_width/2) - (button_width_leave/2), 500, button_width_leave, button_height_leave, 0);

                al_draw_scaled_bitmap(game_over,0, 0, 512, 128, 0, (world.screen_height/2) - 160, world.screen_width, 320, 0);                
                
            }

            //WIN SCREEN
            if(running_screen ==  YOU_WIN){  // PRINT WIN INFO
                al_clear_to_color(al_map_rgb(0, 0, 0));

                al_draw_scaled_bitmap(win_bkg, 0, 0, 1920, 1080, 0, 0, world.screen_width, world.screen_height, 0);

                al_draw_scaled_bitmap(leave_bt, 0, 0, 120, 40, (world.screen_width/2) - (button_width_leave/2), 500, button_width_leave, button_height_leave, 0);

                
            }

            //GAME SCREEN
            if(running_screen ==  GAME){  // PRINT MAIN GAME INFO
                

                if(player.direction == RIGHT){
                    player.sprite_off_y = 64;
                }
                else{
                    player.sprite_off_y = 0;    // GUARANTEES CORRECT PLAYER SPRITE
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
                
                if(heart.available){  // heart object
                    al_draw_scaled_bitmap(player.heart, 0, 0, 32, 32, heart.x - world.screen_limit_L, heart.y, heart.size, heart.size, 0);
                }

                //ENEMIES
                for(int i = 0; i < ENEMY_AMT; i++){
                    if(horde.enemy[i].state == ALIVE){
                        al_draw_tinted_scaled_bitmap(horde.enemy_sprite, al_map_rgb(horde.enemy[i].rgb[0], horde.enemy[i].rgb[1], horde.enemy[i].rgb[2]), horde.enemy[i].sprite_off_x, horde.enemy[i].sprite_off_y, player.og_dimensions, player.og_dimensions, horde.enemy[i].x, horde.enemy[i].y, player.dimensions, player.dimensions, 0);
                        al_draw_scaled_bitmap(horde.proj_sprite, 0, 0, 32, 32, horde.enemy[i].bullet.x, horde.enemy[i].bullet.y, 25, 25, 0);
                    }
                   
                }

            }

            if(running_screen ==  GAME_BOSS_STATE){  // PRINT BOSS GAME INFO
                
                player.sprite_off_y = 64;
                player.sprite_off_x = 448;  // GUARANTEES CORRECT PLAYER SPRITE
               
                
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
                
                //BOSS_ATTACKS
                for(int i = 0; i < 4; i++){
                    if(boss.attack[i].appear){
                        al_draw_filled_rectangle(boss.attack[i].x, boss.attack[i].y, (boss.attack[i].x + boss.attack[i].width), (boss.attack[i].y + boss.attack[i].height), al_map_rgb(255, 0, 100));
                    }
                }
                
                //BOSS
                al_draw_tinted_scaled_bitmap(boss.enemy_sprite, al_map_rgb(boss.rgb[0],boss.rgb[1],boss.rgb[2]), 0, 0, 64, 256, world.screen_width - 192, (boss.timer * 7), 128, 4*128, 0);
                

                if(world.counter < 30){   // COUNTDOWN ANIMATION
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
            
            

            al_flip_display();  // SEND INFO TO SCREEN
            redraw = false;
        }
        
        world.counter++; //UPDATE UNIVERSAL TIMER

    }
    weapon = destroy_weapon(weapon);
    backup = destroy_weapon(backup);
    al_destroy_bitmap(world.bkg);
    al_destroy_bitmap(player.sprite);
    al_destroy_bitmap(horde.enemy_sprite);
    al_destroy_bitmap(boss.enemy_sprite);
    al_destroy_bitmap(player.heart);
    al_destroy_bitmap(bkg_boss);
    al_destroy_bitmap(bkg_menu);
    al_destroy_bitmap(win_bkg);
    al_destroy_bitmap(play_bt);
    al_destroy_bitmap(leave_bt);
    al_destroy_bitmap(main_title);
    al_destroy_bitmap(game_over);
    al_destroy_bitmap(bosstxt);
    al_destroy_bitmap(t3);
    al_destroy_bitmap(t2);
    al_destroy_bitmap(t1);
    al_destroy_bitmap(go);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}