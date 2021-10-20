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
    VkCommandBuffer commandBuffer;
    Entity* dino1;
    Entity* dino2;
    Model* model;

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

    //SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_ENABLE);

    //gf3d_vgraphics_rotate_camera(0, 2);

    

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here
        
        //gf3d_vgraphics_rotate_camera(0.001, 2);

        SDL_GetRelativeMouseState(&x, &y);


        //gf3d_vgraphics_test_function((int)test);
       

        
        if (x != 0)
        {
            //slog("x:%i", x);
            gf3d_rotate_camera(0.001 * x, 2);
        }
        if (y != 0)
        {
            gf3d_rotate_camera(0.001 * y, 0);
        }


        if (keys[SDL_SCANCODE_W])
        {
            //gf3d_camera_test1();
            gf3d_camera_move(vector3d(0, 0, -.01));
        }
        else if (keys[SDL_SCANCODE_S])
        {
            //gf3d_camera_test2();
            gf3d_camera_move(vector3d(0, 0, .01));
        }

        if (keys[SDL_SCANCODE_A])
        {
            gf3d_camera_move(vector3d(-.01, 0, 0));
        }
        else if (keys[SDL_SCANCODE_D])
        {
            gf3d_camera_move(vector3d(.01, 0, 0));
        }

        if (keys[SDL_SCANCODE_SPACE])
        {
            gf3d_camera_move(vector3d(0, .01, 0));
        }
        else if (keys[SDL_SCANCODE_LSHIFT])
        {
            gf3d_camera_move(vector3d(0, -.01, 0));
        }


        gf3d_camera_update();
        

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);

                entity_manager_draw(bufferFrame, commandBuffer);

                //gf3d_model_draw(model,bufferFrame,commandBuffer,modelMat);
                //gf3d_model_draw(model2,bufferFrame,commandBuffer,modelMat2);
                
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
