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
        vector3d(0, 10, 0),
        vector3d(0, 0, 1)
    );

    camera._active = 1;
}

void gf3d_camera_update()
{
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
    camera.view[0][3] = position.x;
    camera.view[1][3] = position.y;
    camera.view[2][3] = position.z;
}

void gf3d_camera_move(Vector3D move)
{
    camera.view[0][3] += move.x;
    camera.view[1][3] += move.y;
    camera.view[2][3] += move.z;
}

void gf3d_rotate_camera(float degrees, int axis)
{

    if (axis == 0)
    {
        gfc_matrix_rotate(
            camera.view,
            camera.view,
            degrees,
            vector3d(1, 0, 0));
        //camera.view[0][1] = 0;
        //camera.view[2][0] = 0;
    }
    else if (axis == 2)
    {
        gfc_matrix_rotate(
            camera.view,
            camera.view,
            degrees,
            vector3d(0, 0, 1));
        //camera.view[0][1] = 0;
        //camera.view[2][0] = 0;
    }

}

void gf3d_translate_camera(float x, float y, float z)
{
    gfc_matrix_translate(camera.view, vector3d(x, y, z));
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
