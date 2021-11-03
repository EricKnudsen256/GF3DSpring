#ifndef __ROOM_H__
#define __ROOM_H__

#include "gf3d_model.h"
#include "gfc_types.h"
#include "gfc_vector.h"

#include "g_hitbox.h"

typedef enum
{
    DOOR_UNDEFINED,
    DOOR_NEG_X,
    DOOR_NEG_Y,
    DOOR_POS_X,
    DOOR_POS_Y
}DoorType;

typedef struct
{
    Vector3D center;
    
}Door;

typedef struct
{
    Uint8 _inuse;
    Uint32 _id;
    Model *model;
    Vector3D position;
    Vector3D rotation;
    Vector3D velocity;
    Vector3D scale;
    Matrix4 modelMat;

    Hitbox **hitbox_list;
    Uint32 hitbox_max;
    
    Door *door_list;
    Uint32 max_doors;
    
}Room;

void room_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer, Room *room);

void room_make_hitbox(Vector3D dimensions, Vector3D offset, Room* room);



#endif
