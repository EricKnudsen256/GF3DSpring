#include "simple_logger.h"

#include "gfc_matrix.h"
#include "gfc_vector.h"

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

    int x, y;

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    SDL_GetRelativeMouseState(&x, &y);
    
    
    gf3d_camera_look_at(
        vector3d(0, 0, 0),
        vector3d(0, 1, 0),
        vector3d(0, 0, 1)
    );
    

    gfc_matrix_rotate(
        camera.view,
        camera.view,
        camera.rotation.x,
        vector3d(1, 0, 0));
    
    
    gfc_matrix_rotate(
        camera.view,
        camera.view,
        camera.rotation.z,
        vector3d(0, 0, 1));
    //camera.view[0][1] = 0;
    //camera.view[2][0] = 0;


    //camera.view[0][1] = 0;
    //camera.view[2][0] = 0;



    Vector3D xaxis = vector3d(camera.view[0][0], camera.view[1][0], camera.view[2][0]);
    Vector3D yaxis = vector3d(camera.view[0][1], camera.view[1][1], camera.view[2][1]);
    Vector3D zaxis = vector3d(camera.view[0][2], camera.view[1][2], camera.view[2][2]);

    //getting wrong input from rotation values, need rotation to be based on actual degrees, use sin + cos for rotation amount
    gf3d_camera_angle_vectors(camera.rotation, &camera.forward, &camera.right, &camera.up);

    camera.back = vector3d(-camera.forward.x, -camera.forward.y,-camera.forward.z);
    camera.left = vector3d(-camera.right.x, -camera.right.y, -camera.right.z);
    camera.down = vector3d(-camera.up.x, -camera.up.y, -camera.up.z);

    vector3d_scale(camera.forward, camera.forward, .01);
    vector3d_scale(camera.back, camera.back, .01);
    vector3d_scale(camera.right, camera.right, .01);
    vector3d_scale(camera.left, camera.left, .01);
    vector3d_scale(camera.up, camera.up, .01);
    vector3d_scale(camera.down, camera.down, .01);


    camera.view[3][0] = -vector3d_dot_product(xaxis, camera.position);
    camera.view[3][1] = -vector3d_dot_product(yaxis, camera.position);
    camera.view[3][2] = -vector3d_dot_product(zaxis, camera.position);
    
    
    //gf3d_camera_make_rotation();
    
    //scale here
    
    //translation



    
    //slog("T: X:%f, Y:%f, Z:%f", camera.position.x, camera.position.y, camera.position.z);
    
    //slog("R: X:%f, Y:%f, Z:%f", camera.rotation.x, camera.rotation.y, camera.rotation.z);
    
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

void gf3d_camera_set_rotation(Vector3D position)
{
    camera.rotation.x = position.x;
    camera.rotation.y = position.y;
    camera.rotation.z = position.z;
}

void gf3d_camera_move(Vector3D move)
{
    camera.position.x += move.x;
    camera.position.y += move.y;
    camera.position.z += move.z;
}

void gf3d_rotate_camera(float degrees, int axis)
{

    if(axis == 0)
    {
        camera.rotation.x += degrees;
    }
    else if(axis == 2)
    {
        camera.rotation.z += degrees;
    }

}

void gf3d_camera_make_rotation()
{

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

void gf3d_camera_angle_vectors(Vector3D angles, Vector3D *forward, Vector3D *right, Vector3D *up)
{
  float angle;
  float sr, sp, sy, cr, cp, cy;
  
  angle = angles.z;
  sy = sin(angle);
  cy = cos(angle);
  angle = angles.x;
  sp = sin(angle);
  cp = cos(angle);
  angle = angles.y;
  sr = sin(angle);
  cr = cos(angle);
  
  if(forward)
  {
    forward->x = cp*sy;
    forward->y = cy;
    forward->z = 0;
  }
  if(right)
  {
    right->x = cy;
    right->y = -1*sy*cp;
    right->z = 0;
  }
  if(up)
  {
    up->x = 0;
    up->y = 0;
    up->z = 1;
  }
}

Camera gf3d_get_camera()
{
    return camera;
}

/*eol@eof*/
