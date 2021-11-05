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

#include "g_entity.h"
#include "g_random.h"

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
    VkCommandBuffer commandBuffer, wireCommandBuffer;
    Entity* dino1;
    Entity* dino2;
    Model* model;
    Pipeline *pipe, *wirePipe;

    Player* player;

    SDL_Event event;
    Bool drawWireframe = false;

    int x, y;

    
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
    world_init(100);
    init_random();


    Vector3D playerSpawn = vector3d(0, 0, -5);

    player = player_new(playerSpawn);

    dino1 = entity_new();

    dino1->model = gf3d_model_load("dino");
    gfc_matrix_identity(dino1->modelMat);
    dino1->position = vector3d(20,20,0);

    world_layout_rooms();
    

    entity_make_hitbox(vector3d(10, 8, 10), vector3d(0, .5, -1.5), dino1);

    //SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_ENABLE);

    

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame


        //think functions
        entity_manager_think();


        //check collisions
        entity_manager_check_collions();


        //update functions
        entity_manager_update();



        //camera update
        gf3d_camera_update();

        pipe = gf3d_vgraphics_get_graphics_pipeline();
        wirePipe = gf3d_vgraphics_get_wireframe_pipeline();

        //draw calls
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(pipe, wirePipe, bufferFrame);
    
            commandBuffer = gf3d_command_rendering_begin(bufferFrame, pipe, wirePipe);

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
                gf3d_command_rendering_next_pipeline(bufferFrame, commandBuffer, wirePipe);

                entity_manager_draw_hitboxes(bufferFrame, commandBuffer);
                world_draw_hitboxes(bufferFrame, commandBuffer);
            }

            entity_manager_draw(bufferFrame, commandBuffer);
            world_draw(bufferFrame, commandBuffer);

            gf3d_command_rendering_end(commandBuffer);

            gf3d_vgraphics_render_end(bufferFrame);

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
