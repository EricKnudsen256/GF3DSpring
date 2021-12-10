#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

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

    Player* player;
    Monster* monster;

    SDL_Event event;
    Bool drawWireframe = false;

    Sprite* test;

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
    gf3d_camera_init();
    world_init(60);
    init_random();
    init_time();


    Vector3D playerSpawn = vector3d(0, 0, -5);
    player = player_new(playerSpawn);

    world_layout_rooms();
    monster = monster_new();

    //SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_ENABLE);

    entity_manager_think();

    //update functions
    entity_manager_update();

    test = gf3d_sprite_load("images/slime.png", 64, 64, 1);

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

        update_time();


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
            
            /*
            if (drawWireframe)
            {
                gf3d_command_rendering_next_pipeline(bufferFrame, commandBuffer, wirePipe);

                entity_manager_draw_hitboxes();
                world_draw_hitboxes();
            }
            */
            


            entity_manager_draw();
            world_draw();

            //gf3d_sprite_draw(test, vector2d(100, 100), vector2d(1, 1), 0);

            gf3d_vgraphics_render_end();           

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
