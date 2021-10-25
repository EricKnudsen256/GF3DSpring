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

    float test = 0;

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
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        validate                //validation
    );
	slog_sync();

    // main game loop
    slog("gf3d main loop begin");
	slog_sync();

    entity_manager_init(1000);
    gf3d_camera_init();

    dino1 = entity_new();
    dino2 = entity_new();

    dino1->model = gf3d_model_load("dino");
    gfc_matrix_identity(dino1->modelMat);
    gfc_matrix_make_translation(
        dino1->modelMat,
            vector3d(0,20,0)
        );

    entity_make_hitbox(vector3d(10, 10, 10), dino1);

    //SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_ENABLE);

    //gf3d_vgraphics_rotate_camera(0, 2);

    

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here

        pipe = gf3d_vgraphics_get_graphics_pipeline();
        wirePipe = gf3d_vgraphics_get_wireframe_pipeline();
        

       
        gf3d_camera_update();
        

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(pipe, wirePipe, bufferFrame);
    
            commandBuffer = gf3d_command_rendering_begin(bufferFrame, pipe, wirePipe);

            entity_manager_draw(bufferFrame, commandBuffer);

            //gf3d_command_rendering_next_pipeline(bufferFrame, commandBuffer, wirePipe);

            entity_manager_draw_hitboxes(bufferFrame, commandBuffer);

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
