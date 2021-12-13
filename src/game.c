#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "gfc_audio.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_sprite.h"

#include "g_entity.h"
#include "g_random.h"
#include "g_time.h"
#include "g_monster.h"
#include "g_light.h"

#include "o_bed.h"
#include "o_locker.h"
#include "o_lever.h"
#include "o_vent.h"

#include "p_player.h"

#include "w_world.h"
#include "w_room.h"

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 0;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;

    int editor = 1;
    int game = 1;

    Player* player;
    Monster* monster;

    SDL_Event event;
    Bool drawWireframe = false;

    Sprite* test, *mainMenuIcon, * gameOverIcon, * winIcon;

    Sound* bgMusic, *roar;

    //int x, y;

    
    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }



    init_logger("gf3d.log");    
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1920,                   //screen width
        1080,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        false,                      //fullscreen
        validate                //validation
    );
	slog_sync();

    // main game loop
    slog("gf3d main loop begin");
	slog_sync();

    entity_manager_init(1000);
    object_manager_init(500);
    lighting_manager_init(200);
    gf3d_camera_init();
    world_init(60);
    init_random();
    init_time();
    gfc_audio_init(32, 6, 4, 8, true, false);

    SDL_SetRelativeMouseMode(SDL_ENABLE);

    mainMenuIcon = gf3d_sprite_load("images/Mainscreen.png", 3840, 2160, 1);
    gameOverIcon = gf3d_sprite_load("images/gameOver.png", 3840, 2160, 1);
    winIcon = gf3d_sprite_load("images/winScreen.png", 3840, 2160, 1);


    Vector3D playerSpawn = vector3d(0, 0, -19);
    player = player_new(playerSpawn);

    while (!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

        update_time();


        gf3d_camera_update();

        gf3d_vgraphics_render_start();

        gf3d_sprite_draw(mainMenuIcon, vector2d(0, 0), vector2d(1, 1), 0);

        gf3d_vgraphics_render_end();

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        if (keys[SDL_SCANCODE_L])
        {
            done = 1;
            editor = 0;
            game = 1;
        }
        else if (keys[SDL_SCANCODE_RETURN])
        {
            done = 1;
            editor = 1;
            game = 0;
        }

    }

    if (!editor)
    {
        Room* editorRoom = world_new_room();
        editorRoom->model = gf3d_model_load("roomLarge");
        room_make_hitboxs(editorRoom);
        room_set_position(vector3d(0, 0, 0), editorRoom);
        room_setup_doors(editorRoom);

        editorRoom->editorRoom = true;

        Uint32 lastPress = get_current_time();
        Uint32 cooldown = 1000;


        monster = monster_new();

        monster->ent->position = vector3d(0, 0, -100000);

        light_set_ambient_color(vector4d(1.0, 1.0, 1.0, 1.0), player->light);

        //SDL_ShowCursor(SDL_DISABLE)

        entity_manager_think();

        //update functions
        entity_manager_update();

        while (!editor)
        {
            SDL_PumpEvents();   // update SDL's internal event structures
            keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

            update_time();
            Object* currentObj;

            //slog("Editor");


            //test functions to add objects for editor
            if (get_current_time() > lastPress + cooldown)
            {
                if (keys[SDL_SCANCODE_B])
                {
                    currentObj = bed_new(vector3d(player->ent->position.x, player->ent->position.y + 10, player->ent->position.z), vector3d(0, 0, 0));
                    lastPress = get_current_time();
                }
                if (keys[SDL_SCANCODE_L])
                {
                    currentObj = locker_new(vector3d(player->ent->position.x, player->ent->position.y + 10, player->ent->position.z), vector3d(0, 0, 0));
                    lastPress = get_current_time();
                }
                if (keys[SDL_SCANCODE_N])
                {
                    currentObj = lever_new(vector3d(player->ent->position.x, player->ent->position.y + 10, player->ent->position.z), vector3d(0, 0, 0), editorRoom);
                    lastPress = get_current_time();
                }
                if (keys[SDL_SCANCODE_V])
                {
                    currentObj = vent_new(vector3d(player->ent->position.x, player->ent->position.y + 10, player->ent->position.z), vector3d(0, 0, 0), editorRoom);
                    lastPress = get_current_time();
                }
            }
            
            if (currentObj)
            {
                if (keys[SDL_SCANCODE_UP])
                {
                    currentObj->position.y += 5 * .001;
                    currentObj->hitbox->center = currentObj->position;
                    currentObj->interactBox->center = currentObj->position;

                }
                else if (keys[SDL_SCANCODE_DOWN])
                {
                    currentObj->position.y -= 5 * .001;
                    currentObj->hitbox->center = currentObj->position;
                    currentObj->interactBox->center = currentObj->position;

                }
                else if (keys[SDL_SCANCODE_RIGHT])
                {
                    currentObj->position.x += 5 * .001;
                    currentObj->hitbox->center = currentObj->position;
                    currentObj->interactBox->center = currentObj->position;

                }
                else if (keys[SDL_SCANCODE_LEFT])
                {
                    currentObj->position.x -= 5 * .001;
                    currentObj->hitbox->center = currentObj->position;
                    currentObj->interactBox->center = currentObj->position;

                }

                else if (keys[SDL_SCANCODE_R] && get_current_time() > lastPress + cooldown)
                {
                    currentObj->rotation.z += M_PI / 2;
                    currentObj->hitbox->center = currentObj->position;
                    currentObj->interactBox->center = currentObj->position;

                    lastPress = get_current_time();

                }

                else if (keys[SDL_SCANCODE_T] && get_current_time() > lastPress + cooldown)
                {
                    currentObj->position.z += 5 * .001;
                    currentObj->hitbox->center = currentObj->position;
                    currentObj->interactBox->center = currentObj->position;

                }

                else if (keys[SDL_SCANCODE_G] && get_current_time() > lastPress + cooldown)
                {
                    currentObj->position.z -= 5 * .001;
                    currentObj->hitbox->center = currentObj->position;
                    currentObj->interactBox->center = currentObj->position;

                }

                if (keys[SDL_SCANCODE_RETURN])
                {
                    currentObj = NULL;
                }
            }

            if (keys[SDL_SCANCODE_BACKSPACE] && get_current_time() > lastPress + cooldown)
            {
                //save all objects as template

                editor_save_layout(editorRoom);
                slog("Attempting to save layout");

                lastPress = get_current_time();

            }
            


            //think functions
            entity_manager_think();


            //check collisions
            entity_manager_check_collions();


            //update functions
            entity_manager_update();



            //camera update
            gf3d_camera_update();


            //draw calls
            gf3d_vgraphics_render_start();


            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_KEYDOWN && event.button.button == SDL_SCANCODE_P)
                {
                    if (drawWireframe == false)
                    {
                        drawWireframe = true;
                    }
                    else if (drawWireframe == true)
                    {
                        drawWireframe = false;
                    }
                }
            }


            if (drawWireframe)
            {
                entity_manager_draw_hitboxes();
                world_draw_hitboxes();
                object_manager_draw_hitboxes();

            }




            entity_manager_draw();
            world_draw();
            object_manager_draw();


            gf3d_vgraphics_render_end();

            if (keys[SDL_SCANCODE_ESCAPE])editor = 1; // exit condition
        }

        vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());
    }

    if (!game)
    {
        world_layout_rooms();
        monster = monster_new();

        //SDL_ShowCursor(SDL_DISABLE)

        entity_manager_think();

        //update functions
        entity_manager_update();

        bgMusic = gfc_sound_load("audio/Condemned_ Criminal Origins Unofficial ST - Stray.wav", 1, 1);
        roar = gfc_sound_load("audio/Monster Roar.wav", 1, 1);
        gfc_sound_play(bgMusic, -1, .05, -1, -1);


        while (!game)
        {
            SDL_PumpEvents();   // update SDL's internal event structures
            keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

            update_time();


            //test functions to add objects
            if (keys[SDL_SCANCODE_B])
            {
                bed_new(vector3d(player->ent->position.x, player->ent->position.y + 10, player->ent->position.z), vector3d(0, 0, 0));
            }
            if (keys[SDL_SCANCODE_L])
            {
                locker_new(vector3d(player->ent->position.x, player->ent->position.y + 10, player->ent->position.z), vector3d(0, 0, 0));
            }

            //think functions
            entity_manager_think();


            //check collisions
            entity_manager_check_collions();


            //update functions
            entity_manager_update();



            //camera update
            gf3d_camera_update();


            //draw calls
            gf3d_vgraphics_render_start();


            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_KEYDOWN && event.button.button == SDL_SCANCODE_P)
                {
                    if (drawWireframe == false)
                    {
                        drawWireframe = true;
                    }
                    else if (drawWireframe == true)
                    {
                        drawWireframe = false;
                    }
                }
            }


            if (drawWireframe)
            {
                entity_manager_draw_hitboxes();
                world_draw_hitboxes();
                object_manager_draw_hitboxes();

            }




            entity_manager_draw();
            world_draw();
            object_manager_draw();

            if (player->ent->dead && !monster->attacking)
            {
                gf3d_sprite_draw(gameOverIcon, vector2d(0, 0), vector2d(1, 1), 0);
            }

            if (player->win)
            {
                gf3d_sprite_draw(winIcon, vector2d(0, 0), vector2d(1, 1), 0);
            }

            if (monster->playSound)
            {
                
                gfc_sound_play(roar, -1, .05, 2, 0);
                monster->playSound = false;
            }



            gf3d_vgraphics_render_end();

            slog("Monster Pos: X:%f, Y:%f, Player Pos: X:%f, Y:%f", monster->ent->position.x, monster->ent->position.y, player->ent->position.x, player->ent->position.y);

            if (keys[SDL_SCANCODE_ESCAPE])game = 1; // exit condition
        }


        vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());
    }
       
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
