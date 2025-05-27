#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#define KEY_SEEN     1
#define KEY_DOWN     2

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

typedef struct player{
    float x;
    float y;
    int og_dimensions;
    int dimensions;
    int speed;

    int shadow_x;
    int shadow_y;
    int shadow_width;
    int shadow_height;

    ALLEGRO_BITMAP *sprite;
}player;

int main(){
    const float gravity = 40;
    player player;
    int screen_x = 1280;
    int screen_y = 720;
    float level_height = screen_y - 150;
    float frame = 0;
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    

    ALLEGRO_DISPLAY* disp = al_create_display(screen_x, screen_y);
    must_init(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* mysha = al_load_bitmap("mysha.png");
    must_init(mysha, "mysha");

    ALLEGRO_BITMAP* bkg = al_load_bitmap("bkg.png");
    must_init(bkg, "bkg");

    player.sprite = al_load_bitmap("kyara.png");
    must_init(player.sprite, "kyara");

    al_convert_mask_to_alpha(player.sprite, al_map_rgb(26, 255, 0));

    player.x = 100;
    player.y = level_height;
    player.og_dimensions = 64;
    player.dimensions = 100;
    player.speed = 30;

    int shadow_height = 7;
    int shadow_mod = 0;

    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    int air_round = 0;
    int ground_round = 0;
    bool jump = true; // jump enabled
    ALLEGRO_EVENT event;
    ALLEGRO_KEYBOARD_STATE ks;

    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
        
            case ALLEGRO_EVENT_TIMER:
                al_get_keyboard_state(&ks);

                if(player.x >= player.speed && (al_key_down(&ks, ALLEGRO_KEY_LEFT) || al_key_down(&ks, ALLEGRO_KEY_A)))
                    player.x -= player.speed;
                if(player.x <= (screen_x - player.speed - player.dimensions) && (al_key_down(&ks, ALLEGRO_KEY_RIGHT) || al_key_down(&ks, ALLEGRO_KEY_D)))
                    player.x += player.speed;

                if(al_key_down(&ks, ALLEGRO_KEY_SPACE) && jump){
                    ground_round = 0;
                    jump = false;
                    frame = 50;
                }

                if(al_key_down(&ks, ALLEGRO_KEY_ESCAPE))
                    done = true;
        
                player.y -= frame;
                
                if(player.y >= level_height){
                    player.y = level_height;
                    frame = 0;
                    ground_round++;
                    if(ground_round > 3){
                        jump = true;
                    }
                }

                if (player.y <= level_height){
                    shadow_mod = (player.y - level_height) / 100;
                }

                if (player.y <= level_height - (8*frame)){
                    air_round++;
                    if(al_key_down(&ks, ALLEGRO_KEY_SPACE) && air_round < 35){
                        frame = 0;
                    }
                    else{ 
                        frame = -gravity;
                    }

                }
 
   

                player.shadow_x = player.x + (player.dimensions/2);
                player.shadow_y = level_height + 93;
                player.shadow_height = shadow_height + shadow_mod;
                player.shadow_width = ((shadow_height + shadow_mod) * 3);
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
            ALLEGRO_VERTEX v[4];
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_scaled_bitmap(bkg, 0, 0, 4093, 2894, 0, 0, screen_x, screen_y, 0);
            al_draw_filled_ellipse(player.shadow_x, player.shadow_y, player.shadow_width, player.shadow_height, al_map_rgba_f(0, 0, 0, 0.5));
            al_draw_scaled_bitmap(player.sprite, 0, 0, player.og_dimensions, player.og_dimensions, player.x, player.y, player.dimensions, player.dimensions, 0);
            al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %3.1f Y: %3.1f", player.x, player.y);
            

            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_bitmap(mysha);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}