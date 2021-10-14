#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gfc_matrix.h"

typedef struct
{
    Bool _active;
    Vector3D position;
    Vector3D forwardVec;
    Vector3D rotation;  //x, y, z. Y not used in this case, since it would be roll
    Matrix4 cameraMat;

}Camera;

#endif
