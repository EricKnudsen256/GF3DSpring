#include "simple_logger.h"

#include "gfc_matrix.h"

#include "gf3d_camera.h"
#include "gf3d_vgraphics.h"

#include <string.h>

Camera camera = { 0 };
float num1 = 0;
float num2 = 0;


void gf3d_camera_init()
{
    gfc_matrix_identity(camera.view);

    gf3d_camera_look_at(
        vector3d(0, 0, 0),
        vector3d(0, 1, 0),
        vector3d(0, 0, 1)
    );
    
    camera.rotation = vector3d(0, 0, 0);
    
    camera.position = vector3d(0, 0, 0);

    camera._active = 1;
}

void gf3d_camera_update()
{
    //Order of operations here, rotate, scale, transform
    
    gf3d_camera_look_at(
        vector3d(0, 0, 0),
        vector3d(0, 1, 0),
        vector3d(0, 0, 1)
    );
    
    

    gfc_matrix_rotate(
        camera.view,
        camera.view,
        camera.rotation.z,
        vector3d(1, 0, 0));
    //camera.view[0][1] = 0;
    //camera.view[2][0] = 0;

    gfc_matrix_rotate(
        camera.view,
        camera.view,
        camera.rotation.x,
        vector3d(0, 0, 1));
    //camera.view[0][1] = 0;
    //camera.view[2][0] = 0;
    
    
    
    //scale here
    
    //translation
    
    gfc_matrix_translate(camera.view, camera.position);
    
    slog("T: X:%f, Y:%f, Z:%f", camera.position.x, camera.position.y, camera.position.z);
    
    slog("R: X:%f, Y:%f, Z:%f", camera.rotation.x, camera.rotation.y, camera.rotation.z);
    
    gf3d_set_view(camera.view);
    
}

void gf3d_camera_look_at(
    Vector3D position,
    Vector3D target,
    Vector3D up
)
{
    gfc_matrix_view(
        camera.view,
        position,
        target,
        up
    );
}

void gf3d_camera_set_position(Vector3D position)
{
    camera.position.x = position.x;
    camera.position.y = position.y;
    camera.position.z = position.z;
}

void gf3d_camera_move(Vector3D move)
{
    camera.position.x += move.x;
    camera.position.y += move.y;
    camera.position.z += move.z;
}

void gf3d_rotate_camera(float degrees, int axis)
{

    if(axis == 2)
    {
        camera.rotation.x += degrees;
    }
    else if(axis == 0)
    {
        camera.rotation.z += degrees;
    }

}

void gf3d_camera_test1()
{
    num1 += .001;
    camera.view[2][2] = num1;
}

void gf3d_camera_test2()
{
    num1 -= .001;
    camera.view[2][2] = num1;
}

/*eol@eof*/
