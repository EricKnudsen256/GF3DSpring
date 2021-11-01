#ifndef __GF3D_CAMERA_H__
#define __GF3D_CAMERA_H__

#include "gfc_matrix.h"

typedef struct
{
    Bool _active;
    Vector3D scale;
    Vector3D rotation;
    Vector3D position;
    Vector3D forward, back;
    Vector3D right, left;
    Vector3D up, down;
    
    float time;
    
    Matrix4 view;

}Camera;

void gf3d_camera_init();

void gf3d_camera_update();
/**
 * @brief set the camera properties based on position and direction that the camera should be looking
 * @param position the location for the camera
 * @param target the point the camera should be looking at
 * @param up the direction considered to be "up"
 */
void gf3d_camera_look_at(
    Vector3D position,
    Vector3D target,
    Vector3D up
);


/**
 * @brief explicitely set the camera positon, holding all other parameters the same
 * @param position the new position for the camera
 */
void gf3d_camera_set_position(Vector3D position);

void gf3d_camera_set_rotation(Vector3D position);

void gf3d_rotate_camera(float degrees, int axis);

void gf3d_camera_make_rotation();

/**
 * @brief move the camera relatively based on the vector provided
 * @param move the ammount to move the camera
 */
void gf3d_camera_move(Vector3D move);

void gf3d_camera_angle_vectors(Vector3D angles, Vector3D *forward, Vector3D *right, Vector3D *up);

Camera gf3d_get_camera();

#endif
